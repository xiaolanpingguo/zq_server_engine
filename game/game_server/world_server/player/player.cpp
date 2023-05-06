#include "game_server/world_server/player/player.h"
#include "game_server/world_server/player/player_baseinfo_module.h"
#include "game_server/world_server/player/player_operator_module.h"


namespace zq{


Player::Player()
{
	registerModule<PlayerBaseInfoModule>(*this);
	registerModule<PlayerOperatorModule>(*this);
}

Player::~Player()
{
	for (auto& it : m_playerModules)
	{
		delete it.second;
		it.second = nullptr;
	}
}

bool Player::loadFromDB(const DB::DBPlayer& playerDBData)
{
	for (const auto& m : m_playerModules)
	{
		if (!m.second->loadFromDB(playerDBData))
		{
			return false;
		}
	}

	return true;
}

bool Player::saveToDB(DB::DBPlayer& playerDBData)
{
	for (const auto& m : m_playerModules)
	{
		if (!m.second->saveToDB(playerDBData))
		{
			return false;
		}
	}

	return true;
}


}

