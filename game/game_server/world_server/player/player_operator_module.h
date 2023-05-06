#pragma once

#include "game_server/world_server/player/i_player_module.h"

namespace zq
{

class Player;
class PlayerOperatorModule : public IPlayerModule
{
	INIT_MODULE_NAME(PlayerOperatorModule);

public:
	PlayerOperatorModule(Player& player);
	~PlayerOperatorModule();

	bool loadFromDB(const DB::DBPlayer& playerDBData) override;
	bool saveToDB(DB::DBPlayer& playerDBData) override;

public:
	Player& m_player;
	constexpr static std::string_view s_logCategory = "PlayerBaseInfoModule";
};

} //namespace zq
