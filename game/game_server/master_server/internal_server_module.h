#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_server.hpp"
#include "protocol/s2s/s2s_common.pb.h"


namespace zq {


class MasterServer;
class InternalServerModule : public IModule
{
	INIT_MODULE_NAME(InternalServerModule);

	struct ServerInfo
	{
		TcpConnectionPtr conncetion;
		std::string appId;
		std::string internalIp;
		uint16_t internalPort;
	};
public:
	InternalServerModule(MasterServer* thisServer);
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
	void onS2SServerRegisterReq(TcpConnectionPtr connection, const S2S::S2SServerRegisterReq& msg);

	const InternalServerModule::ServerInfo* findServer(const std::string& appId);
	const InternalServerModule::ServerInfo* findServer(TcpConnectionPtr connection);
	bool removeServer(TcpConnectionPtr connection);

	void cmdStopAccept(const std::vector<std::string>& args);

private:

	MasterServer* m_thisServer;
	std::unique_ptr<TcpServer<TcpConnection>> m_tcpServer;

	std::unordered_map<std::string, ServerInfo> m_servers;

	constexpr static std::string_view s_logCategory = "NetworkServerModule";
};

}
