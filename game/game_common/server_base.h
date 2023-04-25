#pragma once


#include "common/common.hpp"
#include "common/message_buffer.hpp"
#include "common/timer.h"
#include "game_common/i_module.hpp"

#include <asio/signal_set.hpp>


namespace zq {


struct ServerLaunchConfig
{
	uint32_t serverId;
	bool deamon;
};

class ServerBase
{
public:
	ServerBase(int argc, char* argv[]);
	~ServerBase();

public:

	virtual bool start();
	virtual void run();

	uint64_t addTimer(std::chrono::steady_clock::duration interval, const std::function<void(void*)>& fn, bool runOnce = false);
	bool cancelTimer(uint64_t id);

	asio::io_context& getIoContext() { return m_ioContext; }

	template <typename T, typename... Args>
	bool registerModule(Args&&... args)
	{
		IModule* m = new T(std::forward<Args>(args)...);
		auto it = m_modules.emplace(typeid(T).name(), m);
		ASSERT(it.second, "register {} multiple times!", typeid(T).name());
		return it.second;
	}

	template <typename T>
	T* getModule()
	{
		auto it = m_modules.find(typeid(T).name());
		if (it == m_modules.end())
		{
			ASSERT(false, "can't get module:{}!", typeid(T).name());
			return nullptr;
		}
		return (T*)(it.second);
	}

protected:
	virtual void stop();
	virtual bool registerServerModules() = 0;

private:

	bool initLog();
	void registerSignal();
	void signalHandler(std::error_code ec, int signo);

private:

	virtual std::string_view getServername() = 0;

protected:

	uint32_t m_serverId;
	std::string m_configPath;

	asio::io_context m_ioContext;
	asio::signal_set m_signals;

	std::unique_ptr<Timer> m_timer;
	std::unordered_map<std::string, IModule*> m_modules;

	constexpr static std::string_view s_logCategory = "BaseServer";
};

}
