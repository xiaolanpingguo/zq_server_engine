#include "game_server/login_server/client_to_login_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/login_server/login_server.h"
#include "game_common/game_db_def.hpp"
#include "game_common/bson_object.h"
#include "game_common/gid.h"
#include "db_mongo/mongo_module.h"
#include "db_redis/redis_module.h"


namespace zq{


#define CMD_LOGIN_2_DB_FIND_ACCOUNT_REQ 1
#define CMD_LOGIN_2_DB_INSERT_ACCOUNT_REQ 2

ClientToLoginModule::ClientToLoginModule(LoginServer* thisServer)
	:
		m_thisServer(thisServer)
{

}

ClientToLoginModule::~ClientToLoginModule()
{

}

bool ClientToLoginModule::init()
{
	using namespace std::placeholders;

	MessageHelper& messagehelper = MessageHelper::getInstance();
	messagehelper.registerHandler<&ClientToLoginModule::onC2SHeatBeatReq>(this, C2S::MSG_ID_HEARTBEAT);
	messagehelper.registerHandler<&ClientToLoginModule::onC2SClientLoginReq>(this, C2S::MSG_ID_LOGIN_REQ);

	m_tcpServer = std::make_unique<TcpServer<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().internalIp, m_thisServer->getConfig().internalPort);
	m_tcpServer->setClientConnectedCb(std::bind(&ClientToLoginModule::onClientConnected, this, _1));
	m_tcpServer->setClientDisconnectedCb(std::bind(&ClientToLoginModule::onClientDisconnected, this, _1));
	m_tcpServer->setClientDataReceivedCb(std::bind(&ClientToLoginModule::onClientDataReceived, this, _1, _2, _3, _4));
	if (!m_tcpServer->start())
	{
		LOG_ERROR(s_logCategory, "tcp server start failed!");
		return false;
	}

	return true;
}

bool ClientToLoginModule::finalize()
{
	return true;
}

