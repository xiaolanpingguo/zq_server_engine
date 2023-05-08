#include "game_server/db_server/internal_server_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/db_server/db_server.h"


namespace zq{



InternalServerModule::InternalServerModule(DBServer* thisServer)
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

	MessageHelper& messagehelper = MessageHelper::getInstance();
	messagehelper.registerHandler<&InternalServerModule::onS2SHeatBeatReq>(this, S2S::MSG_ID_HEARTBEAT);

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

void InternalServerModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void InternalServerModule::onS2SHeatBeatReq(TcpConnectionPtr connection, const S2S::S2SHeartBeat& msg)
{

}


}

