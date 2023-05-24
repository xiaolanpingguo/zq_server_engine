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

	void registerSuccessCallback(const std::function<void()>& cb) { m_registerSuccessCb = cb; }

private:
	void onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg);
	void onDisconnectedFromServer(TcpConnectionPtr connection);
	void onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);

private:
	void onS2SServerRegisterRes(TcpConnectionPtr connection, const S2S::S2SServerRegisterRes& msg);

private:
	ZoneServer* m_thisServer;
	std::shared_ptr<TcpClient<TcpConnection>> m_tcpClient;
	TcpConnectionPtr m_masterConnection;
	CoroutineConnectionPtr m_coConnection;
	std::function<void()> m_registerSuccessCb;

	constexpr static std::string_view s_logCategory = "ZoneToMasterModule";
};

} //namespace zq
