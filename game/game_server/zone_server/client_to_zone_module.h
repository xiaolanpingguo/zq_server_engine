#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_server.hpp"
#include "common/coroutine_awaitor.hpp"
#include "protocol/c2s/c2s_login.pb.h"
#include "protocol/s2s/s2s_global_data.pb.h"
#include "game_common/bson_object.h"


namespace zq {


class ZoneServer;
class Player;
class ClientToZoneModule : public IModule
{
	INIT_MODULE_NAME(ClientToZoneModule);

public:
	ClientToZoneModule(ZoneServer* thisServer);
	~ClientToZoneModule();

public:
	bool init() override;
	bool finalize() override;

private:

	void onClientConnected(TcpConnectionPtr connection);
	void onClientDisconnected(TcpConnectionPtr connection);
	void onClientDataReceived(TcpConnectionPtr connection, uint16_t msgId, const void* data, uint32_t len);
	void onServerRegisterSuccess();

private:
	void onC2SHeatBeatReq(TcpConnectionPtr connection, const C2S::C2SHeartBeat& msg);
	void onC2SClientLoginReq(TcpConnectionPtr connection, const C2S::C2SLoginZoneReq& msg);

private:
	async_simple::coro::Lazy<void> processLogin(TcpConnectionPtr connection, const C2S::C2SLoginZoneReq& msg);
	async_simple::coro::Lazy<int> findUserSession(const std::string& profileId, S2S::S2SPlayerSessionData& sessionData);
	async_simple::coro::Lazy<int> getPlayerDBData(const std::string& profileId, BsonObjectPtr playerBin);
	void kickoutPlayer(Player* player);

private:

	ZoneServer* m_thisServer;
	std::unique_ptr<TcpServer<TcpConnection>> m_tcpServer;

	constexpr static std::string_view s_logCategory = "ClientToZoneModule";
};

}
