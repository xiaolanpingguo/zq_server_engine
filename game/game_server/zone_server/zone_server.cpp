#include "game_server/zone_server/zone_server.h"
#include "game_server/zone_server/zone_to_master_module.h"
#include "game_server/zone_server/internal_server_module.h"
#include "game_server/zone_server/player_manager_module.h"
#include "game_server/zone_server/zone_to_db_module.h"

#include <nlohmann-json/json.hpp>


namespace zq{


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ZoneServerConfig, appId, masterServerIp, masterServerPort, 
	dbServerIp, dbServerPort, internalIp, internalPort, externalIp, externalPort)

ZoneServer::ZoneServer(int argc, char* argv[])
	:
		ServerBase(argc,argv)
{
}

ZoneServer::~ZoneServer()
{

}

bool ZoneServer::registerServerModules()
{
	registerModule<WorldToMasterModule>(this);
	registerModule<InternalServerModule>(this);
	registerModule<PlayerManagerModule>(this);
	registerModule<WorldToDBModule>(this);
	return true;
}

bool ZoneServer::readServerConfig()
{
	// we need to read config first, the log has not created for now
	std::ifstream f(m_configName);
	if (!f.is_open())
	{
		printf("cannot find server config file:%s.\n", m_configName.c_str());
		return false;
	}

	nlohmann::json data = nlohmann::json::parse(f, nullptr, false);
	if (data.is_discarded())
	{
		printf("parse server config file:%s failed.\n", m_configName.c_str());
		return false;
	}

	data.get_to(m_serverConfg);
	return true;
}




}

