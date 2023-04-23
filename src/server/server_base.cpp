#include "server/server_base.h"
#include "server/network/tcp_server.hpp"
#include "server/network/tcp_client.hpp"


namespace zq{


ServerBase::ServerBase(int argc, char* argv[])
	:
		m_ioContext(),
		m_timer(std::make_unique<Timer>(m_ioContext)),
		m_signals(m_ioContext),
		m_serverId(0)
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg.find("-cfg=") != std::string::npos)
		{
			arg.erase(0, 4);
			m_configPath = arg;
			break;
		}
	}

	//ASSERT(!m_configPath.empty(), "there are missing config file, start server with 'server -cfg=path'");
}

ServerBase::~ServerBase()
{

}

bool ServerBase::start()
{
	m_serverId = 1000;
	if (!initLog())
	{
		ASSERT(false, "create log failed!");
		return false;
	}
	registerSignal();

	return true;
}

void ServerBase::run()
{
	m_ioContext.run();
}

void ServerBase::stop()
{
	m_ioContext.stop();
}

uint64_t ServerBase::addTimer(std::chrono::steady_clock::duration interval, const std::function<void(void*)>& fn, bool runOnce)
{
	return m_timer->addTimer(interval, fn, runOnce);
}

bool ServerBase::cancelTimer(uint64_t id)
{
	return m_timer->cancelTimer(id);
}

bool ServerBase::initLog()
{
	std::string name = std::string(getServername()) + "-" + std::to_string(m_serverId);
	if (!Log::getInstance().init("server", 4))
	{
		return false;
	}

	return true;
}

void ServerBase::registerSignal()
{
	m_signals.add(SIGINT);
	m_signals.add(SIGTERM);

#if defined(PLATFORM_LINUX)
	m_signals.add(SIGHUP);
	m_signals.add(SIGPIPE);
	m_signals.add(SIGQUIT);
#endif

	m_signals.async_wait(std::bind(&ServerBase::signalHandler, this, std::placeholders::_1, std::placeholders::_2));
}

void ServerBase::signalHandler(std::error_code ec, int signo)
{
	stop();
#if defined(PLATFORM_LINUX)
	m_signals.add(SIGHUP);
	m_signals.add(SIGPIPE);
	m_signals.add(SIGQUIT);
#endif
}

void ServerBase::onClientAccept(uint64_t id, const std::string& ip, int port)
{

}

void ServerBase::onClientDataReceived(uint64_t id, const MessageBuffer& buffer)
{

}

void ServerBase::onClientDisconnect(uint64_t id)
{

}

}

