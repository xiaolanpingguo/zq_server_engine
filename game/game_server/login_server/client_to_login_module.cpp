#include "game_server/login_server/client_to_login_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/login_server/login_server.h"
#include "game_common/game_db_def.hpp"
#include "game_common/bson_object.h"
#include "game_server/login_server/login_to_db_module.h"
#include "game_common/gid.h"


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

	LoginToDBModule* loginToDbModule = m_thisServer->getModule<LoginToDBModule>();
	loginToDbModule->registerFindResCb(CMD_LOGIN_2_DB_FIND_ACCOUNT_REQ, std::bind(&ClientToLoginModule::onS2SFindAccountRes, this, _1, _2, _3));
	loginToDbModule->registerInsertResCb(CMD_LOGIN_2_DB_INSERT_ACCOUNT_REQ, std::bind(&ClientToLoginModule::onS2SInsertAccountRes, this, _1, _2));

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
	std::string sdkUserId = msg.sdk_user_id();
	std::string token = msg.token();
	int channeId = msg.channel_id();

	if (sdkUserId.empty() || token.empty())
	{
		return;
	}

	// the user login has in progress
	auto session = findSession(sdkUserId);
	if (session)
	{
		return;
	}

	std::string sessionId = Gid::genGid();

	session = std::make_shared<Session>();
	session->createTime = time(nullptr);
	session->sdkAccountInfo.sdkUserId = sdkUserId;
	session->sdkAccountInfo.channelId = channeId;
	session->sdkAccountInfo.token = token;
	session->sessionId = sessionId;
	session->status = SessionStatus::WAITING_DB_RES;
	session->connection = connection;
	m_sessions[sdkUserId] = session;

	// find user from DB
	BsonObject selector;
	selector.appendString(ACCOUNT_KEY_SDK_USER_ID, sdkUserId);

	S2S::MongoUserData userData;
	userData.set_int32_var1(CMD_LOGIN_2_DB_FIND_ACCOUNT_REQ);
	userData.set_string_var1(sdkUserId);
	m_thisServer->getModule<LoginToDBModule>()->requestFind(DB_NAME, COL_ACCOUNT, selector, &userData);
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
		m_thisServer->getModule<LoginToDBModule>()->requestInsert(DB_NAME, COL_ACCOUNT, obj, &data);
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

