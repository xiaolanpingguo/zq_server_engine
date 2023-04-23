#include "game_server/world_server/world_server.h"


namespace zq{


WorldServer::WorldServer(int argc, char* argv[])
	:
		ServerBase(argc,argv),
		m_networkClientModule(m_ioContext)
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

	TcpClientConfig config{"127.0.0.1", 20001};
	m_networkClientModule.init(config);

	return true;
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

