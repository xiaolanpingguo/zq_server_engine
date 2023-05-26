#include "game_common/server_base.h"
#include "common/timer.h"
#include "network/tcp_server.hpp"
#include "network/tcp_client.hpp"
#include "game_common/gid.h"
#include "common/file.hpp"
#include <csignal>
#if defined(PLATFORM_WIN)
#include <processthreadsapi.h> // GetCurrentProcessId
#include <consoleapi2.h> // SetConsoleTitleA
#else
#include <unistd.h> // getpid
#include <sys/types.h> //fork
#include <sys/stat.h> //umask
#include <signal.h> // kill()
#include <sys/file.h> // flock()
#endif

namespace zq{

static constexpr const char* s_serverCfgDir = "assets/server_cfg/";
static constexpr const char* s_lockFileDir = "";
static constexpr const char* s_pidFileDir = ""; 
static std::string s_lockFile;
static std::string s_pidFile; 
static std::function<void(int sgno)> s_signalHandler = nullptr;

constexpr const int APPCMD_NOTHING_TODO = 10001;
constexpr const int APPCMD_STOP_SERVICE = 10002;
constexpr const int APPCMD_RELOAD_CONFIG = 10003;
constexpr const int APPCMD_QUIT_SERVICE = 10004;

static int getPid()
{
#if defined(PLATFORM_WIN)
	int pid = (int)GetCurrentProcessId();
#elif defined(PLATFORM_LINUX)
	int pid = getpid();
#else
	int pid = 0;
#endif

	return pid;
}

#if defined(PLATFORM_LINUX)
static int checkLock(const std::string& file)
{
	int lockfd = open(file.c_str(), O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	if (lockfd < 0)
	{
		return -1;
	}

	if (flock(lockfd, LOCK_EX | LOCK_NB) < 0)
	{
		return -2;
	}

	return 0;
}

static bool writePidFile(const std::string& file)
{
	int pid = getPid();
	if (!File::write(file, std::to_string(pid)))
	{
		return false;
	}

	return true;
}

static void cleanPidFile()
{
	unlink(s_pidFile.c_str());
}

static int readPidFile(const std::string& file)
{
	std::string out;
	if (!File::read(file, out, std::ios::in))
	{
		printf("failed to read pid file: %s\n", file.c_str());
		return -1;
	}

	int pid = atoi(out.c_str());
	if (pid == 0)
	{
		printf("read a wrong pid, pid file : %s, please check your pid file!\n", file.c_str());
	}
	return pid;
}

static void _signalHandler(int signo)
{
	if (s_signalHandler)
	{
		s_signalHandler(signo);
	}
}
#endif

ServerBase::ServerBase(int argc, char* argv[])
	:
		m_ioContext(),
		m_stop(false),
		m_timer(std::make_unique<Timer>(m_ioContext))
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		m_args.emplace_back(arg);
	}
}

ServerBase::~ServerBase()
{

}

bool ServerBase::start()
{
	bool success = false;
	do 
	{
		std::string fileName = getCommandArg("cfg=");
		if (fileName.empty())
		{
			printf("there are missing config file, start server with 'server cfg=[config name]'\n");
			break;
		}

		m_configName += s_serverCfgDir + fileName;
		if (!readServerConfig())
		{
			break;
		}

		if (!checkAppid())
		{
			break;
		}

		std::string cmd = getCommandArg("cmd=");
		s_pidFile = std::string(s_pidFileDir) + getAppIdStr() + ".pid";
		if (!processCmd(cmd))
		{
			break;
		}

		if (!daemonLaunch())
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

#if defined(PLATFORM_WIN)
	std::string title = fmt::format("Server: {}  AppId: {}  Pid: {}",  getName(), getAppIdStr(), getPid());
	SetConsoleTitleA(title.c_str());
#endif
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
	LOG_INFO(s_logCategory, "server:{}, appId:{} will stop...", getName(), getAppIdStr());

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
		LOG_INFO(s_logCategory, "io service has stopped");
	}

	LOG_INFO(s_logCategory, "server:{}, appId:{} has stopped", getName(), getAppIdStr());
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

bool ServerBase::processCmd(const std::string& cmd)
{
#if defined(PLATFORM_LINUX)
	if (cmd == "start")
	{
		return true;
	}

	int pid = readPidFile(s_pidFile.c_str());
	if (pid > 0)
	{
		if (cmd == "stop")
		{
			printf("will send stop command to pid: %d\n", pid);
			kill(pid, SIGUSR1);
		}
		else if (cmd == "reload")
		{
			printf("will send reload command to pid: %d\n", pid);
			kill(pid, SIGUSR2);
		}
		else
		{
			printf("unsupported cmd:%s\n", cmd.c_str());
		}
	}

	::exit(0);
#endif

	return true;
}

bool ServerBase::daemonLaunch()
{
#if defined(PLATFORM_LINUX)

	// check if the server is running
	s_lockFile = std::string(s_lockFileDir) + getAppIdStr() + ".lock";
	int ret = checkLock(s_lockFile);
	if (ret != 0)
	{
		std::cout << "check lock failed,  same server is running!, appid: " << getAppIdStr() << std::endl;
		::exit(1);
	}

	daemon(1, 0);

	if (!installSignals())
	{
		printf("install signal failed!\n");
		return false;
	}

	if (!writePidFile(s_pidFile))
	{
		printf("write pid file failed!\n");
		return false;
	}

	atexit(cleanPidFile);

	return true;
#else
	return true;
#endif
}

bool ServerBase::checkAppid()
{
	if (!m_appId.init(getAppIdStr()))
	{
		printf("check appid failed! appid: %s\n", getAppIdStr().c_str());
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

bool ServerBase::installSignals()
{
#if defined(PLATFORM_LINUX)
	static auto lam = [](int sigNum) {
		if (signal(sigNum, SIG_IGN) == SIG_ERR) return false;
		return true;
	};
	int signos[] = { SIGINT, SIGHUP, SIGQUIT, SIGTTOU, SIGTTIN, SIGCHLD, SIGTERM, SIGHUP, SIGPIPE };
	for (size_t i = 0; i < sizeof(signos) / sizeof(int); ++i)
	{
		if (!lam(signos[i]))
		{
			printf("install signal failed, index:%ld, signo:%d\n", i, signos[i]);
			return false;
		}
	}

	std::signal(SIGUSR1, _signalHandler);
	std::signal(SIGUSR2, _signalHandler);
	s_signalHandler = std::bind(&ServerBase::signalHandler, this, std::placeholders::_1);
#endif

	return true;
}

std::string ServerBase::getCommandArg(std::string_view str)
{
	if (str.empty())
	{
		return "";
	}

	for (const std::string& arg : m_args)
	{
		std::string temp = arg;
		if (temp.find(str) != std::string::npos)
		{
			temp.erase(0, str.size());
			return temp;
		}
	}

	return "";
}

void ServerBase::signalHandler(int signo)
{
#if defined(PLATFORM_LINUX)
	if (signo == SIGUSR1)
	{
		LOG_INFO(s_logCategory, "received 'stop' signal:{}, server will shutdown..", signo);
		shutdown();
	}
	if (signo == SIGUSR2)
	{
		// to do
		LOG_INFO(s_logCategory, "received 'reload' signal:{}, server will reload config..", signo);
	}
#endif
}


}

