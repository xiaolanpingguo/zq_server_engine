#pragma once


#include "server/server_base.h"
#include "game_server/world_server/network_server_module.h"



namespace zq {


class MasterServer : public ServerBase
{
public:
	MasterServer(int argc, char* argv[]);
	~MasterServer();

public:
	bool start() override;
	void run() override;

private:

	virtual void stop();

	virtual std::string_view getServername() override
	{
		constexpr static std::string_view name = "MasterServer";
		return name;
	}

private:

	NetworkServerModule m_networkServerModule;
	constexpr static std::string_view s_logCategory = "MasterServer";
};

}
