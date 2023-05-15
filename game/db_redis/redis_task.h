#pragma once


#include "common/common.hpp"
#include "game_common/query_callback.hpp"

struct redisReply;
namespace zq{



using RedisResultPtr = std::shared_ptr<redisReply>;
using RedisQueryCallback = QueryCallback<RedisResultPtr>;
using RedisQueryResultFuture = typename RedisQueryCallback::QueryResultFuture;
using RedisQueryResultPromise = typename RedisQueryCallback::QueryResultPromise;


class RedisModule;
struct RedisClient;
class RedisTask
{
public:
	RedisTask(RedisModule* redisModule, RedisClient& client, const std::string& command);
	virtual ~RedisTask() {}
	virtual void execute();
	RedisQueryResultFuture getFuture();

protected:

	RedisModule* m_redisModule;
	RedisClient& m_client;
	std::string m_command;
	std::unique_ptr<RedisQueryResultPromise> m_result;
};


}
