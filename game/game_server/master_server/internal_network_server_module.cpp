#include "game_server/master_server/internal_network_server_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/master_server/master_server.h"


namespace zq{



InternalNetworkServerModule::InternalNetworkServerModule(MasterServer* thisServer)
	:
		m_thisServer(thisServer)
{

}

InternalNetworkServerModule::~InternalNetworkServerModule()
{

}

bool InternalNetworkServerModule::init()
{
	using namespace std::placeholders;

	MessageHelper& messagehelper = MessageHelper::getInstance();
	messagehelper.registerHandler<&InternalNetworkServerModule::onS2SHeatBeatReq>(this, S2SMsg::S2S_ID_HEARTBEAT);
	messagehelper.registerHandler<&InternalNetworkServerModule::onS2SServerRegisterReq>(this, S2SMsg::S2S_ID_SERVER_REGSTER_REQ);

	m_tcpServer = std::make_unique<TcpServer<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().internalIp, m_thisServer->getConfig().internalPort);
	m_tcpServer->setClientConnectedCb(std::bind(&InternalNetworkServerModule::onClientConnected, this, _1));
	m_tcpServer->setClientDisconnectedCb(std::bind(&InternalNetworkServerModule::onClientDisconnected, this, _1));
	m_tcpServer->setClientDataReceivedCb(std::bind(&InternalNetworkServerModule::onClientDataReceived, this, _1, _2, _3, _4));
	if (!m_tcpServer->start())
	{
		LOG_ERROR(s_logCategory, "tcp server start failed!");
		return false;
	}

	return true;
}

bool InternalNetworkServerModule::finalize()
{
	return true;
}

void InternalNetworkServerModule::onClientConnected(TcpConnectionPtr connection)
{	
	LOG_INFO(s_logCategory, "a new client has connected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void InternalNetworkServerModule::onClientDisconnected(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a client has disconnected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void InternalNetworkServerModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void InternalNetworkServerModule::onS2SHeatBeatReq(TcpConnectionPtr connection, const S2SMsg::S2SHeartBeat& msg)
{

}

void InternalNetworkServerModule::onS2SServerRegisterReq(TcpConnectionPtr connection, const S2SMsg::S2SServerRegisterReq& msg)
{
	const S2SMsg::ServerInfo& serverInfo = msg.server_info();
	int serverType = serverInfo.server_type();
	int serverId = serverInfo.server_id();
	std::string ip = serverInfo.ip();
	int port = serverInfo.port();

	LOG_INFO(s_logCategory, "a server has register, type:{}, id:{}, ip:{}, port:{}", serverType, serverId, ip, port);

	S2SMsg::S2SServerRegisterRes s2sPackage;
	s2sPackage.set_success(true);
	s2sPackage.set_error_msg("dwadawd");
	MessageHelper::getInstance().sendPacket(connection, S2SMsg::S2S_ID_SERVER_REGSTER_RES, s2sPackage);
}



}

