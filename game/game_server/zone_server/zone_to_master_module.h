#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_client.hpp"
#include "network/coroutine/coroutine_connection.hpp"
#include "protocol/s2s/s2s_common.pb.h"

namespace zq
{


class ZoneServer;
class ZoneToMasterModule : public IModule
{
	INIT_MODULE_NAME(ZoneToMasterModule);

public:
	ZoneToMasterModule(ZoneServer* thisServer);
	~ZoneToMasterModule();

public:
	bool init() override;
	bool finalize() override;

private:
	void onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg);
	void onDisconnectedFromServer(TcpConnectionPtr connection);
	void onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);

private:
	void onS2SServerRegisterRes(TcpConnectionPtr connection, const S2S::S2SServerRegisterRes& msg);

	async_simple::coro::Lazy<bool> testFun1();

private:
	ZoneServer* m_thisServer;
	std::shared_ptr<TcpClient<TcpConnection>> m_tcpClient;
	CoroutineConnectionPtr m_coConnection;

	constexpr static std::string_view s_logCategory = "WorldToMasterModule";
};

} //namespace zq
