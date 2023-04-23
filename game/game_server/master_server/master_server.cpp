#include "game_server/master_server/master_server.h"


namespace zq{


MasterServer::MasterServer(int argc, char* argv[])
	:
		ServerBase(argc,argv),
		m_networkServerModule(m_ioContext)
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

	TcpServerConfig config{"127.0.0.1", 20001};
	m_networkServerModule.init(config);

	return true;
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

