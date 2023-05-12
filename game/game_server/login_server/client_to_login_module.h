#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_server.hpp"
#include "game_common/bson_object.h"
#include "protocol/c2s/c2s_common.pb.h"
#include "common/coroutine_awaitor.hpp"


namespace zq {


enum class SessionStatus
{
	NONE,
	WAITING_DB_RES,
	READY
};

struct SDKAccountInfo
{
	std::string sdkUserId;
	int channelId;
	std::string token;
};

struct Session
{
	SDKAccountInfo sdkAccountInfo;
	std::string sessionId;
	uint64_t createTime = 0;
	SessionStatus status = SessionStatus::NONE;
	TcpConnectionPtr connection;
};

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

private:
	void onC2SHeatBeatReq(TcpConnectionPtr connection, const C2S::C2SHeartBeat& msg);
	void onC2SClientLoginReq(TcpConnectionPtr connection, const C2S::C2SLoginReq& msg);
	async_simple::coro::Lazy<void> processLogin(TcpConnectionPtr connection, const C2S::C2SLoginReq& msg);

private:
	void onS2SFindAccountRes(bool success, const S2S::MongoUserData& userData, const std::vector<BsonObject>& result);
	void onS2SInsertAccountRes(bool success, const S2S::MongoUserData& userData);

private:
	std::shared_ptr<Session> findSession(const std::string& sdkUserId);

private:

	LoginServer* m_thisServer;

	std::unique_ptr<TcpServer<TcpConnection>> m_tcpServer;

	std::unordered_map<std::string, std::shared_ptr<Session>> m_sessions;

	constexpr static std::string_view s_logCategory = "ClientToLoginModule";
};

}
