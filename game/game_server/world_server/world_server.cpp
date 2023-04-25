#include "game_server/world_server/world_server.h"
#include "game_server/world_server/internal_network_client_module.h"
#include "game_server/world_server/internal_network_server_module.h"

namespace zq{


WorldServer::WorldServer(int argc, char* argv[])
	:
		ServerBase(argc,argv)
{
	m_serverId = 1001;
}

WorldServer::~WorldServer()
{

}

bool WorldServer::start()
{
	if (!ServerBase::start())
	{
		return false;
	}

	return true;
}

bool WorldServer::registerServerModules()
{
	bool r = registerModule<InternalNetworkClientModule>(this);
	r &= registerModule<InternalNetworkServerModule>(this);
	return r;
}

void WorldServer::run()
{
	ServerBase::run();
}

void WorldServer::stop()
{
	ServerBase::stop();
}




}

