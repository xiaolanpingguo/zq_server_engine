#pragma once


#include "game_common/server_base.h"


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

	bool registerServerModules() override;
	virtual void stop();

	virtual std::string_view getServername() override
	{
		constexpr static std::string_view name = "MasterServer";
		return name;
	}

private:

	constexpr static std::string_view s_logCategory = "MasterServer";
};

}
