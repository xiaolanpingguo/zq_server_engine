#include "game_common/server_base.h"
#include "common/timer.h"
#include "network/tcp_server.hpp"
#include "network/tcp_client.hpp"
#include "game_common/gid.h"


namespace zq{


ServerBase::ServerBase(int argc, char* argv[])
	:
		m_ioContext(),
		m_work(m_ioContext),
		m_signals(m_ioContext),
		m_stop(false),
		m_timer(std::make_unique<Timer>(m_ioContext))
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
	bool success = false;
	do 
	{
		if (!readServerConfig())
		{
			break;
		}

		if (!checkAppid())
		{
			break;
		}

		if (!initGid())
		{
			break;
		}

		if (!initLog())
		{
			break;
		}

		if (!registerServerModules())
		{
			break;
		}

		if (!initModules())
		{
			break;
		}

		success = true;
	} while (0);
	
	if (!success)
	{
		LOG_INFO(s_logCategory, "server:{}, appId:{} start failed!", getName(), getAppIdStr());
		stop();
		return false;
	}

	LOG_INFO(s_logCategory, "server:{}, appId:{} start success!", getName(), getAppIdStr());
	return true;
}

void ServerBase::run()
{
	try
	{
		static const std::chrono::steady_clock::time_point applicationStartTime = std::chrono::steady_clock::now();
		static uint64_t lastTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - applicationStartTime).count();
		while (!m_stop)
		{
			uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - applicationStartTime).count();
			uint64_t delta = (now >= lastTime) ? (now - lastTime) : 0;

			m_ioContext.poll_one();
			update(delta);

			lastTime = now;
		}
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(s_logCategory, "!!!exception occurred, server:{}, appId:{}, e:{}", getName(), getAppIdStr(), e.what());
	}

	stop();
}

void ServerBase::shutdown()
{
	m_stop = true;
}

void ServerBase::stop()
{
	LOG_INFO(s_logCategory, "server:{}, appId:{} will to stop...", getName(), getAppIdStr());

	if (m_timer != nullptr)
	{
		m_timer->stop();
		m_timer = nullptr;
		LOG_INFO(s_logCategory, "timer has stopped");
	}

	for (auto& it : m_modules)
	{
		if (!it.second->finalize())
		{
			LOG_ERROR(s_logCategory, "module:{} finalize failed, name:{}", it.first);
		}

		LOG_INFO(s_logCategory, "module: {}, has stopped", it.first);
		delete it.second;
		it.second = nullptr;
	}

	m_modules.clear();

	if (!m_ioContext.stopped())
	{
		m_ioContext.stop();
	}
	LOG_INFO(s_logCategory, "io service has stopped");

	Log::getInstance().finalize();
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
	std::string name = std::string(getName()) + "-" + getAppIdStr();
	if (!Log::getInstance().init(name, 1024 * 1024 *8))
	{
		return false;
	}

	return true;
}

bool ServerBase::checkAppid()
{
	if (!m_appId.init(getAppIdStr()))
	{
		printf("Appid failed:%s, please check your appid!\n", getAppIdStr().c_str());
		return false;
	}

	return true;
}

bool ServerBase::initGid()
{
	if (!Gid::initialize(m_appId))
	{
		printf("initialize Gid failed:%s, please check your appid!\n", getAppIdStr().c_str());
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

		LOG_INFO(s_logCategory, "init module: {}..", it.first);
	}

	return true;
}

void ServerBase::update(uint64_t delta)
{
	for (const auto& it : m_modules)
	{
		it.second->update(delta);
	}
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

