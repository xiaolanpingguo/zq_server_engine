#include "game_server/login_server/client_to_login_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/login_server/login_server.h"
#include "game_common/game_db_def.hpp"
#include "game_common/bson_object.h"
#include "game_common/gid.h"
#include "db_mongo/mongo_module.h"


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
	std::string sdk_token = msg.sdk_token();
	int channeId = msg.channel_id();

	if (sdkUserId.empty() || sdk_token.empty())
	{
		co_return;
	}

	// the user login has in progress
	auto session = findSession(sdkUserId);
	if (session)
	{
		co_return;
	}

	std::string sessionId = Gid::genGid();

	session = std::make_shared<Session>();
	session->createTime = time(nullptr);
	session->sdkAccountInfo.sdkUserId = sdkUserId;
	session->sdkAccountInfo.channelId = channeId;
	session->sdkAccountInfo.token = sdk_token;
	session->sessionId = sessionId;
	session->status = SessionStatus::WAITING_DB_RES;
	session->connection = connection;
	m_sessions[sdkUserId] = session;

	// find user from DB
	BsonObjectPtr selector = std::make_shared<BsonObject>();
	selector->appendString(ACCOUNT_KEY_SDK_USER_ID, sdkUserId);
	MongoResultPtr result = co_await m_thisServer->getModule<MongoModule>()->find(DB_NAME, COL_ACCOUNT, selector);
	
	// check if session expired

	int errorCode = 1;

	do
	{
		if (!result->success)
		{
			LOG_ERROR(s_logCategory, "find acccount db failed:{}.", result->errorMsg);
			errorCode = 1;
			break;
		}

		if (result->foundResult.size() > 1)
		{
			LOG_ERROR(s_logCategory, "user account data error!, there are {} result!", result->foundResult.size());
			errorCode = 1;
			break;
		}

		// we got a new user, insert this account data
		if (result->foundResult.empty())
		{
			BsonObjectPtr insertor = std::make_shared<BsonObject>();
			insertor->appendString(ACCOUNT_KEY_SDK_USER_ID, sdkUserId);
			insertor->appendInt32(ACCOUNT_KEY_SDK_CHANNEL_ID, channeId);
			MongoResultPtr insertResult = co_await m_thisServer->getModule<MongoModule>()->insert(DB_NAME, COL_ACCOUNT, insertor);
			if (!insertResult->success)
			{
				LOG_ERROR(s_logCategory, "insert user account failed:{}", insertResult->errorMsg);
				int errorCode = 1;
				break;
			}
		}

		// check account info
		BsonObjectPtr account = result->foundResult[0];

		errorCode = 0;
	}
	while (0);

	C2S::C2SLoginRes res;
	res.set_error_code(errorCode);
	res.set_zone_token("");
	res.set_host("127.0.0.1");
	res.set_port(1234);

	std::string strRes;
	if (!res.SerializeToString(&strRes))
	{
		LOG_ERROR(s_logCategory, "SerializeToString error!");
		co_return;
	}

	connection->sendData(C2S::MSG_ID_LOGIN_RES, strRes.data(), (uint32_t)strRes.size());
}

void ClientToLoginModule::onS2SFindAccountRes(bool success, const S2S::MongoUserData& userData, const std::vector<BsonObject>& result)
{
	std::string sdkUserId = userData.string_var1();
	std::shared_ptr<Session> session = findSession(sdkUserId);
	if (session == nullptr)
	{
		LOG_ERROR(s_logCategory, "user seesion has deleted.");
		return;
	}

	// new user, insert this account data
	if (result.empty())
	{
		S2S::MongoUserData data;
		data.set_cmd(CMD_LOGIN_2_DB_INSERT_ACCOUNT_REQ);
		data.set_string_var1(sdkUserId);

		BsonObject obj;
		obj.appendString(ACCOUNT_KEY_SDK_USER_ID, session->sdkAccountInfo.sdkUserId);
		obj.appendInt32(ACCOUNT_KEY_SDK_CHANNEL_ID, session->sdkAccountInfo.channelId);
		//m_thisServer->getModule<LoginToDBModule>()->requestInsert(DB_NAME, COL_ACCOUNT, obj, &data);
		return;
	}

	C2S::C2SLoginRes res;
	int errorCode = 1;

	do 
	{
		if (!success)
		{
			LOG_ERROR(s_logCategory, "find acccount db failed.");
			errorCode = 1;
			break;
		}

		if (result.size() > 1)
		{
			LOG_ERROR(s_logCategory, "user account data error!, there are {} result!", result.size());
			errorCode = 1;
			break;
		}

		errorCode = 0;
	} while (0);

	res.set_error_code(errorCode);

	std::string strRes;
	if (!res.SerializeToString(&strRes))
	{
		LOG_ERROR(s_logCategory, "SerializeToString error!");
		return;
	}

	session->connection->sendData(C2S::MSG_ID_LOGIN_RES, strRes.data(), (uint32_t)strRes.size());
}

void ClientToLoginModule::onS2SInsertAccountRes(bool success, const S2S::MongoUserData& userData)
{
	std::string sdkUserId = userData.string_var1();
	std::shared_ptr<Session> session = findSession(sdkUserId);
	if (session == nullptr)
	{
		LOG_ERROR(s_logCategory, "user seesion has deleted.");
		return;
	}

	C2S::C2SLoginRes res;
	int errorCode = 1;
	if (!success)
	{
		LOG_ERROR(s_logCategory, "find acccount db failed.");
		errorCode = 1;
	}

	res.set_error_code(errorCode);

	std::string strRes;
	if (!res.SerializeToString(&strRes))
	{
		LOG_ERROR(s_logCategory, "SerializeToString error!");
		return;
	}

	session->connection->sendData(C2S::MSG_ID_LOGIN_RES, strRes.data(), (uint32_t)strRes.size());
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

