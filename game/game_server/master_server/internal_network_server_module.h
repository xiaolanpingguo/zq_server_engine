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

class MasterServer;
class InternalNetworkServerModule : public IModule
{
	INIT_MODULE_NAME(InternalNetworkServerModule);

public:
	InternalNetworkServerModule(MasterServer* thisServer);
	~InternalNetworkServerModule();

public:
	bool init() override;
	bool finalize() override;

private:

	void onClientConnected(TcpConnectionPtr connection);
	void onClientDisconnected(TcpConnectionPtr connection);
	void onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);

private:
	void onS2SHeatBeatReq(TcpConnectionPtr connection, const S2SMsg::S2SHeartBeat& msg);
	void onS2SServerRegisterReq(TcpConnectionPtr connection, const S2SMsg::S2SServerRegisterReq& msg);

private:

	MasterServer* m_thisServer;

	std::unique_ptr<TcpServer<TcpConnection>> m_tcpServer;
	std::unique_ptr<MessageRouter<TcpConnection>> m_messageRouter;

	constexpr static std::string_view s_logCategory = "NetworkServerModule";
};

}
