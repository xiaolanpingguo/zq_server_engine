#pragma once


#include "game_common/server_base.h"


namespace zq {


struct LoginServerConfig
{
	std::string appId;
	std::string masterServerIp;
	uint16_t masterServerPort;
	std::string dbServerIp;
	uint16_t dbServerPort;
	std::string internalIp;
	uint16_t internalPort;
	std::string externalIp;
	uint16_t externalPort;
};

class LoginServer : public ServerBase
{
	INIT_SERVER_NAME(LoginServer);

public:
	LoginServer(int argc, char* argv[]);
	~LoginServer();

public:
	const std::string& getStrAppId() override { return m_serverConfg.appId; }
	const LoginServerConfig& getConfig() { return m_serverConfg; }

private:
	bool readServerConfig() override;
	bool registerServerModules() override;

private:

	LoginServerConfig m_serverConfg;
	constexpr static std::string_view s_logCategory = "LoginServer";
};

}
