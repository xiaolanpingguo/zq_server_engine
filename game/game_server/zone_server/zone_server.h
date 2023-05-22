#pragma once


#include "game_common/server_base.h"

namespace zq {


struct ZoneServerConfig
{
	std::string appId;
	std::string masterServerIp;
	uint16_t masterServerPort;
	std::string internalIp;
	uint16_t internalPort;
	std::string externalIp;
	uint16_t externalPort;
	std::string mongoUser;
	std::string mongoPwd;
	std::string mongoHost;
	uint16_t mongoPort;
	std::string redisAuth;
	std::string redisHost;
	uint16_t redisPort;
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
	const std::string& getAppIdStr() override { return m_serverConfg.appId; }
	const ZoneServerConfig& getConfig() { return m_serverConfg; }

private:

	bool readServerConfig() override;
	bool registerServerModules() override;

private:

	ZoneServerConfig m_serverConfg;
	constexpr static std::string_view s_logCategory = "ZoneServer";
};


}
