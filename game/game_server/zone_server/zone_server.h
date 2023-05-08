#pragma once


#include "game_common/server_base.h"

namespace zq {


struct ZoneServerConfig
{
	std::string appId;
	std::string masterServerIp;
	uint16_t masterServerPort;
	std::string dbServerIp;
	uint16_t dbServerPort;
	std::string externalIp;
	uint16_t externalPort;
};

class WorldToMasterModule;
class InternalNetworkServerModule;
class ZoneServer : public ServerBase
{
	INIT_SERVER_NAME(ZoneServer);

public:
	ZoneServer(int argc, char* argv[]);
	~ZoneServer();

public:
	const std::string& getStrAppId() override { return m_serverConfg.appId; }
	const ZoneServerConfig& getConfig() { return m_serverConfg; }

private:

	bool readServerConfig() override;
	bool registerServerModules() override;

private:

	ZoneServerConfig m_serverConfg;
	constexpr static std::string_view s_logCategory = "ZoneServer";
};


}
