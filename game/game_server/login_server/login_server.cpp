#include "game_server/login_server/login_server.h"
#include "game_server/login_server/internal_server_module.h"
#include "game_server/login_server/client_to_login_module.h"
#include "db_mongo/mongo_module.h"
#include "db_redis/redis_module.h"
#include "game_common/game_db_def.hpp"

#include <nlohmann-json/json.hpp>


namespace zq{

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LoginServerConfig, appId, masterServerIp, masterServerPort,
		internalIp, internalPort, externalIp, externalPort,
		mongoUser, mongoPwd, mongoHost, mongoPort,
		redisAuth, redisHost, redisPort)

LoginServer::LoginServer(int argc, char* argv[])
	:
		ServerBase(argc,argv)
{
}

LoginServer::~LoginServer()
{

}

bool LoginServer::registerServerModules()
{
	std::vector<std::pair<std::string, std::string>> collections
	{
		{ DB_NAME, COL_ACCOUNT },
		{ DB_NAME, COL_PLAYER },
	};

	registerModule<InternalServerModule>(this);
	registerModule<MongoModule>(m_serverConfg.mongoUser, m_serverConfg.mongoPwd, m_serverConfg.mongoHost, m_serverConfg.mongoPort, collections);
	registerModule<RedisModule>(m_serverConfg.redisAuth, m_serverConfg.redisHost, m_serverConfg.redisPort);
	registerModule<ClientToLoginModule>(this);
	return true;
}

bool LoginServer::readServerConfig()
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

