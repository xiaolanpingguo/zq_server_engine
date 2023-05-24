#include "game_server/zone_server/zone_to_master_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/zone_server/zone_server.h"
#include "network/coroutine/coroutine_connection.hpp"


namespace zq
{

ZoneToMasterModule::ZoneToMasterModule(ZoneServer* thisServer) :
		m_thisServer(thisServer),
		m_tcpClient(nullptr),
		m_masterConnection(nullptr)
{
}

ZoneToMasterModule::~ZoneToMasterModule()
{
}

bool ZoneToMasterModule::init()
{
	using namespace std::placeholders;

	MessageHelper& messageHelper = MessageHelper::getInstance();
	messageHelper.registerHandler<&ZoneToMasterModule::onS2SServerRegisterRes>(this, S2S::MSG_ID_SERVER_REGSTER_RES);

	m_tcpClient = std::make_shared<TcpClient<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().masterServerIp, m_thisServer->getConfig().masterServerPort);
	m_tcpClient->setConnectToServerCb(std::bind(&ZoneToMasterModule::onConnectToServerCallback, this, _1, _2));
	m_tcpClient->setDisconnectFromServerCb(std::bind(&ZoneToMasterModule::onDisconnectedFromServer, this, _1));
	m_tcpClient->setDataReceivedCb(std::bind(&ZoneToMasterModule::onDataReceivedFromServer, this, _1, _2, _3, _4));
	m_tcpClient->asyncConnect();

	return true;
}

bool ZoneToMasterModule::finalize()
{
	if (m_masterConnection)
	{
		m_masterConnection->close();
		m_masterConnection = nullptr;
	}

	return true;
}

void ZoneToMasterModule::onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg)
{
	if (connection == nullptr)
	{
		LOG_INFO(s_logCategory, "connect to server failed, msg:{}", errorMsg);
		return;
	}

	LOG_INFO(s_logCategory, "connect to master server success!");
	
	if (m_masterConnection)
	{
		m_masterConnection->close();
		m_masterConnection = nullptr;
	}
	m_masterConnection = connection;

	S2S::S2SServerRegisterReq s2sPackage;
	S2S::ServerInfo* serverInfo = s2sPackage.mutable_server_info();
	serverInfo->set_app_id(m_thisServer->getAppIdStr());
	serverInfo->set_internal_ip(m_thisServer->getConfig().internalIp);
	serverInfo->set_internal_port(m_thisServer->getConfig().internalPort);
	MessageHelper::getInstance().sendPacket(m_masterConnection, S2S::MSG_ID_SERVER_REGSTER_REQ, s2sPackage);
}

void ZoneToMasterModule::onDisconnectedFromServer(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "disconnected from server: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void ZoneToMasterModule::onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void ZoneToMasterModule::onS2SServerRegisterRes(TcpConnectionPtr connection, const S2S::S2SServerRegisterRes& res)
{
	if (res.success() && m_registerSuccessCb)
	{
		LOG_INFO(s_logCategory, "register to master success!");
		m_registerSuccessCb();
	}
	else
	{
		LOG_ERROR(s_logCategory, "register to master failed, please check your appid!");
	}
}


}
