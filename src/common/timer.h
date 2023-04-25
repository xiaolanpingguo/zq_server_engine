#pragma once


#include "common/common.hpp"

#include <asio/steady_timer.hpp>


namespace zq {


struct TimerData;
class Timer
{
public:
	Timer(asio::io_context& m_ioContext);
	~Timer();

public:

	uint64_t addTimer(std::chrono::steady_clock::duration interval, const std::function<void(void*)>& fn, bool runOnce = false);
	bool cancelTimer(uint64_t id);

private:

	void timerCallback(std::shared_ptr<asio::steady_timer> timer, std::shared_ptr <TimerData> timerData);
private:
	asio::io_context& m_ioContext;
	uint64_t m_id;
	std::unordered_map<uint64_t, std::shared_ptr<asio::steady_timer>> m_timers;

	constexpr static std::string_view s_logCategory = "Timer";
};

}
