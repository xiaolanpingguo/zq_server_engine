#include "game_server/db_server/db_server.h"
#include "game_server/db_server/internal_server_module.h"
#include "game_server/db_server/db_mongo_module.h"

#include <nlohmann-json/json.hpp>


namespace zq{

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DBServerConfig, appId, masterServerIp, masterServerPort, internalIp, internalPort, user, pwd, dbHost, dbPort)

DBServer::DBServer(int argc, char* argv[])
	:
		ServerBase(argc,argv)
{
}

DBServer::~DBServer()
{

}

bool DBServer::registerServerModules()
{
	registerModule<InternalServerModule>(this);
	registerModule<DBMongoModule>(this);
	return true;
}

bool DBServer::readServerConfig()
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

