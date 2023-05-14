#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <atomic>
#include <type_traits>

namespace zq {

template <typename T>
class ConcurrentQueue
{
public:

	ConcurrentQueue() : m_shutdown(false) {}

	void push(const T& value)
	{
		std::lock_guard<std::mutex> lock(m_queueLock);
		m_queue.push(std::move(value));
		m_condition.notify_one();
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(m_queueLock);
		return m_queue.empty();
	}

	bool pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m_queueLock);
		if (m_queue.empty() || m_shutdown)
		{
			return false;
		}

		value = m_queue.front();
		m_queue.pop();
		return true;
	}

	void waitAndPop(T& value)
	{
		std::unique_lock<std::mutex> lock(m_queueLock);
		while (m_queue.empty() && !m_shutdown)
		{
			m_condition.wait(lock);
		}

		if (m_queue.empty() || m_shutdown)
		{
			return;
		}

		value = m_queue.front();
		m_queue.pop();
	}

	void cancel()
	{
		std::unique_lock<std::mutex> lock(m_queueLock);

		while (!m_queue.empty())
		{
			T& value = m_queue.front();
			m_queue.pop();
		}

		m_shutdown = true;
		m_condition.notify_all();
	}

private:
	std::mutex m_queueLock;
	std::queue<T> m_queue;
	std::condition_variable m_condition;
	std::atomic<bool> m_shutdown;
};

}


