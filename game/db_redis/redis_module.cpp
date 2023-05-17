#include "db_redis/redis_module.h"

#ifdef PLATFORM_WIN
#include <winsock2.h> /* For struct timeval */
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4200) // hiredis lib: nonstandard extension used: zero-sized array in struct/union
#endif

#include <hiredis/hiredis.h>


namespace zq{


// for keep alive check
constexpr static int s_normalCheckInterval = 15;
constexpr static int s_excptionCheckInterval = 5;

// redis CRC16 implementation according to CCITT standards.doc: https://redis.io/docs/reference/cluster-spec/
static const uint16_t s_crc16tab[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

static uint16_t crc16(const char* buf, int len)
{
	int counter;
	uint16_t crc = 0;
	for (counter = 0; counter < len; counter++)
	{
		crc = (crc << 8) ^ s_crc16tab[((crc >> 8) ^ *buf++) & 0x00FF];
	}

	return crc;
}

RedisModule::RedisModule(const std::string& auth, const std::string& host, uint16_t port) :
		m_threadStop(false),
		m_auth(auth),
		m_host(host),
		m_port(port),
		m_pingCheckInterval(s_normalCheckInterval),
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

	m_taskThread = std::make_unique<std::thread>(std::bind(&RedisModule::taskThrad, this));
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
	m_queue.stopWait();
	if (m_taskThread && m_taskThread->joinable())
	{
		m_taskThread->join();
	}

	if (m_redisClient.conn)
	{
		redisFree(m_redisClient.conn);
	}

	return true;
}

bool RedisModule::initRedis()
{
	redisContext* c = makeConnection(m_host, m_port);
	if (c == nullptr)
	{
		LOG_ERROR(s_logCategory, "connect redis failed, host:{}:{}.", m_host, m_port);
		return false;
	}

	m_redisClient.conn = c;
	m_redisClient.connected = true;
	m_redisClient.lastActiveTime = time(nullptr);

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
	if (!setupRedisCluster())
	{
		LOG_ERROR(s_logCategory, "set redis cluster faild!");
		return false;
	}

	return true;
}

redisContext* RedisModule::makeConnection(const std::string& host, uint16_t port, int timeoutMs)
{
	struct timeval timeout = { timeoutMs / 1000000, timeoutMs % 1000000 };
	redisContext* c = redisConnectWithTimeout(host.c_str(), port, timeout);
	if (c == nullptr)
	{
		LOG_ERROR(s_logCategory, "Connection error: can't allocate redis context\n");
		return nullptr;
	}

	if (c->err != 0)
	{
		LOG_ERROR(s_logCategory, "Connection error: {}\n", c->errstr);
		redisFree(c);
		return nullptr;
	}

	// auth
	if (m_auth != "null" && !AUTH(c, m_auth))
	{
		LOG_ERROR(s_logCategory, "redis authorization faild!");
		redisFree(c);
		return nullptr;
	}

	return c;
}

RedisClient* RedisModule::getConnection()
{
	return &m_redisClient;
}

RedisClient* RedisModule::getConnection(const std::string& key)
{
	if (m_clusterEnabled)
	{
		return getClusterConnection(key);
	}
	else
	{
		return &m_redisClient;
	}
}

RedisClient* RedisModule::getClusterConnection(const std::string& key)
{
	uint16_t crc = crc16(key.c_str(), (int)key.size()) % s_maxRedisNodes;
	for (size_t i = 0; i < m_redisClusterShards.size(); ++i)
	{
		RedisClusterShard& shard = m_redisClusterShards[i];

		bool foundIndex = false;
		for (const auto& pair : shard.slots)
		{
			if (crc >= pair.first && crc <= pair.second)
			{
				foundIndex = true;
				break;
			}
		}

		if (!foundIndex)
		{
			continue;
		}

		RedisClient* masterClient = nullptr;
		RedisClient* replicaClient = nullptr;
		for (size_t j = 0; j < shard.nodes.size(); ++j)
		{
			if (shard.nodes[j].role == "master")
			{
				masterClient = &shard.nodes[j].client;
			}
			else if (shard.nodes[j].role == "replica")
			{
				replicaClient = &shard.nodes[j].client;
			}
		}

		// check the master node connection firstly
		if (masterClient && masterClient->conn && masterClient->connected)
		{
			return masterClient;
		}

		// the master node connection is down, then using replica connection
		if (replicaClient && replicaClient->conn && replicaClient->connected)
		{
			return replicaClient;
		}

		LOG_ERROR(s_logCategory, "the master and replica connection are down! crc:{}", crc);
		return nullptr;
	}

	LOG_ERROR(s_logCategory, "can't found any cluster connection by crc:{}", crc);
	return nullptr;
}

int RedisModule::checkClusterEnabled()
{
	redisReply* reply = (redisReply*)redisCommand(m_redisClient.conn, "INFO");
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

bool RedisModule::setupRedisCluster()
{
	if (getRedisClusterInfo() != 0)
	{
		LOG_ERROR(s_logCategory, "get redis cluster info failed!");
		return false;
	}

	if (!connectClusterNode())
	{
		return false;
	}

	return true;
}

int RedisModule::getRedisClusterInfo()
{
	redisReply* reply = (redisReply*)redisCommand(m_redisClient.conn, "CLUSTER SHARDS");
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
		if (slotIndex.size() % 2 != 0 || slotIndex.back() >= s_maxRedisNodes)
		{
			freeReplyObject(reply);
			LOG_INFO(s_logCategory, "slot size is not a multiple of 2, size:{}", slotIndex.size());
			return 1;
		}

		RedisClusterShard shardInfo;
		for (size_t j = 0; j < slotIndex.size(); j += 2)
		{
			shardInfo.slots.emplace_back(std::make_pair(slotIndex[j], slotIndex[j + 1]));
		}
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
			slotsStr.append(std::to_string(shard.slots[j].first));
			slotsStr.append(" ");
			slotsStr.append(std::to_string(shard.slots[j].second));
			slotsStr.append(" ");
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

			nodesStr.append("port: ");
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

bool RedisModule::connectClusterNode()
{
	for (size_t i = 0; i < m_redisClusterShards.size(); ++i)
	{
		RedisClusterShard& shard = m_redisClusterShards[i];

		for (size_t j = 0; j < shard.nodes.size(); ++j)
		{
			std::string ip = shard.nodes[j].ip;
			int port = shard.nodes[j].port;

			redisContext* c = makeConnection(ip, port);
			if (c == nullptr)
			{
				LOG_ERROR(s_logCategory, "connect redis cluster failed, id:{}, role:{}, host:{}:{}.", shard.nodes[j].id, shard.nodes[j].role, ip, port);
				return false;
			}

			shard.nodes[j].client.conn = c;
			shard.nodes[j].client.connected = true;
			shard.nodes[j].client.lastActiveTime = time(nullptr);
		}
	}

	return true;
}

void RedisModule::keepAlive()
{
	uint64_t now = time(nullptr);

	if (!m_clusterEnabled)
	{
		if (now - m_redisClient.lastActiveTime < m_pingCheckInterval)
		{
			return;
		}

		m_redisClient.lastActiveTime = now;
		if (!PING(m_redisClient))
		{
			LOG_ERROR(s_logCategory, "ping error! client has disconnted from redis:  host:{}:{}.", m_host, m_port);

			if (m_redisClient.conn)
			{
				redisFree(m_redisClient.conn);
				m_redisClient.conn = nullptr;
			}
			m_redisClient.connected = false;

			redisContext* c = makeConnection(m_host, m_port);
			if (c == nullptr)
			{
				LOG_ERROR(s_logCategory, "redis standalone, make a new connection failed, cannot connect to redis!");
				return;
			}

			m_redisClient.conn = c;
			m_redisClient.connected = true;
			m_redisClient.lastActiveTime = time(nullptr);

			m_pingCheckInterval = s_normalCheckInterval;
		}
	}
	else
	{
		for (size_t i = 0; i < m_redisClusterShards.size(); ++i)
		{
			RedisClusterShard& shard = m_redisClusterShards[i];
			for (size_t j = 0; j < shard.nodes.size(); ++j)
			{
				RedisClusterNode& node = shard.nodes[j];

				if (now - node.client.lastActiveTime < m_pingCheckInterval)
				{
					continue;
				}

				node.client.lastActiveTime = now;
				if (!PING(node.client))
				{
					LOG_ERROR(s_logCategory, "ping error! client has disconnted from redis cluster: id:{}, role:{}, host:{}:{}.", node.id, node.role, node.ip, node.port);

					if (node.client.conn)
					{
						redisFree(node.client.conn);
						node.client.conn = nullptr;
					}
					node.client.connected = false;
					redisContext* c = makeConnection(m_host, m_port);
					if (c == nullptr)
					{
						LOG_ERROR(s_logCategory, "redis cluster, make a new connection failed, cannot connect to redis!");
						continue;
					}

					node.client.conn = c;
					node.client.connected = true;
					node.client.lastActiveTime = time(nullptr);

					m_pingCheckInterval = s_normalCheckInterval;
				}
			}
		}
	}
}

RedisResultPtr RedisModule::exeCmd(RedisClient& client, const std::string& cmd)
{
	if (client.conn == nullptr)
	{
		LOG_ERROR(s_logCategory, "exe cmd error, conn == nullptr.");
		return nullptr;
	}

	if (!client.connected)
	{
		LOG_ERROR(s_logCategory, "exe cmd error, client has disconnted.");
		return nullptr;
	}

	int r = redisAppendFormattedCommand(client.conn, cmd.c_str(), cmd.size());
	if (r != REDIS_OK)
	{
		return nullptr;
	}

	// if REDIS_ERR returned, it maybe client has disconnted, just make ping to check it
	redisReply* reply = nullptr;
	r = redisGetReply(client.conn, (void**)&reply);
	if (r != REDIS_OK || reply == nullptr)
	{
		client.connected = false;
		m_pingCheckInterval = s_excptionCheckInterval;
		return nullptr;
	}

	client.lastActiveTime = time(nullptr);

	if (REDIS_REPLY_ERROR == reply->type)
	{
		LOG_ERROR(s_logCategory, "exe cmd error, cmd:{}, error:{}", cmd, std::string(reply->str, reply->len));
		freeReplyObject(reply);
		return nullptr;
	}

	return RedisResultPtr(reply, [](redisReply* r) { if (r) freeReplyObject(r); });
}

RedisQueryCallback RedisModule::addTask(std::shared_ptr<RedisTask> task)
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

void RedisModule::processTask()
{
	std::shared_ptr<RedisTask> task = nullptr;
	if (m_queue.pop(task))
	{
		task->execute();
	}
}

void RedisModule::taskThrad()
{
	while (!m_threadStop || !m_queue.empty())
	{
		keepAlive();
		processTask();
	}
}

}