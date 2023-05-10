#include "game_server/login_server/login_server.h"
#include "game_server/login_server/internal_server_module.h"
#include "game_server/login_server/client_to_login_module.h"
#include "game_server/login_server/login_to_db_module.h"

#include <nlohmann-json/json.hpp>


namespace zq{

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LoginServerConfig, appId, masterServerIp, masterServerPort,
		dbServerIp, dbServerPort, internalIp, internalPort, externalIp, externalPort)

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
	registerModule<InternalServerModule>(this);
	registerModule<LoginToDBModule>(this);
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

