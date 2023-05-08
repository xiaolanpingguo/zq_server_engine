#pragma once


#include "game_common/server_base.h"


namespace zq {


struct DBServerConfig
{
	std::string appId;
	std::string internalIp;
	int internalPort;

	// db
	std::string user;
	std::string pwd;
	std::string dbHost;
	uint16_t dbPort;
};

class DBServer : public ServerBase
{
	INIT_SERVER_NAME(DBServer);

public:
	DBServer(int argc, char* argv[]);
	~DBServer();

public:
	const std::string& getStrAppId() override { return m_serverConfg.appId; }
	const DBServerConfig& getConfig() { return m_serverConfg; }

private:
	bool readServerConfig() override;
	bool registerServerModules() override;

private:

	DBServerConfig m_serverConfg;
	constexpr static std::string_view s_logCategory = "MasterServer";
};

}
