#include "game_server/world_server/world_to_master_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/world_server/world_server.h"
#include "network/coroutine/coroutine_connection.hpp"


namespace zq
{

WorldToMasterModule::WorldToMasterModule(WorldServer* thisServer) :
		m_thisServer(thisServer),
		m_tcpClient(nullptr)
{
}

WorldToMasterModule::~WorldToMasterModule()
{
}

bool WorldToMasterModule::init()
{
	using namespace std::placeholders;

	m_coConnection = std::make_shared<CoroutineConnection>(m_thisServer->getIoContext(), m_thisServer->getConfig().masterServerIp, m_thisServer->getConfig().masterServerPort, nullptr, nullptr);
	m_coConnection->asyncConnect();


	MessageHelper& messageHelper = MessageHelper::getInstance();
	messageHelper.registerHandler<&WorldToMasterModule::onS2SServerRegisterRes>(this, S2SMsg::S2S_ID_SERVER_REGSTER_RES);

	m_tcpClient = std::make_shared<TcpClient<TcpConnection>>(m_thisServer->getIoContext(), m_thisServer->getConfig().masterServerIp, m_thisServer->getConfig().masterServerPort);
	m_tcpClient->setConnectToServerCb(std::bind(&WorldToMasterModule::onConnectToServerCallback, this, _1, _2));
	m_tcpClient->setDisconnectFromServerCb(std::bind(&WorldToMasterModule::onDisconnectedFromServer, this, _1));
	m_tcpClient->setDataReceivedCb(std::bind(&WorldToMasterModule::onDataReceivedFromServer, this, _1, _2, _3, _4));
	m_tcpClient->asyncConnect();

	return true;
}

bool WorldToMasterModule::finalize()
{
	return true;
}

void WorldToMasterModule::onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg)
{
	if (connection == nullptr)
	{
		LOG_INFO(s_logCategory, "connect to server failed, msg:{}", errorMsg);
		return;
	}
	
	S2SMsg::S2SServerRegisterReq s2sPackage;
	S2SMsg::ServerInfo* serverInfo = s2sPackage.mutable_server_info();
	serverInfo->set_server_type(100);
	serverInfo->set_server_id(101);
	serverInfo->set_ip("1234");
	serverInfo->set_port(88);
	MessageHelper::getInstance().sendPacket(connection, S2SMsg::S2S_ID_SERVER_REGSTER_REQ, s2sPackage);
}

void WorldToMasterModule::onDisconnectedFromServer(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "disconnected from server: {}-{}:{}", connection->getConnectionId(), connection->getHost(), connection->getPort());
}

void WorldToMasterModule::onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	MessageHelper::getInstance().dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

async_simple::coro::Lazy<bool> WorldToMasterModule::testFun1()
{
	S2SMsg::S2SServerRegisterReq s2sPackage;
	S2SMsg::ServerInfo* serverInfo = s2sPackage.mutable_server_info();
	serverInfo->set_server_type(3255);
	serverInfo->set_server_id(2131);
	serverInfo->set_ip("1234111");
	serverInfo->set_port(7777);

	std::string strData;
	if (!s2sPackage.SerializeToString(&strData))
	{
	}

	S2SMsg::S2SServerRegisterRes res;
	bool b = co_await m_coConnection->send(S2SMsg::S2S_ID_SERVER_REGSTER_REQ, strData.data(), (uint32_t)strData.size(), res);
	LOG_INFO(s_logCategory, "testFun1, success:{}, error:{}", res.success(), res.error_msg());
	co_return b;
}


void WorldToMasterModule::onS2SServerRegisterRes(TcpConnectionPtr connection, const S2SMsg::S2SServerRegisterRes& res)
{
	LOG_INFO(s_logCategory, "onS2SServerRegisterRes, success:{}, error:{}", res.success(), res.error_msg());
	testFun1().start([](async_simple::Try<bool> result) {
		if (result.hasError())
		{
			try
			{
				std::rethrow_exception(result.getException());
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}

		} }
            );
}


}
