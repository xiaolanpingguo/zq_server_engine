#pragma once


#include "common/common.hpp"
#include "game_common/i_module.hpp"
#include "common/coroutine_awaitor.hpp"


namespace zq {


class ZoneServer;
class Player;
struct SDKAccountInfo;
class TcpConnection;
class BsonObject;
class PlayerManagerModule : public IModule
{
	INIT_MODULE_NAME(PlayerManagerModule);

public:
	PlayerManagerModule(ZoneServer* thisServer);
	~PlayerManagerModule();

	bool init() override;
	bool update(uint64_t delta) override;
	bool finalize() override;

	Player* findPlayer(std::string profileId);
	async_simple::coro::Lazy<int> onPlayerLogin(const std::string& profileId, const SDKAccountInfo& sdkInfo, std::shared_ptr<TcpConnection> conn);

private:
	async_simple::coro::Lazy<int> getPlayerDBData(const std::string& profileId, std::shared_ptr<BsonObject> playerBin);
	async_simple::coro::Lazy<int> savePlayerDBData(Player* player);
	bool saveToDB();

public:

	ZoneServer* m_thisServer;
	std::unordered_map<std::string, Player*> m_players;

	constexpr static std::string_view s_logCategory = "PlayerManagerModule";
};


}
