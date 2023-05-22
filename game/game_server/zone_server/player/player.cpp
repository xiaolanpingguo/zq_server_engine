#include "game_server/zone_server/player/player.h"
#include "network//tcp_connection.hpp"
#include "game_server/zone_server/player/player_baseinfo_module.h"
#include "game_server/zone_server/player/player_operator_module.h"



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

bool Player::loadFromDB(const S2S::DBPlayerData& playerDBData)
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

bool Player::saveToDB(S2S::DBPlayerData& playerDBData)
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

std::shared_ptr<TcpConnection> Player::getConnection()
{
	return m_conection;
}

void Player::setConnection(std::shared_ptr<TcpConnection> conn)
{
	m_conection = conn;
}


}

