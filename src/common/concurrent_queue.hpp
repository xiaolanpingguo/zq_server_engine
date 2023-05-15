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

	ConcurrentQueue(){}
	~ConcurrentQueue() {}

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
		if (m_queue.empty())
		{
			return false;
		}

		value = m_queue.front();
		m_queue.pop();
		return true;
	}

	bool waitAndPop(T& value)
	{
		std::unique_lock<std::mutex> lock(m_queueLock);
		while (m_queue.empty() && !m_stopWait)
		{
			m_condition.wait(lock);
		}

		if (m_queue.empty())
		{
			return false;
		}

		value = m_queue.front();
		m_queue.pop();
		return true;
	}

	void stopWait()
	{
		std::unique_lock<std::mutex> lock(m_queueLock);
		m_stopWait = true;
		m_condition.notify_all();
	}

private:
	std::mutex m_queueLock;
	std::queue<T> m_queue;
	std::condition_variable m_condition;
	std::atomic<bool> m_stopWait = false;
};

}


