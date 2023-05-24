#pragma once


#include "common/common.hpp"
#include "common/message_buffer.hpp"
#include "game_common/i_module.hpp"
#include "game_common/appid.h"

#include <asio/signal_set.hpp>


namespace zq {


#define INIT_SERVER_NAME(NAME) public: std::string_view getName() override{ constexpr std::string_view name = #NAME; return name;}
class Timer;
class ServerBase
{
public:
	ServerBase(int argc, char* argv[]);
	~ServerBase();

public:

	bool start();
	void run();
	void shutdown();

	uint64_t addTimer(std::chrono::steady_clock::duration interval, const std::function<void(void*)>& fn, bool runOnce = false);
	bool cancelTimer(uint64_t id);

	asio::io_context& getIoContext() { return m_ioContext; }

	template <typename T, typename... Args>
	bool registerModule(Args&&... args)
	{
		IModule* m = new T(std::forward<Args>(args)...);
		auto it = m_modules.emplace(std::string(T::getName()), m);
		ASSERT(it.second, "register {} multiple times!", T::getName());
		return it.second;
	}

	template <typename T>
	T* getModule()
	{
		auto it = m_modules.find(std::string(T::getName()));
		if (it == m_modules.end())
		{
			ASSERT(false, "can't get module:{}!", T::getName());
			return nullptr;
		}
		return (T*)(it->second);
	}

protected:

private:

	virtual void stop();
	virtual bool registerServerModules() = 0;
	virtual bool readServerConfig() = 0;
	virtual const std::string& getAppIdStr() = 0;
	virtual std::string_view getName() = 0;

	bool checkAppid();
	bool initGid();
	bool initLog();
	void registerSignal();
	bool initModules();

	void signalHandler(std::error_code ec, int signo);

	void update(uint64_t delta);

protected:

	std::string m_configName;
	AppId m_appId;

	asio::io_context m_ioContext;
	asio::io_context::work m_work;
	asio::signal_set m_signals;
	bool m_stop;

	std::unique_ptr<Timer> m_timer;
	std::unordered_map<std::string, IModule*> m_modules;

	constexpr static std::string_view s_logCategory = "BaseServer";
};

}
