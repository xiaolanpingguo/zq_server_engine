#include "game_server/world_server/network_server_module.h"


namespace zq{


NetworkServerModule::NetworkServerModule(asio::io_context& ioContext)
	:
		m_ioContext(ioContext)
{

}

NetworkServerModule::~NetworkServerModule()
{

}

bool NetworkServerModule::init(const TcpServerConfig& config)
{
	using namespace std::placeholders;
	m_tcpServer = std::make_unique<TcpServer>(m_ioContext, config.ip, config.port, ConnectionType::TcpConnection);
	m_tcpServer->setClientConnectedCb(std::bind(&NetworkServerModule::onClientConnected, this, _1));
	m_tcpServer->setClientDisconnectedCb(std::bind(&NetworkServerModule::onClientDisconnected, this, _1));
	m_tcpServer->setClientDataReceivedCb(std::bind(&NetworkServerModule::onClientDataReceived, this, _1, _2, _3));
	if (!m_tcpServer->start())
	{
		LOG_ERROR(s_logCategory, "tcp server start failed!");
		return false;
	}

	return true;
}

void NetworkServerModule::run()
{

}

void NetworkServerModule::stop()
{

}

void NetworkServerModule::onClientConnected(ConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a new client has connected: {}-{}:{}", connection->getConnectionId(), connection->getIp(), connection->getPort());
}

void NetworkServerModule::onClientDisconnected(ConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a client has disconnected: {}-{}:{}", connection->getConnectionId(), connection->getIp(), connection->getPort());
}

void NetworkServerModule::onClientDataReceived(ConnectionPtr connection, const void* data, size_t len)
{
	std::string str((const char*)data);
	LOG_INFO(s_logCategory , "onClientDataReceived, data:{}", str);
}



}

