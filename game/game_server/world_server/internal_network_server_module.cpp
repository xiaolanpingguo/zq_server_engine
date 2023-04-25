#include "game_server/world_server/internal_network_server_module.h"
#include "game_common/message_router.hpp"
#include "game_server/world_server/world_server.h"


namespace zq{


InternalNetworkServerModule::InternalNetworkServerModule(WorldServer* thisServer)
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

	TcpServerConfig config{ "127.0.0.1", 20001 };
	m_tcpServer = std::make_unique<TcpServer<TcpConnection>>(m_thisServer->getIoContext(), config.ip, config.port);
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
	LOG_INFO(s_logCategory, "a new client has connected: {}-{}:{}", connection->getConnectionId(), connection->getIp(), connection->getPort());
}

void InternalNetworkServerModule::onClientDisconnected(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a client has disconnected: {}-{}:{}", connection->getConnectionId(), connection->getIp(), connection->getPort());
}

void InternalNetworkServerModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, uint32_t len)
{

}



}

