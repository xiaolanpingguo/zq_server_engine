#pragma once


#include "game_common/server_base.h"



namespace zq {


class InternalNetworkClientModule;
class InternalNetworkServerModule;
class WorldServer : public ServerBase
{
public:
	WorldServer(int argc, char* argv[]);
	~WorldServer();

public:
	bool start() override;
	void run() override;
	bool registerServerModules() override;

private:

	virtual void stop();

	virtual std::string_view getServername() override
	{
		constexpr static std::string_view name = "WorldServer";
		return name;
	}

private:

	//InternalNetworkClientModule m_internalNetworkClientModule;
	constexpr static std::string_view s_logCategory = "WorldServer";
};


}
