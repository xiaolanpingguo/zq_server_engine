#pragma once


#include "game_server/zone_server/player/i_player_module.h"


namespace zq
{

class Player;
class PlayerBaseInfoModule : public IPlayerModule
{
	INIT_MODULE_NAME(PlayerBaseInfoModule);

public:
	PlayerBaseInfoModule(Player& player);
	~PlayerBaseInfoModule();

	bool loadFromDB(const S2S::DBPlayerData& playerDBData) override;
	bool saveToDB(S2S::DBPlayerData& playerDBData) override;

public:

	Player& m_player;
	constexpr static std::string_view s_logCategory = "PlayerBaseInfoModule";
};

} //namespace zq
