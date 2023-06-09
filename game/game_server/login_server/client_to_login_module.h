#pragma once


#include "game_common/i_module.hpp"
#include "game_common/bson_object.h"
#include "network/tcp_server.hpp"
#include "common/coroutine_awaitor.hpp"
#include "protocol/c2s/c2s_login.pb.h"
#include "protocol/s2s/s2s_global_data.pb.h"


namespace zq {


class LoginServer;
class ClientToLoginModule : public IModule
{
	INIT_MODULE_NAME(ClientToLoginModule);

public:
	ClientToLoginModule(LoginServer* thisServer);
	~ClientToLoginModule();

public:
	bool init() override;
	bool finalize() override;

private:

	void onClientConnected(TcpConnectionPtr connection);
	void onClientDisconnected(TcpConnectionPtr connection);
	void onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);
	void onServerRegisterSuccess();

private:
	void onC2SHeatBeatReq(TcpConnectionPtr connection, const C2S::C2SHeartBeat& msg);
	void onC2SClientLoginReq(TcpConnectionPtr connection, const C2S::C2SLoginReq& msg);
	async_simple::coro::Lazy<void> processLogin(TcpConnectionPtr connection, const C2S::C2SLoginReq& msg);
	async_simple::coro::Lazy<int> findUserSession(const std::string& profileId, S2S::S2SPlayerSessionData& sessionData);
	async_simple::coro::Lazy<int> createSeesion(const S2S::S2SPlayerSessionData& session, bool overwrite);
	async_simple::coro::Lazy<int> findAndSaveUser(const std::string& sdkUserId, const std::string& sdkToken, int channelId, std::string& profileId);
	async_simple::coro::Lazy<int> getSuitableZoneServer(S2S::S2SZoneServerData& serverData);
	async_simple::coro::Lazy<int> isZoneServerAailable(const std::string& appId);
	async_simple::coro::Lazy<int> deleteSession(const std::string& profileId);

private:

	LoginServer* m_thisServer;

	std::unique_ptr<TcpServer<TcpConnection>> m_tcpServer;

	constexpr static std::string_view s_logCategory = "ClientToLoginModule";
};

}
