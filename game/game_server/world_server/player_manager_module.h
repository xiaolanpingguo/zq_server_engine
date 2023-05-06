#pragma once


#include "common/common.hpp"
#include "game_common/i_module.hpp"


namespace zq {


class WorldServer;
class Player;
class PlayerManagerModule : public IModule
{
	INIT_MODULE_NAME(PlayerManagerModule);

public:
	PlayerManagerModule(WorldServer* thisServer);
	~PlayerManagerModule();

public:

	WorldServer* m_thisServer;
	std::unordered_map<std::string, Player*> m_playerModules;

	constexpr static std::string_view s_logCategory = "PlayerManagerModule";
};


}
