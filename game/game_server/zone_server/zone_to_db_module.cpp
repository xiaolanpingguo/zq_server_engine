#include "game_server/zone_server/zone_to_db_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/zone_server/zone_server.h"


namespace zq
{

WorldToDBModule::WorldToDBModule(ZoneServer* thisServer) :
		m_thisServer(thisServer),
		m_tcpClient(nullptr),
		m_connection(nullptr)
{
}

WorldToDBModule::~WorldToDBModule()
{
}

bool WorldToDBModule::init()
{
	using namespace std::placeholders;

	//m_coConnection = std::make_shared<CoroutineConnection>(m_thisServer->getIoContext(), m_thisServer->getConfig().masterServerIp, m_thisServer->getConfig().masterServerPort, nullptr, nullptr);
	//m_coConnection->asyncConnect();

	//MessageHelper& messageHelper = MessageHelper::getInstance();
	//messageHelper.registerHandler<&WorldToDBModule::onS2SServerRegisterRes>(this, S2SMsg::S2S_ID_SERVER_REGSTER_RES);

	m_tcpClient = std::make_shared<TcpClient<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().dbServerIp, m_thisServer->getConfig().dbServerPort);
	m_tcpClient->setConnectToServerCb(std::bind(&WorldToDBModule::onConnectToServerCallback, this, _1, _2));
	m_tcpClient->setDisconnectFromServerCb(std::bind(&WorldToDBModule::onDisconnectedFromServer, this, _1));
	m_tcpClient->setDataReceivedCb(std::bind(&WorldToDBModule::onDataReceivedFromServer, this, _1, _2, _3, _4));
	m_tcpClient->asyncConnect();

	return true;
}

bool WorldToDBModule::finalize()
{
	return true;
}

void WorldToDBModule::onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg)
{
	if (connection == nullptr)
	{
		LOG_INFO(s_logCategory, "connect to server failed, msg:{}", errorMsg);
		return;
	}
	
	LOG_INFO(s_logCategory, "connect to db server success!");
	m_connection = connection;
}

void WorldToDBModule::onDisconnectedFromServer(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "disconnected from server: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void WorldToDBModule::onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void WorldToDBModule::sendToDBServer(uint16_t msgId, const char* data, uint32_t len)
{
	if (m_connection == nullptr || !m_connection->isOpen())
	{	
		return;
	}

	m_connection->sendData(msgId, data, len);
}


}
