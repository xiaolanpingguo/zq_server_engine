#include "game_server/world_server/internal_network_client_module.h"
#include "game_common/message_router.hpp"
#include "game_server/world_server/world_server.h"
#include <google/protobuf/util/json_util.h>


namespace zq
{

InternalNetworkClientModule::InternalNetworkClientModule(WorldServer* thisServer) :
		m_thisServer(thisServer),
		m_tcpClient(nullptr),
		m_connection(nullptr)
{
}

InternalNetworkClientModule::~InternalNetworkClientModule()
{
}

bool InternalNetworkClientModule::init()
{
	using namespace std::placeholders;

	S2SMsg::S2SServerRegisterReq s2sPackage;
	S2SMsg::ServerInfo* serverInfo = s2sPackage.mutable_server_info();
	serverInfo->set_server_type(100);
	serverInfo->set_server_id(101);
	serverInfo->set_ip("1234");
	serverInfo->set_port(88);
	std::string out;
	google::protobuf::util::MessageToJsonString(s2sPackage, &out);
	LOG_INFO(s_logCategory, "json:{}", out.c_str());


	TcpClientConfig config{ "127.0.0.1", 20001 };

	m_messageRouter = std::make_unique<MessageRouter<TcpConnection>>();
	m_messageRouter->registerHandler<&InternalNetworkClientModule::onS2SServerRegisterRes>(this, S2SMsg::S2S_ID_SERVER_REGSTER_RES);

	m_tcpClient = std::make_shared<TcpClient<TcpConnection>>(m_thisServer->getIoContext(), config.ip, config.port);
	m_tcpClient->setConnectToServerCb(std::bind(&InternalNetworkClientModule::onConnectToServerCallback, this, _1, _2));
	m_tcpClient->setDisconnectFromServerCb(std::bind(&InternalNetworkClientModule::onDisconnectedFromServer, this, _1));
	m_tcpClient->setDataReceivedCb(std::bind(&InternalNetworkClientModule::onDataReceivedFromServer, this, _1, _2, _3, _4));
	m_tcpClient->asyncConnect();

	return true;
}

bool InternalNetworkClientModule::finalize()
{
	return true;
}

void InternalNetworkClientModule::onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg)
{
	if (connection == nullptr)
	{
		LOG_INFO(s_logCategory, "connect to server failed, msg:{}", errorMsg);
		return;
	}

	m_connection = connection;

	
	S2SMsg::S2SServerRegisterReq s2sPackage;
	S2SMsg::ServerInfo* serverInfo = s2sPackage.mutable_server_info();
	serverInfo->set_server_type(100);
	serverInfo->set_server_id(101);
	serverInfo->set_ip("1234");
	serverInfo->set_port(88);

	//MessageRouter<TcpConnection>::sendPacket(m_connection, S2SMsg::S2S_ID_SERVER_REGSTER_REQ, s2sPackage);
}

void InternalNetworkClientModule::onDisconnectedFromServer(TcpConnectionPtr connection)
{
	LOG_INFO(s_logCategory, "disconnected from server: {}-{}:{}", connection->getConnectionId(), connection->getIp(), connection->getPort());
}

void InternalNetworkClientModule::onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len)
{
	LOG_INFO(s_logCategory, "onClientDataReceived, msgid:{}, len:{}", msgId, len);
	m_messageRouter->dispatch(connection, msgId, (const char*)data, (uint32_t)len);
}

void InternalNetworkClientModule::onS2SServerRegisterRes(TcpConnectionPtr connection, const S2SMsg::S2SServerRegisterRes& msg)
{
	LOG_INFO(s_logCategory, "onS2SServerRegisterRes, success:{}, error:{}", msg.success(), msg.error_msg());
}


}
