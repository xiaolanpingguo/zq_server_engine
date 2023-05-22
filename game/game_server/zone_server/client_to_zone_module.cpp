#include "game_server/zone_server/client_to_zone_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/zone_server/zone_server.h"
#include "game_common/game_db_def.hpp"
#include "db_mongo/mongo_module.h"
#include "db_redis/redis_module.h"
#include "game_server/zone_server/player_manager_module.h"
#include "game_server/zone_server/player/player.h"
#include "protocol/s2s/db_player.pb.h"


namespace zq
{

ClientToZoneModule::ClientToZoneModule(ZoneServer* thisServer) :
		m_thisServer(thisServer)
{
}

ClientToZoneModule::~ClientToZoneModule()
{
}

bool ClientToZoneModule::init()
{
	using namespace std::placeholders;

	MessageHelper& messagehelper = MessageHelper::getInstance();
	messagehelper.registerHandler<&ClientToZoneModule::onC2SHeatBeatReq>(this, C2S::MSG_ID_HEARTBEAT);
	messagehelper.registerHandler<&ClientToZoneModule::onC2SClientLoginReq>(this, C2S::MSG_ID_LOGIN_ZONE_REQ);

	m_tcpServer = std::make_unique<TcpServer<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().externalIp, m_thisServer->getConfig().externalPort);
	m_tcpServer->setClientConnectedCb(std::bind(&ClientToZoneModule::onClientConnected, this, _1));
	m_tcpServer->setClientDisconnectedCb(std::bind(&ClientToZoneModule::onClientDisconnected, this, _1));
	m_tcpServer->setClientDataReceivedCb(std::bind(&ClientToZoneModule::onClientDataReceived, this, _1, _2, _3, _4));
	if (!m_tcpServer->start())
	{
		LOG_ERROR(s_logCategory, "tcp server start failed!");
		return false;
	}

	return true;
}

bool ClientToZoneModule::finalize()
{
	return true;
}

void ClientToZoneModule::onClientConnected(TcpConnectionPtr connection)
{
	LOG_DEBUG(s_logCategory, "a new client has connected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void ClientToZoneModule::onClientDisconnected(TcpConnectionPtr connection)
{
	LOG_DEBUG(s_logCategory, "a client has disconnected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void ClientToZoneModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, uint32_t len)
{
}

void ClientToZoneModule::onC2SHeatBeatReq(TcpConnectionPtr connection, const C2S::C2SHeartBeat& msg)
{
}

void ClientToZoneModule::onC2SClientLoginReq(TcpConnectionPtr connection, const C2S::C2SLoginZoneReq& msg)
{
	processLogin(connection, msg).start([](auto&&) {});
}

async_simple::coro::Lazy<void> ClientToZoneModule::processLogin(TcpConnectionPtr connection, const C2S::C2SLoginZoneReq& msg)
{
	std::string profileId = msg.profile_id();

	C2S::C2SLoginZoneRes res;

	C2S::C2S_ERROR_CODE errorCode = C2S::EC_GENERRAL_ERROR;
	do
	{
		if (profileId.empty())
		{
			errorCode = C2S::EC_INVALID_PARAMETER;
			break;
		}

		S2S::S2SPlayerSessionData sessionData;
		int r = co_await findUserSession(profileId, sessionData);
		if (r != 0)
		{
			errorCode = C2S::EC_LOGIN_SESSION_HAS_EXPIRED;
			break;
		}

		if (sessionData.app_id() != m_thisServer->getAppIdStr())
		{
			errorCode = C2S::EC_LOGIN_IN_PROGRESS;
			break;
		}

		// check if the player is online
		Player* player = m_thisServer->getModule<PlayerManagerModule>()->findPlayer(profileId);
		if (player != nullptr)
		{
			// the player is online, then kickout old player
			kickoutPlayer(player);

			// set new player, actually we just only use new conntection instead of old onnection
			player->setConnection(connection);
		}
		else
		{
			// create a new player
			player = new Player();
			player->setConnection(connection);

			// get playey db data
			BsonObjectPtr result = nullptr;
			r = co_await getPlayerDBData(profileId, result);
			if (r == 0)
			{
				std::string playerBin = result->getString(PALYER_KEY_BIN);
				S2S::DBPlayerData dbPlayerData;
				if (!dbPlayerData.ParseFromString(playerBin))
				{
					errorCode = C2S::EC_GENERRAL_ERROR;
					break;
				}
			}
		}

		errorCode = C2S::EC_SUCCESS;
	}
	while (0);

	res.set_error_code(errorCode);
	std::string strRes;
	if (!res.SerializeToString(&strRes))
	{
		LOG_ERROR(s_logCategory, "SerializeToString error!");
		co_return;
	}

	connection->sendData(C2S::MSG_ID_LOGIN_ZONE_RES, strRes.data(), (uint32_t)strRes.size());
}

async_simple::coro::Lazy<int> ClientToZoneModule::findUserSession(const std::string& profileId, S2S::S2SPlayerSessionData& sessionData)
{
	std::string strPb;
	bool success = co_await m_thisServer->getModule<RedisModule>()->GET(RD_USER_SESSION_KEY(profileId), strPb);
	if (!success)
	{
		co_return -1;
	}

	// user has online, then return client zone ip and port
	if (!sessionData.ParseFromString(strPb))
	{
		// to do, delete this broken data?
		LOG_ERROR(s_logCategory, "parse string failed, please check your data!");
		co_return -2;
	}

	co_return 0;
}

async_simple::coro::Lazy<int> ClientToZoneModule::getPlayerDBData(const std::string& profileId, BsonObjectPtr playerBin)
{
	if (profileId.empty())
	{
		co_return -1;
	}

	BsonObjectPtr selector = std::make_shared<BsonObject>();
	selector->appendString(COL_PLAYER, PLAYER_KEY_PROFILE_ID);
	MongoResultPtr result = co_await m_thisServer->getModule<MongoModule>()->find(DB_NAME, COL_PLAYER, selector);
	if (!result->success)
	{
		LOG_ERROR(s_logCategory, "find player db failed:{}.", result->errorMsg);
		co_return -2;
	}

	if (result->foundResult.size() > 1)
	{
		LOG_ERROR(s_logCategory, "find player data error!, there are {} result!", result->foundResult.size());
		co_return -3;
	}

	if (result->foundResult.size() == 1)
	{
		playerBin = result->foundResult[0];
	}

	co_return 0;
}

void ClientToZoneModule::kickoutPlayer(Player* player)
{
	TcpConnectionPtr oldConnection = player->getConnection();
	C2S::C2SKickoutRes res;
	res.set_error_code(C2S::EC_SUCCESS);
	std::string strRes;
	if (!res.SerializeToString(&strRes))
	{
		LOG_ERROR(s_logCategory, "SerializeToString error!");
	}
	else
	{
		oldConnection->sendData(C2S::MSG_ID_LOGIN_ZONE_RES, strRes.data(), (uint32_t)strRes.size());
	}

	oldConnection->delayClose();
}

}

