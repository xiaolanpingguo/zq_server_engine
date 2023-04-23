#pragma once


#include "server/server_base.h"
#include "game_server/world_server/network_client_module.h"



namespace zq {


class WorldServer : public ServerBase
{
public:
	WorldServer(int argc, char* argv[]);
	~WorldServer();

public:
	bool start() override;
	void run() override;

private:

	virtual void stop();

	virtual std::string_view getServername() override
	{
		constexpr static std::string_view name = "WorldServer";
		return name;
	}

private:

	NetworkClientModule m_networkClientModule;
	constexpr static std::string_view s_logCategory = "WorldServer";
};

}
