#include "server/timer.h"


namespace zq{


struct TimerData
{
	std::chrono::steady_clock::duration interval;
	void* userData;
	std::function<void(void*)> callback;
	bool runOnce = false;
};


Timer::Timer(asio::io_context& ioContext)
	:
		m_ioContext(ioContext),
		m_id(0)
{

}

Timer::~Timer()
{

}

uint64_t Timer::addTimer(std::chrono::steady_clock::duration interval, const std::function<void(void*)>& fn, bool runOnce)
{
	std::shared_ptr<TimerData> data = std::make_shared<TimerData>();
	data->interval = interval;
	data->callback = fn;
	data->runOnce = runOnce;

	std::shared_ptr<asio::steady_timer> timer = std::make_shared<asio::steady_timer>(m_ioContext);
	timer->expires_at(timer->expiry() + interval);
	timer->async_wait(std::bind(&Timer::timerCallback, this, timer, data));

	m_timers[m_id] = timer;
	return m_id++;
}

bool Timer::cancelTimer(uint64_t id)
{
	auto it = m_timers.find(id);
	if (it == m_timers.end())
	{
		LOG_ERROR(s_logCategory, "cancel timer errorm can't find timer id:{}.", id);
		return false;
	}

	std::shared_ptr<asio::steady_timer> timer = it->second;
	m_timers.erase(it);
	timer->cancel();
	return true;
}

void Timer::timerCallback(std::shared_ptr<asio::steady_timer> timer, std::shared_ptr<TimerData> timerData)
{
	timerData->callback(timerData->userData);
	if (!timerData->runOnce)
	{
		timer->expires_at(timer->expiry() + timerData->interval);
		timer->async_wait(std::bind(&Timer::timerCallback, this, timer, timerData));
	}
}


}

