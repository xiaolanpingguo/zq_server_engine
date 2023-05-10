#include "game_server/db_server/db_to_master_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/db_server/db_server.h"


namespace zq
{

DBToMasterModule::DBToMasterModule(DBServer* thisServer) :
		m_thisServer(thisServer),
		m_tcpClient(nullptr)
{
}

DBToMasterModule::~DBToMasterModule()
{
}

bool DBToMasterModule::init()
{
	using namespace std::placeholders;

	MessageHelper& messageHelper = MessageHelper::getInstance();
	messageHelper.registerHandler<&DBToMasterModule::onS2SServerRegisterRes>(this, S2S::MSG_ID_SERVER_REGSTER_RES);

	m_tcpClient = std::make_shared<TcpClient<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().masterServerIp, m_thisServer->getConfig().masterServerPort);
	m_tcpClient->setConnectToServerCb(std::bind(&DBToMasterModule::onConnectToServerCallback, this, _1, _2));
	m_tcpClient->setDisconnectFromServerCb(std::bind(&DBToMasterModule::onDisconnectedFromServer, this, _1));
	m_tcpClient->setDataReceivedCb(std::bind(&DBToMasterModule::onDataReceivedFromServer, this, _1, _2, _3, _4));
	m_tcpClient->asyncConnect();

	return true;
}

bool DBToMasterModule::finalize()
{
	return true;
}

void DBToMasterModule::onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg)
{
	if (connection == nullptr)
	{
		LOG_INFO(s_logCategory, "connect to server failed, msg:{}", errorMsg);
		return;
	}

	LOG_INFO(s_logCategory, "connect to master server success!");
	
	S2S::S2SServerRegisterReq s2sPackage;
	S2S::ServerInfo* serverInfo = s2sPackage.mutable_server_info();
	serverInfo->set_server_type(100);
	serverInfo->set_server_id(101);
	serverInfo->set_ip("1234");
	serverInfo->set_port(88);
	MessageHelper::getInstance().sendPacket(connection, S2S::MSG_ID_SERVER_REGSTER_REQ, s2sPackage);
}

void DBToMasterModule::onDisconnectedFromServer(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "disconnected from server: {}:{}", connection->getHost(), connection->getPort());
}

void DBToMasterModule::onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void fun11(TcpConnectionPtr connection, const S2S::S2SServerRegisterRes& res)
{
}

void DBToMasterModule::onS2SServerRegisterRes(TcpConnectionPtr connection, const S2S::S2SServerRegisterRes& res)
{
	LOG_INFO(s_logCategory, "onS2SServerRegisterRes, success:{}, error:{}", res.success(), res.error_msg());
}


}
