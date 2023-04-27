#include "game_server/world_server/world_server.h"
#include "game_server/world_server/internal_network_client_module.h"
#include "game_server/world_server/internal_network_server_module.h"

#include <nlohmann-json/json.hpp>


namespace zq{


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WorldServerConfig, serverId, internalIp, internalPort)

WorldServer::WorldServer(int argc, char* argv[])
	:
		ServerBase(argc,argv)
{
}

WorldServer::~WorldServer()
{

}

bool WorldServer::registerServerModules()
{
	bool r = registerModule<InternalNetworkClientModule>(this);
	r &= registerModule<InternalNetworkServerModule>(this);
	return r;
}

bool WorldServer::readServerConfig()
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

