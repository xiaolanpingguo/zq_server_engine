#include "game_server/zone_server/player/player_operator_module.h"
#include "game_server/zone_server/player/player_baseinfo_module.h"
#include "game_server/zone_server/player/player.h"


namespace zq{


PlayerOperatorModule::PlayerOperatorModule(Player& player) :
	m_player(player)
{
}

PlayerOperatorModule::~PlayerOperatorModule()
{
}

bool PlayerOperatorModule::loadFromDB(const S2S::DBPlayerData& dbPlayerData)
{
	PlayerBaseInfoModule* m = m_player.getModule<PlayerBaseInfoModule>();

	return true;
}

bool PlayerOperatorModule::saveToDB(S2S::DBPlayerData& dbPlayerData)
{

	return true;
}

} //namespace zq
