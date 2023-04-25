#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_server.hpp"
#include "protocol/s2s/s2s_common.pb.h"


namespace zq {


struct TcpServerConfig
{
	std::string ip;
	uint16_t port = 0;
};

template <typename T>
class MessageRouter;

class WorldServer;
class InternalNetworkServerModule : public IModule
{
public:
	InternalNetworkServerModule(WorldServer* thisServer);
	~InternalNetworkServerModule();

public:
	bool init() override;
	bool finalize() override;

private:

	void onClientConnected(TcpConnectionPtr connection);
	void onClientDisconnected(TcpConnectionPtr connection);
	void onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, uint32_t len);

private:

	WorldServer* m_thisServer;
	std::unique_ptr<TcpServer<TcpConnection>> m_tcpServer;
	MessageRouter<TcpConnection>* m_messageRouter;

	constexpr static std::string_view s_logCategory = "NetworkServerModule";
};

}
