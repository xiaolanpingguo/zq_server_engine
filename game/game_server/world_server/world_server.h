#pragma once


#include "game_common/server_base.h"

namespace zq {


struct WorldServerConfig
{
	int serverId = 0;
	std::string internalIp;
	int internalPort = 0;
};

class InternalNetworkClientModule;
class InternalNetworkServerModule;
class WorldServer : public ServerBase
{
	INIT_SERVER_NAME(WorldServer);

public:
	WorldServer(int argc, char* argv[]);
	~WorldServer();

public:
	uint16 getServerId() override { return m_serverId; }
	const WorldServerConfig& getConfig() { return m_serverConfg; }

private:

	bool readServerConfig() override;
	bool registerServerModules() override;

private:

	WorldServerConfig m_serverConfg;
	constexpr static std::string_view s_logCategory = "WorldServer";
};


}
