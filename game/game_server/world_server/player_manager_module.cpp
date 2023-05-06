#include "game_server/world_server/player_manager_module.h"
#include "game_server/world_server/world_server.h"
#include "game_server/world_server/player/player.h"


namespace zq{


PlayerManagerModule::PlayerManagerModule(WorldServer* thisServer) :
		m_thisServer(thisServer)
{
}

PlayerManagerModule::~PlayerManagerModule()
{
	for (auto& it : m_playerModules)
	{
		delete it.second;
		it.second = nullptr;
	}
}



}

