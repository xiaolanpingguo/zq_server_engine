#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_client.hpp"
#include "protocol/s2s/s2s_common.pb.h"

namespace zq
{

struct TcpClientConfig
{
	std::string ip;
	uint16_t port = 0;
};

template <typename T>
class MessageRouter;

class WorldServer;
class InternalNetworkClientModule : public IModule
{
	INIT_MODULE_NAME(InternalNetworkClientModule);

public:
	InternalNetworkClientModule(WorldServer* thisServer);
	~InternalNetworkClientModule();

public:
	bool init() override;
	bool finalize() override;

private:
	void onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg);
	void onDisconnectedFromServer(TcpConnectionPtr connection);
	void onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);

private:
	void onS2SServerRegisterRes(TcpConnectionPtr connection, const S2SMsg::S2SServerRegisterRes& msg);

private:
	WorldServer* m_thisServer;
	std::shared_ptr <TcpClient<TcpConnection>> m_tcpClient;
	std::shared_ptr<TcpConnection> m_connection;
	std::unique_ptr<MessageRouter<TcpConnection>> m_messageRouter;

	constexpr static std::string_view s_logCategory = "NetworkServerModule";
};

} //namespace zq