void ClientToLoginModule::onClientConnected(TcpConnectionPtr connection)
{	
	LOG_INFO(s_logCategory, "a new client has connected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void ClientToLoginModule::onClientDisconnected(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a client has disconnected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void ClientToLoginModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void ClientToLoginModule::onC2SHeatBeatReq(TcpConnectionPtr connection, const C2S::C2SHeartBeat& msg)
{

}

void ClientToLoginModule::onC2SClientLoginReq(TcpConnectionPtr connection, const C2S::C2SLoginReq& msg)
{
	processLogin(connection, msg).start([](auto&&) {});
}

async_simple::coro::Lazy<void> ClientToLoginModule::processLogin(TcpConnectionPtr connection, const C2S::C2SLoginReq& msg)
{
	std::string sdkUserId = msg.sdk_user_id();
	std::string sdkToken = msg.sdk_token();
	int channelId = msg.channel_id();

	C2S::C2SLoginRes res;

	C2S::C2S_ERROR_CODE errorCode = C2S::EC_GENERRAL_ERROR;
	bool needDeleteSession = true;
	do 
	{
		if (sdkUserId.empty() || sdkToken.empty())
		{
			errorCode = C2S::EC_INVALID_PARAMETER;
			break;
		}

		// check if the user is online
		S2S::S2SPlayerSessionData sessionData;
		int r = co_await findUserSession(sdkUserId, sessionData);
		if (r == 0)
		{
			S2S::S2S_ONLINE_STATUS status = sessionData.online_status();
			if (status == S2S::ONLINE)
			{
				// user has online, then return client zone ip and port
				res.set_profile_id(sessionData.profile_id());
				res.set_ip(sessionData.ip());
				res.set_port(sessionData.port());
				res.set_zone_token(sessionData.login_token());
				errorCode = C2S::EC_SUCCESS;
				break;
			}
			else
			{
				// The last login is still in progress, return directly here
				co_return;
			}
		}

		r = co_await createSeesion(sdkUserId, sdkToken, channelId);
		if (r != 0)
		{
			if (r == -2)
			{
				needDeleteSession = false;
			}
			LOG_ERROR(s_logCategory, "createNewSeesion failed, user:{}:{}:{}.", sdkUserId, sdkToken, channelId);
			errorCode = C2S::EC_GENERRAL_ERROR;
			break;
		}

		// find user from DB
		BsonObjectPtr userAccountData = std::make_shared<BsonObject>();
		r = co_await findAndSaveUser(sdkUserId, sdkToken, channelId, userAccountData);
		if (r != 0)
		{
			LOG_ERROR(s_logCategory, "findAndSaveUser failed, user:{}:{}:{}.", sdkUserId, sdkToken, channelId);
			errorCode = (r < -1 ? C2S::EC_SERVER_INTERNAL_ERROR : C2S::EC_GENERRAL_ERROR);
			break;
		}

		// we got a user data from db
		std::string profileId = userAccountData->getString(ACCOUNT_KEY_PROFILE_ID);
		if (profileId.empty())
		{
			LOG_ERROR(s_logCategory, "user profileid is empty! user:{}:{}:{}.", sdkUserId, sdkToken, channelId);
			errorCode = C2S::EC_GENERRAL_ERROR;
			break;
		}

		// check which of these zone servers has the least load
		S2S::S2SZoneServerData serverData;
		r = co_await getZoneServer(serverData);
		if (r != 0)
		{
			LOG_ERROR(s_logCategory, "get zone server failed, user:{}:{}:{}.", sdkUserId, sdkToken, channelId);
			errorCode = (r < -1 ? C2S::EC_SERVER_INTERNAL_ERROR : C2S::EC_SERVER_NOT_READY);
			break;
		}

		res.set_profile_id(profileId);
		res.set_ip(serverData.ip());
		res.set_port(serverData.port());
		res.set_zone_token(sessionData.login_token());

		errorCode = C2S::EC_SUCCESS;
	} while (0);

	if (needDeleteSession && errorCode != C2S::EC_SUCCESS)
	{
		co_await deleteSession(sdkUserId);
	}

	res.set_error_code(errorCode);
	std::string strRes;
	if (!res.SerializeToString(&strRes))
	{
		LOG_ERROR(s_logCategory, "SerializeToString error!");
		co_return;
	}

	connection->sendData(C2S::MSG_ID_LOGIN_RES, strRes.data(), (uint32_t)strRes.size());
	connection->delayClose();
}

async_simple::coro::Lazy<int> ClientToLoginModule::findUserSession(const std::string& sdkUserId, S2S::S2SPlayerSessionData& sessionData)
{
	std::string strPb;
	bool success = co_await m_thisServer->getModule<RedisModule>()->GET(RD_USER_SESSION_KEY(sdkUserId), strPb);
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

async_simple::coro::Lazy<int> ClientToLoginModule::createSeesion(const std::string& sdkUserId, const std::string& sdkToken, int channelId)
{
	if (sdkUserId.empty() || sdkToken.empty())
	{
		co_return -1;
	}

	std::string strPb;
	S2S::S2SPlayerSessionData session;
	session.set_sdk_user_id(sdkUserId);
	session.set_sdk_token(sdkToken);
	session.set_channel_id(channelId);
	session.set_login_time(time(nullptr));
	session.set_online_status(S2S::WAITTING_TO_LOGIN_ZONE);
	if (!session.SerializeToString(&strPb))
	{
		LOG_ERROR(s_logCategory, "SerializeToString error!");
		co_return -1;
	}

	bool success = co_await m_thisServer->getModule<RedisModule>()->SETNX(RD_USER_SESSION_KEY(sdkUserId), strPb);
	if (!success)
	{
		// very unlikely to fail, because we have check it before by Redis GET
		// if failed, the client may have logged in to different login servers at the same time
		LOG_WARN(s_logCategory, "client try to logged in to different login servers at the same time.")
		co_return -2;
	}

	co_return 0;
}

async_simple::coro::Lazy<int> ClientToLoginModule::findAndSaveUser(const std::string& sdkUserId, const std::string& sdkToken, int channelId, BsonObjectPtr& userAccountData)
{
	if (sdkUserId.empty() || sdkToken.empty())
	{
		co_return -1;
	}

	BsonObjectPtr selector = std::make_shared<BsonObject>();
	selector->appendString(ACCOUNT_KEY_SDK_USER_ID, sdkUserId);
	MongoResultPtr result = co_await m_thisServer->getModule<MongoModule>()->find(DB_NAME, COL_ACCOUNT, selector);
	if (!result->success)
	{
		LOG_ERROR(s_logCategory, "find acccount db failed:{}.", result->errorMsg);
		co_return -2;
	}

	if (result->foundResult.size() > 1)
	{
		LOG_ERROR(s_logCategory, "user account data error!, there are {} result!", result->foundResult.size());
		co_return -3;
	}

	// we got a new user, save this account data
	if (result->foundResult.empty())
	{
		std::string profileId = Gid::genGid();
		BsonObjectPtr selector = std::make_shared<BsonObject>();
		selector->appendString(ACCOUNT_KEY_SDK_USER_ID, sdkUserId);
		BsonObjectPtr updator = std::make_shared<BsonObject>();
		updator->appendString(ACCOUNT_KEY_SDK_USER_ID, sdkUserId);
		updator->appendInt32(ACCOUNT_KEY_SDK_CHANNEL_ID, channelId);
		updator->appendString(ACCOUNT_KEY_PROFILE_ID, profileId);
		MongoResultPtr saveResult = co_await m_thisServer->getModule<MongoModule>()->save(DB_NAME, COL_ACCOUNT, selector, updator);
		if (!saveResult->success)
		{
			LOG_ERROR(s_logCategory, "save user account failed:{}", saveResult->errorMsg);
			co_return -4;
		}
	}

	userAccountData = result->foundResult[0];
	co_return 0;
}

async_simple::coro::Lazy<int> ClientToLoginModule::getZoneServer(S2S::S2SZoneServerData& serverData)
{
	StringScoreVector zoneInfo;
	bool success = co_await m_thisServer->getModule<RedisModule>()->ZRANGEWITHSCORE(RD_Z_ZONE_LOAD_KEY, 0, 0, zoneInfo);
	if (!success || zoneInfo.empty() || zoneInfo.size() != 1)
	{
		LOG_ERROR(s_logCategory, "there no any zone server.");
		co_return -1;
	}

	if (zoneInfo.size() != 1)
	{
		LOG_ERROR(s_logCategory, "redis data error, found at least data,size: {}", zoneInfo.size());
		co_return -2;
	}

	if (!serverData.ParseFromString(zoneInfo[0].first))
	{
		co_return -3;
	}

	co_return 0;
}

async_simple::coro::Lazy<int> ClientToLoginModule::deleteSession(const std::string& sdkUserId)
{
	int num = co_await m_thisServer->getModule<RedisModule>()->DEL(RD_USER_SESSION_KEY(sdkUserId));
	if (num == 0)
	{
		LOG_ERROR(s_logCategory, "deleteSession failed:{}.", sdkUserId);
		co_return -1;
	}

	co_return 0;
}

std::shared_ptr<Session> ClientToLoginModule::findSession(const std::string& sdkUserId)
{
	auto it = m_sessions.find(sdkUserId);
	if (it != m_sessions.end())
	{
		return it->second;
	}

	return nullptr;
}


}

