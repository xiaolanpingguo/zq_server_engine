#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_client.hpp"
#include "protocol/s2s/s2s_common.pb.h"

namespace zq
{


class DBServer;
class DBToMasterModule : public IModule
{
	INIT_MODULE_NAME(DBToMasterModule);

public:
	DBToMasterModule(DBServer* thisServer);
	~DBToMasterModule();

public:
	bool init() override;
	bool finalize() override;

private:
	void onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg);
	void onDisconnectedFromServer(TcpConnectionPtr connection);
	void onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);

private:
	void onS2SServerRegisterRes(TcpConnectionPtr connection, const S2S::S2SServerRegisterRes& msg);


private:
	DBServer* m_thisServer;
	std::shared_ptr<TcpClient<TcpConnection>> m_tcpClient;

	constexpr static std::string_view s_logCategory = "DBToMasterModule";
};

} //namespace zq
