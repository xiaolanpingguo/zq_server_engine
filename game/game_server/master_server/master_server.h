#pragma once


#include "game_common/server_base.h"


namespace zq {


struct MasterServerConfig
{
	std::string appId;
	std::string internalIp;
	int internalPort;
};

class MasterServer : public ServerBase
{
	INIT_SERVER_NAME(MasterServer);

public:
	MasterServer(int argc, char* argv[]);
	~MasterServer();

public:
	const std::string& getAppIdStr() override { return m_serverConfg.appId; }
	const MasterServerConfig& getConfig() { return m_serverConfg; }

private:
	bool readServerConfig() override;
	bool registerServerModules() override;

private:

	MasterServerConfig m_serverConfg;
	constexpr static std::string_view s_logCategory = "MasterServer";
};

}
