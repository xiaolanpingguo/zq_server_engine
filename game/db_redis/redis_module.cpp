#include "db_redis/redis_module.h"

#ifdef PLATFORM_WIN
#include <winsock2.h> /* For struct timeval */
#endif

#include <hiredis/hiredis.h>


namespace zq{


RedisModule::RedisModule(const std::string& user, const std::string& pwd, const std::string& host, uint16_t port) :
		m_threadStop(false),
		m_thr(std::bind(&RedisModule::taskThrad, this)),
		m_redisContext(nullptr),
		m_user(user),
		m_pwd(pwd),
		m_host(host),
		m_port(port),
		m_clusterEnabled(false)
{

}

RedisModule::~RedisModule()
{
}

bool RedisModule::init()
{
	if (!initRedis())
	{
		return false;
	}

	return true;
}

bool RedisModule::update()
{
	processCallbacks();
	return true;
}

bool RedisModule::finalize()
{
	m_threadStop = true;
	m_thr.join();
	redisFree(m_redisContext);
	return true;
}

bool RedisModule::initRedis()
{
	bool connected = false;
	redisContext* c = nullptr;
	for (int i = 0; i < 3; ++i)
	{
		// connect
		struct timeval timeout = { 1, 500000 }; // 1.5 seconds
		c = redisConnectWithTimeout(m_host.c_str(), m_port, timeout);
		if (c != nullptr && c->err == 0)
		{
			connected = true;
			break;
		}

		if (c == nullptr)
		{
			LOG_ERROR(s_logCategory, "Connection error: can't allocate redis context\n");
			continue;
		}

		if (c->err != 0)
		{
			LOG_ERROR(s_logCategory, "Connection error: {}\n", c->errstr);
			redisFree(c);
		}
	}

	if (!connected)
	{
		return false;
	}

	// auth
	if (!AUTH(m_pwd))
	{
		LOG_ERROR(s_logCategory, "redis authorization faild!");
		redisFree(c);
		return false;
	}

	m_redisContext = c;

	// we not using cluster mode
	int r = checkClusterEnabled();
	if (r == -1)
	{
		LOG_ERROR(s_logCategory, "check cluster mode error!");
		return false;
	}
	if (r == 1)
	{
		//for (size_t i = 0; i < m_clients.size(); i++)
		//{
		//	m_clients.at(i) = m_redisContext;
		//}

		LOG_INFO(s_logCategory, "redis has setup with standalone mode.");
		return true;
	}

	// we are using redis cluster
	m_clusterEnabled = true;
	LOG_INFO(s_logCategory, "redis has setup with cluster mode.");
	if (getRedisClusterInfo() != 0)
	{
		LOG_ERROR(s_logCategory, "get redis cluster faild!");
		return false;
	}

	return true;
}

int RedisModule::checkClusterEnabled()
{
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, "INFO");
	if (reply == nullptr)
	{
		LOG_ERROR(s_logCategory, "send INFO command error!");
		return -1;
	}

	if (reply->type == REDIS_REPLY_STRING)
	{
		char* ptr = std::strstr(reply->str, "cluster_enabled:1");
		if (ptr)
		{
			freeReplyObject(reply);
			return 0;
		}
	}

	freeReplyObject(reply);
	return 1;
}

int RedisModule::getRedisClusterInfo()
{
	// todo 
	redisReply* reply = (redisReply*)redisCommand(m_redisContext, "CLUSTER SHARDS");
	if (reply == nullptr)
	{
		LOG_ERROR(s_logCategory, "send CLUSTER SHARDS command error!");
		return -1;
	}

	if (reply->type != REDIS_REPLY_ARRAY)
	{
		freeReplyObject(reply);
		LOG_ERROR(s_logCategory, "send CLUSTER SHARDS command type error;{}", reply->type);
		return -1;
	}

	for (int i = 0; i < reply->elements; i++)
	{
		redisReply* ptrCluster = reply->element[i];
		if (ptrCluster == nullptr || ptrCluster->type != REDIS_REPLY_ARRAY)
		{
			continue;
		}

		for (int j = 0; j < ptrCluster->elements; j++)
		{
			redisReply* ptrNode = ptrCluster->element[j];
			if (ptrNode == nullptr)
			{
				continue;
			}

			// slots: begin and end
			if (REDIS_REPLY_INTEGER == ptrNode->type)
			{
				if (j == 0)
				{
					m_nodes[i].slot[i].begin = (int)ptrNode->integer;
				}
				else if (j == 1)
				{
					m_nodes[i].slot[i].end = (int)ptrNode->integer;
				}
			}
			// nodes
			else if (REDIS_REPLY_ARRAY == ptrNode->type)
			{
				if (ptrNode->elements == 0)
				{
					continue;
				}

				for (int k = 0; k < 2; k++)
				{
					redisReply* ptrHost = ptrNode->element[k];
					if (ptrHost == nullptr)
					{
						continue;
					}

					if (REDIS_REPLY_STRING == ptrHost->type)
					{
						m_nodes[i].ip = std::string(ptrHost->str, ptrHost->len);
					}
					else if (REDIS_REPLY_INTEGER == ptrHost->type)
					{
						m_nodes[i].port = (uint16_t)ptrHost->integer;
					}
				}

				break;
			}
		}
	}

	return 0;
}

bool RedisModule::setupRedisCluster()
{
	// todo 
	return true;
}

RedisResultPtr RedisModule::exeCmd(const std::string& cmd)
{
	int r = redisAppendFormattedCommand(m_redisContext, cmd.c_str(), cmd.size());
	if (r != REDIS_OK)
	{
		return nullptr;
	}

	redisReply* reply = nullptr;
	r = redisGetReply(m_redisContext, (void**)&reply);
	if (r != REDIS_OK || reply == nullptr)
	{
		return nullptr;
	}

	if (REDIS_REPLY_ERROR == reply->type)
	{
		freeReplyObject(reply);
		return nullptr;
	}

	return RedisResultPtr(reply, [](redisReply* r) { if (r) freeReplyObject(r); });
}

RedisQueryCallback RedisModule::addTask(RedisTask* task)
{
	QueryResultFuture result = task->getFuture();
	m_queue.push(task);
	return RedisQueryCallback(std::move(result));
}

RedisQueryCallback& RedisModule::addCallback(RedisQueryCallback&& query)
{
	m_callbacks.push(std::move(query));
	return m_callbacks.back();
}

void RedisModule::processCallbacks()
{
	while (!m_callbacks.empty())
	{
		RedisQueryCallback& callback = m_callbacks.front();
		if (callback.invokeIfReady())
		{
			m_callbacks.pop();
		}
	}
}

void RedisModule::taskThrad()
{
	while (1)
	{
		RedisTask* task;

		if (m_threadStop)
		{
			while (!m_queue.empty())
			{
				if (m_queue.pop(task))
				{
					task->execute();
					delete task;
					task = nullptr;
				}
			}

			break;
		}

		m_queue.waitAndPop(task);
		task->execute();
		delete task;
		task = nullptr;
	}
}

}