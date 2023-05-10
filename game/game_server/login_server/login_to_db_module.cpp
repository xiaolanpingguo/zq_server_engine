#include "game_server/login_server/login_to_db_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/login_server/login_server.h"
#include "protocol/s2s/s2s_common.pb.h"
#include "game_common/bson_object.h"
#include "game_common/game_db_def.hpp"


namespace zq{



LoginToDBModule::LoginToDBModule(LoginServer* thisServer)
	:
		m_thisServer(thisServer)
{

}

LoginToDBModule::~LoginToDBModule()
{

}

bool LoginToDBModule::init()
{
	using namespace std::placeholders;

	MessageHelper& messagehelper = MessageHelper::getInstance();
	messagehelper.registerHandler<&LoginToDBModule::onS2SMongoInsertRes>(this, S2S::MSG_ID_DB_INSERT_RES);
	messagehelper.registerHandler<&LoginToDBModule::onS2SMongoRemoveRes>(this, S2S::MSG_ID_DB_REMOVE_RES);
	messagehelper.registerHandler<&LoginToDBModule::onS2SMongoSaveRes>(this, S2S::MSG_ID_DB_SAVE_RES);
	messagehelper.registerHandler<&LoginToDBModule::onS2SMongoFindRes>(this, S2S::MSG_ID_DB_FIND_RES);
	messagehelper.registerHandler<&LoginToDBModule::onS2SMongoBatchFindRes>(this, S2S::MSG_ID_DB_BATCH_FIND_RES);

	m_tcpClient = std::make_shared<TcpClient<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().dbServerIp, m_thisServer->getConfig().dbServerPort);
	m_tcpClient->setConnectToServerCb(std::bind(&LoginToDBModule::onConnectToServerCallback, this, _1, _2));
	m_tcpClient->setDisconnectFromServerCb(std::bind(&LoginToDBModule::onDisconnectedFromServer, this, _1));
	m_tcpClient->setDataReceivedCb(std::bind(&LoginToDBModule::onDataReceivedFromServer, this, _1, _2, _3, _4));
	m_tcpClient->asyncConnect();

	return true;
}

bool LoginToDBModule::finalize()
{
	return true;
}

void LoginToDBModule::onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg)
{
	if (connection == nullptr)
	{
		LOG_INFO(s_logCategory, "connect to server failed, msg:{}", errorMsg);
		return;
	}

	LOG_INFO(s_logCategory, "connect to db server success!");
	m_connection = connection;
}

void LoginToDBModule::onDisconnectedFromServer(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "disconnected from server: {}:{}", connection->getHost(), connection->getPort());
	m_connection = nullptr;
}

void LoginToDBModule::onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void LoginToDBModule::onS2SMongoInsertRes(TcpConnectionPtr connection, const S2S::MongoInsertRes& res)
{
	if (!res.has_user_data())
	{
		return;
	}

	const S2S::MongoUserData& userData = res.user_data();
	int cmd = userData.cmd();
	auto it = m_insertResCallbacks.find(cmd);
	if (it != m_insertResCallbacks.end())
	{
		it->second(res.success(), userData);
	}
}

void LoginToDBModule::onS2SMongoRemoveRes(TcpConnectionPtr connection, const S2S::MongoRemoveRes& res)
{
	if (!res.has_user_data())
	{
		return;
	}

	const S2S::MongoUserData& userData = res.user_data();
	int cmd = userData.cmd();
	auto it = m_removeResCallbacks.find(cmd);
	if (it != m_removeResCallbacks.end())
	{
		it->second(res.success(), userData);
	}
}

void LoginToDBModule::onS2SMongoSaveRes(TcpConnectionPtr connection, const S2S::MongoSaveRes& res)
{
	if (!res.has_user_data())
	{
		return;
	}

	const S2S::MongoUserData& userData = res.user_data();
	int cmd = userData.cmd();
	auto it = m_saveResCallbacks.find(cmd);
	if (it != m_saveResCallbacks.end())
	{
		it->second(res.success(), userData);
	}
}

void LoginToDBModule::onS2SMongoFindRes(TcpConnectionPtr connection, const S2S::MongoFindRes& res)
{
	if (!res.has_user_data())
	{
		return;
	}

	std::vector<BsonObject> result;
	for (int i = 0; i < res.data_list_size(); ++i)
	{
		BsonObject obj;
		const S2S::ProtoBsonObj& proto = res.data_list(i);
		obj.convertFromProtoBson(proto);
		result.emplace_back(obj);
	}

	const S2S::MongoUserData& userData = res.user_data();
	int cmd = userData.cmd();
	auto it = m_findResCallbacks.find(cmd);
	if (it != m_findResCallbacks.end())
	{
		it->second(res.success(), userData, result);
	}
}

void LoginToDBModule::onS2SMongoBatchFindRes(TcpConnectionPtr connection, const S2S::MongoBatchFindRes& res)
{
}

bool LoginToDBModule::sendToDBServer(uint16_t msgId, const char* data, uint32_t len)
{
	m_connection->sendData(msgId, data, len);
	return true;
}


}

