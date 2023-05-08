#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_server.hpp"
#include "protocol/s2s/s2s_common.pb.h"
#include "protocol/s2s/db_mongo_proxy.pb.h"


namespace zq {


class DBServer;
class InternalServerModule : public IModule
{
	INIT_MODULE_NAME(InternalServerModule);

public:
	InternalServerModule(DBServer* thisServer);
	~InternalServerModule();

public:
	bool init() override;
	bool finalize() override;

private:

	void onClientConnected(TcpConnectionPtr connection);
	void onClientDisconnected(TcpConnectionPtr connection);
	void onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);

private:
	void onS2SHeatBeatReq(TcpConnectionPtr connection, const S2S::S2SHeartBeat& msg);

private:

	DBServer* m_thisServer;

	std::unique_ptr<TcpServer<TcpConnection>> m_tcpServer;

	constexpr static std::string_view s_logCategory = "InternalServerModule";
};

}
