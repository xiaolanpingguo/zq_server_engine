#pragma once


#include <functional>
#include <list>
#include <queue>
#include <memory>
#include <future>
#include <assert.h>


namespace zq{


template <typename T>
class QueryCallback
{
public:

	using QueryResult = T;
	using QueryResultFuture = std::future<QueryResult>;
	using QueryResultPromise = std::promise<QueryResult>;
	using CallbackT = std::function<void(QueryCallback&, QueryResult)>;

    explicit QueryCallback(QueryResultFuture&& result)
    {
		m_result = std::move(result);
    }

    QueryCallback(QueryCallback&& right) noexcept
    {
		m_result = std::move(right.m_result);
		m_callbacks = std::move(right.m_callbacks);
    }

    QueryCallback& operator=(QueryCallback&& right) noexcept
    {
		if (this != &right)
		{
			m_result = std::move(right.m_result);
			m_callbacks = std::move(right.m_callbacks);
		}

		return *this;
    }

    ~QueryCallback()
    {

    }

    QueryCallback&& withCallback(std::function<void(QueryResult)>&& callback)
    {
		return withChainingCallback([callback](QueryCallback& /*this*/, QueryResult result) { callback(std::move(result)); });
    }

    QueryCallback&& withChainingCallback(std::function<void(QueryCallback&, QueryResult)>&& callback)
    {
		m_callbacks.push(std::move(callback));
		return std::move(*this);
    }

    void setNextQuery(QueryCallback&& next)
    {
		this->m_result = std::move(next.m_result);
    }

    bool invokeIfReady()
    {
		if (m_callbacks.empty())
		{
			return true;
		}

		CallbackT& callback = m_callbacks.front();
		auto checkStateAndReturnCompletion = [this]() {
			m_callbacks.pop();
			bool hasNext = m_result.valid();
			if (m_callbacks.empty())
			{
				assert(!hasNext);
				return true;
			}

			// abort chain
			if (!hasNext)
			{
				return true;
			}

			return false;
		};

		if (m_result.valid() && m_result.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			QueryResultFuture f(std::move(m_result));
			std::function<void(QueryCallback&, QueryResult)> cb(std::move(callback));
			cb(*this, f.get());
			return checkStateAndReturnCompletion();
		}

		return false;
    }

private:
    QueryCallback(QueryCallback const& right) = delete;
    QueryCallback& operator=(QueryCallback const& right) = delete;

    std::queue<CallbackT> m_callbacks;
    QueryResultFuture m_result;
};

}
