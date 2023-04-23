#pragma once


#include "common/common.hpp"
#include "common/message_buffer.hpp"
#include "server/timer.h"

#include "asio/asio/include/asio.hpp"


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

protected:
	virtual void stop();

private:

	bool initLog();
	void registerSignal();
	void signalHandler(std::error_code ec, int signo);

private:

	virtual std::string_view getServername() = 0;

	void onClientAccept(uint64_t id, const std::string& ip, int port);
	void onClientDataReceived(uint64_t id, const MessageBuffer& buffer);
	void onClientDisconnect(uint64_t id);

protected:

	uint32_t m_serverId;
	std::string m_configPath;

	asio::io_context m_ioContext;
	asio::signal_set m_signals;

	std::unique_ptr<Timer> m_timer;

	constexpr static std::string_view s_logCategory = "BaseServer";
};

}
