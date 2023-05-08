#include "game_server/zone_server/client_to_zone_server_module.h"
#include "game_common/message_router.hpp"
#include "game_server/zone_server/zone_server.h"
#include "protocol/c2s/c2s_common.pb.h"


namespace zq{


ClientToServerModule::ClientToServerModule(ZoneServer* thisServer)
	:
		m_thisServer(thisServer)
{

}

ClientToServerModule::~ClientToServerModule()
{

}

bool ClientToServerModule::init()
{
	using namespace std::placeholders;

	m_tcpServer = std::make_unique<TcpServer<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().externalIp, m_thisServer->getConfig().externalPort);
	m_tcpServer->setClientConnectedCb(std::bind(&ClientToServerModule::onClientConnected, this, _1));
	m_tcpServer->setClientDisconnectedCb(std::bind(&ClientToServerModule::onClientDisconnected, this, _1));
	m_tcpServer->setClientDataReceivedCb(std::bind(&ClientToServerModule::onClientDataReceived, this, _1, _2, _3, _4));
	if (!m_tcpServer->start())
	{
		LOG_ERROR(s_logCategory, "tcp server start failed!");
		return false;
	}

	return true;
}

bool ClientToServerModule::finalize()
{
	return true;
}

void ClientToServerModule::onClientConnected(TcpConnectionPtr connection)
{
	LOG_DEBUG(s_logCategory, "a new client has connected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void ClientToServerModule::onClientDisconnected(TcpConnectionPtr connection)
{
	LOG_DEBUG(s_logCategory, "a client has disconnected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void ClientToServerModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, uint32_t len)
{

}



}

