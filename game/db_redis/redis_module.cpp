#include "db_redis/redis_module.h"

#ifdef PLATFORM_WIN
#include <winsock2.h> /* For struct timeval */
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4200) // hiredis lib: nonstandard extension used: zero-sized array in struct/union
#endif

#include <hiredis/hiredis.h>


namespace zq{


RedisModule::RedisModule(const std::string& auth, const std::string& host, uint16_t port) :
		m_threadStop(false),
		m_thr(std::bind(&RedisModule::taskThrad, this)),
		m_redisContext(nullptr),
		m_auth(auth),
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
	if (m_auth != "null" && !AUTH(m_auth))
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
		LOG_INFO(s_logCategory, "redis has setup with standalone mode.");
		return true;
	}

	// using redis cluster
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
		char* ptr = strstr(reply->str, "cluster_enabled:1");
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

	for (int i = 0; i < (int)reply->elements; i++)
	{
		redisReply* shard = reply->element[i];
		if (shard == nullptr || shard->type != REDIS_REPLY_ARRAY)
		{
			continue;
		}

		std::vector<int> slotIndex;
		std::vector<RedisClusterNode> nodes;

		for (int j = 0; j < (int)shard->elements; j++)
		{
			redisReply* node = shard->element[j];
			if (node == nullptr)
			{ 
				continue;
			}

			if (node->type == REDIS_REPLY_STRING && node->str)
			{
				if (strcmp(node->str, "slots") == 0)
				{
					redisReply* ele = shard->element[j + 1];
					if (ele == nullptr || ele->type != REDIS_REPLY_ARRAY)
					{
						continue;
					}

					for (int k = 0; k < ele->elements; k++)
					{
						redisReply* slots = ele->element[k];
						if (slots == nullptr)
						{
							continue;
						}

						if (slots->type == REDIS_REPLY_INTEGER)
						{
							slotIndex.emplace_back((int)slots->integer);
						}
					}

					// we have got a slots by shard->element[j + 1]
					++j;
				}
				else if (strcmp(node->str, "nodes") == 0)
				{
					redisReply* ele = shard->element[j + 1];
					if (ele == nullptr || ele->type != REDIS_REPLY_ARRAY)
					{
						continue;
					}

					for (int k = 0; k < ele->elements; ++k)
					{
						RedisClusterNode node;

						redisReply* nodesEle = ele->element[k];
						if (nodesEle == nullptr || nodesEle->type != REDIS_REPLY_ARRAY)
						{
							continue;
						}

						for (int x = 0; x < nodesEle->elements; x += 2)
						{
							redisReply* key = nodesEle->element[x];
							redisReply* value = nodesEle->element[x + 1];
							if (key == nullptr || value == nullptr)
							{
								continue;
							}

							if (strcmp(key->str, "id") == 0 && value->type == REDIS_REPLY_STRING && value->str && value->len > 0)
							{
								std::string id = std::string(value->str, value->len);
								node.id = id;
							}
							else if (strcmp(key->str, "ip") == 0 && value->type == REDIS_REPLY_STRING && value->str && value->len > 0)
							{
								std::string ip = std::string(value->str, value->len);
								node.ip = ip;
							}
							else if (strcmp(key->str, "role") == 0 && value->type == REDIS_REPLY_STRING && value->str && value->len > 0)
							{
								std::string role = std::string(value->str, value->len);
								node.role = role;
							}
							else if (strcmp(key->str, "port") == 0 && value->type == REDIS_REPLY_INTEGER)
							{
								uint16_t port = (uint16_t)value->integer;
								node.port = port;
							}
						}

						nodes.emplace_back(node);
					}

					++j;
				}	
			}
		}

		std::sort(slotIndex.begin(), slotIndex.end());
		RedisClusterShard shardInfo;
		shardInfo.slots = std::move(slotIndex);
		shardInfo.nodes = std::move(nodes);
		m_redisClusterShards.emplace_back(shardInfo);
	}

	LOG_INFO(s_logCategory, "redis cluster info: ");
	for (size_t i = 0; i < m_redisClusterShards.size(); ++i)
	{
		const RedisClusterShard& shard = m_redisClusterShards[i];
		std::string slotsStr;
		for (size_t j = 0; j < shard.slots.size(); ++j)
		{
			slotsStr += std::to_string(shard.slots[j]);
			slotsStr += " ";
		}
		std::string nodesStr;
		for (size_t j = 0; j < shard.nodes.size(); ++j)
		{
			nodesStr.append("id: ");
			nodesStr.append(shard.nodes[j].id);
			nodesStr.append("  ");

			nodesStr.append("ip: ");
			nodesStr.append(shard.nodes[j].ip);
			nodesStr.append("  ");

			nodesStr.append("pord: ");
			nodesStr.append(std::to_string(shard.nodes[j].port));
			nodesStr.append("  ");

			nodesStr.append("role: ");
			nodesStr.append(shard.nodes[j].role);
			nodesStr.append("  ");
		}

		LOG_INFO(s_logCategory, "redis cluster shard:{}, slots index:{}, nodes:{}", i, slotsStr, nodesStr);
	}

	freeReplyObject(reply);

	return 0;
}

bool RedisModule::setupRedisCluster()
{
	// to do
	for (size_t i = 0; i < m_redisClusterShards.size(); ++i)
	{
		redisContext* conn = nullptr;
		const RedisClusterShard& shard = m_redisClusterShards[i];
		std::string slotsStr;
		for (size_t j = 0; j < shard.slots.size(); ++j)
		{
			slotsStr += std::to_string(shard.slots[j]);
			slotsStr += " ";
		}

		for (size_t j = 0; j < shard.nodes.size(); ++j)
		{
			std::string ip = shard.nodes[j].ip;
			int port = shard.nodes[j].port;
		}

	}

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
	RedisQueryResultFuture result = task->getFuture();
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