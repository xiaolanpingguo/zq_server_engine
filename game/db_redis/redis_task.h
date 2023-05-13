#pragma once


#include "common/common.hpp"
#include "game_common/query_callback.hpp"

struct redisReply;
namespace zq{



using RedisResultPtr = std::shared_ptr<redisReply>;
using RedisQueryCallback = QueryCallback<RedisResultPtr>;
using QueryResultFuture = typename RedisQueryCallback::QueryResultFuture;
using QueryResultPromise = typename RedisQueryCallback::QueryResultPromise;


class RedisModule;
class RedisTask
{
public:
	RedisTask(RedisModule* redisModule, const std::string& command);
	virtual ~RedisTask() {}
	virtual void execute();
	QueryResultFuture getFuture();

protected:
	std::unique_ptr<QueryResultPromise> m_result;

	std::string m_command;
	RedisModule* m_redisModule;
};


}
