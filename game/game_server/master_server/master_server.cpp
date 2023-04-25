#include "game_server/master_server/master_server.h"
#include "game_server/master_server/internal_network_server_module.h"


namespace zq{


MasterServer::MasterServer(int argc, char* argv[])
	:
		ServerBase(argc,argv)
{
	m_serverId = 2001;
}

MasterServer::~MasterServer()
{

}

bool MasterServer::start()
{
	if (!ServerBase::start())
	{
		return false;
	}


	return true;
}

bool MasterServer::registerServerModules()
{
	bool r = registerModule<InternalNetworkServerModule>(this);
	return r;
}

void MasterServer::run()
{
	ServerBase::run();
}

void MasterServer::stop()
{
	ServerBase::stop();
}


}

