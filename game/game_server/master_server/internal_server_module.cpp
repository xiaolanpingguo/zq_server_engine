#include "game_server/master_server/internal_server_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/master_server/master_server.h"
#include "game_common/command_module.h"


namespace zq{



InternalServerModule::InternalServerModule(MasterServer* thisServer)
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
	messagehelper.registerHandler<&InternalServerModule::onS2SServerRegisterReq>(this, S2S::MSG_ID_SERVER_REGSTER_REQ);

	m_thisServer->getModule<CommandModule>()->registerCmd("stop_accept", std::bind(&InternalServerModule::cmdStopAccept, this, _1));

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
	LOG_INFO(s_logCategory, "a new server has connected: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void InternalServerModule::onClientDisconnected(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "a server has disconnected: {}:{}" , connection->getHost(), connection->getPort());
	removeServer(connection);
}

void InternalServerModule::onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void InternalServerModule::onS2SHeatBeatReq(TcpConnectionPtr connection, const S2S::S2SHeartBeat& msg)
{

}

const InternalServerModule::ServerInfo* InternalServerModule::findServer(const std::string& appId)
{
	auto it = m_servers.find(appId);
	if (it == m_servers.end())
	{
		return nullptr;
	}

	return &it->second;
}

const InternalServerModule::ServerInfo* InternalServerModule::findServer(TcpConnectionPtr connection)
{
	for (const auto& pair : m_servers)
	{
		if (pair.second.conncetion == connection)
		{
			return &pair.second;
		}
	}

	return nullptr;
}

bool InternalServerModule::removeServer(TcpConnectionPtr connection)
{
	bool found = false;
	for (auto& pair : m_servers)
	{
		if (pair.second.conncetion == connection)
		{
			found = true;
			m_servers.erase(pair.first);
			return true;
		}
	}

	if (!found)
	{
		// assert(0)
		LOG_ERROR(s_logCategory, "can't find any server by connection!");
	}

	return false;
}

void InternalServerModule::onS2SServerRegisterReq(TcpConnectionPtr connection, const S2S::S2SServerRegisterReq& msg)
{
	ServerInfo info;
	const S2S::ServerInfo& serverInfo = msg.server_info();
	info.appId = serverInfo.app_id();
	info.internalIp = serverInfo.internal_ip();
	info.internalPort = serverInfo.internal_port();
	info.conncetion = connection;

	S2S::S2SServerRegisterRes s2sPackage;
	const InternalServerModule::ServerInfo* server = findServer(info.appId);
	if (info.appId.empty() || info.internalIp.empty() || info.internalPort <= 0 || server != nullptr)
	{
		s2sPackage.set_success(false);
	}
	else
	{
		s2sPackage.set_success(true);
		LOG_INFO(s_logCategory, "a server has register, appId:{}, ip:{}, port:{}", info.appId, info.internalIp, info.internalPort);
		m_servers[info.appId] = info;
	}

	MessageHelper::getInstance().sendPacket(connection, S2S::MSG_ID_SERVER_REGSTER_RES, s2sPackage);
}

void InternalServerModule::cmdStopAccept(const std::vector<std::string>& args)
{
	if (args.empty())
	{
		return;
	}

	int v = std::stol(args[0]);
	if (v == 0)
	{
		m_tcpServer->stop();
	}
	else if (v == 1)
	{
		m_tcpServer->start();
	}
}



}

