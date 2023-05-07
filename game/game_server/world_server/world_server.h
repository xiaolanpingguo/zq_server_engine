#pragma once


#include "game_common/server_base.h"

namespace zq {


struct WorldServerConfig
{
	std::string appId;
	std::string masterServerIp = "127.0.0.1";
	int masterServerPort = 10001;
	std::string externalIp = "127.0.0.1";
	int externalPort = 20001;
};

class WorldToMasterModule;
class InternalNetworkServerModule;
class WorldServer : public ServerBase
{
	INIT_SERVER_NAME(WorldServer);

public:
	WorldServer(int argc, char* argv[]);
	~WorldServer();

public:
	const std::string& getStrAppId() override { return m_serverConfg.appId; }
	const WorldServerConfig& getConfig() { return m_serverConfg; }

private:

	bool readServerConfig() override;
	bool registerServerModules() override;

private:

	WorldServerConfig m_serverConfg;
	constexpr static std::string_view s_logCategory = "WorldServer";
};


}
