#pragma once


#include "common/common.hpp"
#include "game_common/i_module.hpp"


namespace zq {


class ZoneServer;
class Player;
class PlayerManagerModule : public IModule
{
	INIT_MODULE_NAME(PlayerManagerModule);

public:
	PlayerManagerModule(ZoneServer* thisServer);
	~PlayerManagerModule();

	Player* findPlayer(std::string profileId);

private:
	bool saveToDB();

public:

	ZoneServer* m_thisServer;
	std::unordered_map<std::string, Player*> m_players;

	constexpr static std::string_view s_logCategory = "PlayerManagerModule";
};


}
