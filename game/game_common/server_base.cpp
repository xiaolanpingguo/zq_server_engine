#include "game_common/server_base.h"
#include "common/timer.h"
#include "network/tcp_server.hpp"
#include "network/tcp_client.hpp"


namespace zq{


ServerBase::ServerBase(int argc, char* argv[])
	:
		m_ioContext(),
		m_work(m_ioContext),
		m_timer(std::make_unique<Timer>(m_ioContext)),
		m_signals(m_ioContext),
		m_serverId(0)
{
	std::string fileName;
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg.find("-cfg=") != std::string::npos)
		{
			arg.erase(0, 5);
			fileName = arg;
			break;
		}
	}

	ASSERT(!fileName.empty(), "there are missing config file, start server with 'server -cfg=[config name]'");
	m_configName += "assets/server_cfg/" + fileName;
}

ServerBase::~ServerBase()
{

}

bool ServerBase::start()
{
	if (!readServerConfig())
	{
		return false;
	}

	if (!initLog())
	{
		return false;
	}

	if (!registerServerModules())
	{
		return false;
	}

	if (!initModules())
	{
		return false;
	}

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
	std::string name = std::string(getName()) + "-" + std::to_string(getServerId());
	if (!Log::getInstance().init(name, 4))
	{
		return false;
	}

	return true;
}

bool ServerBase::initModules()
{
	for (const auto& it : m_modules)
	{
		if (!it.second->init())
		{
			LOG_ERROR(s_logCategory, "init module failed, name:{}", it.first);
			return false;
		}
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


}

