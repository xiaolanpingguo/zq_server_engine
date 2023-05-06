#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_server.hpp"
#include "protocol/s2s/s2s_common.pb.h"


namespace zq {


class WorldServer;
class InternalServerModule : public IModule
{
	INIT_MODULE_NAME(InternalServerModule);

public:
	InternalServerModule(WorldServer* thisServer);
	~InternalServerModule();

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

	constexpr static std::string_view s_logCategory = "NetworkServerModule";
};

}
