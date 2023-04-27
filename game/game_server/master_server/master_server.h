#pragma once


#include "game_common/server_base.h"


namespace zq {


struct MasterServerConfig
{
	int serverId = 0;
	std::string internalIp;
	int internalPort = 0;
};

class MasterServer : public ServerBase
{
	INIT_SERVER_NAME(MasterServer);

public:
	MasterServer(int argc, char* argv[]);
	~MasterServer();

public:
	uint16 getServerId() override { return m_serverId; }
	const MasterServerConfig& getConfig() { return m_serverConfg; }

private:
	bool readServerConfig() override;
	bool registerServerModules() override;

private:

	MasterServerConfig m_serverConfg;
	constexpr static std::string_view s_logCategory = "MasterServer";
};

}
