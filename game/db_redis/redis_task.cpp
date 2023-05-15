#include "db_redis/redis_task.h"
#include "db_redis/redis_module.h"


namespace zq{


RedisTask::RedisTask(RedisModule* redisModule, RedisClient& client, const std::string& command) :
		m_redisModule(redisModule),
		m_client(client),
		m_command(command)
{
	m_result = std::make_unique<RedisQueryResultPromise>();
}

RedisQueryResultFuture RedisTask::getFuture()
{
	return m_result->get_future();
}

void RedisTask::execute()
{
	RedisResultPtr result = m_redisModule->exeCmd(m_client, m_command);
	m_result->set_value(result);
}


}