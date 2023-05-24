#include "game_server/zone_server/internal_server_module.h"
#include "game_common/message_router.hpp"
#include "game_server/zone_server/zone_server.h"


namespace zq{


InternalServerModule::InternalServerModule(ZoneServer* thisServer)
	:
		m_thisServer(thisServer)
{

}

InternalServerModule::~InternalServerModule()
{

}

bool InternalServerModule::init()
{
	using namespace std::placeholders;

	m_tcpServer = std::make_unique<TcpServer<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().internalIp, m_thisServer->getConfig().internalPort);
	m_tcpServer->setClientConnectedCb(std::bind(&InternalServerModule::onClientConnected, this, _1));
	m_tcpServer->setClientDisconnectedCb(std::bind(&InternalServerModule::onClientDisconnected, this, _1));
	m_tcpServer->setClientDataReceivedCb(std::bind(&InternalServerModule::onClientDataReceived, this, _1, _2, _3, _4));
	if (!m_tcpServer->start())
	{
		LOG_ERROR(s_logCategory, "tcp server start failed!");
		return false;
	}

	return true;
}

bool InternalServerModule::finalize()
{
	return true;
}

void InternalServerModule::onClientConnected(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a new client has connected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void InternalServerModule::onClientDisconnected(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a client has disconnected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void InternalServerModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, uint32_t len)
{

}



}

