#pragma once


#include "common/common.hpp"
#include "common/concurrent_queue.hpp"
#include "game_common/i_module.hpp"
#include "db_redis/redis_task.h"
#include "common/coroutine_awaitor.hpp"


struct redisContext;

namespace zq{


struct RedisClient
{
	redisContext* conn = nullptr;
	uint64_t lastActiveTime = 0;
	bool connected = false;
};

// for redis cluster
struct RedisClusterNode
{
	std::string id;
	std::string ip;
	std::string role;
	uint16_t port;
	RedisClient client;
};

struct RedisClusterShard
{
	std::vector<std::pair<int, int>> slots;
	std::vector<RedisClusterNode> nodes;
};


using StringPair = std::pair<std::string, std::string>;
using StringPairVector = std::vector<StringPair>;
using StringScorePair = std::pair<std::string, double>;
using StringScoreVector = std::vector<StringScorePair>;

class RedisTask;
class RedisModule : public IModule
{
	INIT_MODULE_NAME(RedisModule);

public:
	RedisModule(const std::string& auth, const std::string& host, uint16_t port);
	~RedisModule();

public:
	bool init() override;
	bool update() override;
	bool finalize() override;

	RedisResultPtr exeCmd(RedisClient& client, const std::string& cmd);

public:
	/////////   key  //////////////
	async_simple::coro::Lazy<bool> DEL(const std::string& key);

	async_simple::coro::Lazy<bool> EXISTS(const std::string& key);

	async_simple::coro::Lazy<bool> EXPIRE(const std::string& key, const unsigned int secs);

	async_simple::coro::Lazy<bool> EXPIREAT(const std::string& key, const int64_t unixTime);

	async_simple::coro::Lazy<bool> PERSIST(const std::string& key);

	async_simple::coro::Lazy<int> TTL(const std::string& key);

	/////////   string  //////////////
	async_simple::coro::Lazy<bool> APPEND(const std::string& key, const std::string& value, int& length);

	async_simple::coro::Lazy<bool> DECR(const std::string& key, int64_t& value);

	async_simple::coro::Lazy<bool> DECRBY(const std::string& key, const int64_t decrement, int64_t& value);

	async_simple::coro::Lazy<bool> GET(const std::string& key, std::string& value);

	async_simple::coro::Lazy<bool> GETSET(const std::string& key, const std::string& value, std::string& oldValue);

	async_simple::coro::Lazy<bool> INCR(const std::string& key, int64_t& value);

	async_simple::coro::Lazy<bool> INCRBY(const std::string& key, const int64_t increment, int64_t& value);

	async_simple::coro::Lazy<bool> INCRBYFLOAT(const std::string& key, const float increment, float& value);

	async_simple::coro::Lazy<bool> MSET(const StringPairVector& values);
	async_simple::coro::Lazy<bool> MGET(const std::vector<std::string>& keys, std::vector<std::string>& values);
	async_simple::coro::Lazy<bool> SET(const std::string& key, const std::string& value);

	async_simple::coro::Lazy<bool> SETEX(const std::string& key, const std::string& value, int time);

	async_simple::coro::Lazy<bool> SETNX(const std::string& key, const std::string& value);

	async_simple::coro::Lazy<bool> STRLEN(const std::string& key, int& length);

	/////////  hash  //////////////
	async_simple::coro::Lazy <int> HDEL(const std::string& key, const std::string& field);
	async_simple::coro::Lazy <int> HDEL(const std::string& key, const std::vector<std::string>& fields);

	async_simple::coro::Lazy<bool> HEXISTS(const std::string& key, const std::string& field);

	async_simple::coro::Lazy<bool> HGET(const std::string& key, const std::string& field, std::string& value);

	async_simple::coro::Lazy<bool> HGETALL(const std::string& key, std::vector<StringPair>& values);

	async_simple::coro::Lazy<bool> HINCRBY(const std::string& key, const std::string& field, const int by, int64_t& value);

	async_simple::coro::Lazy<bool> HINCRBYFLOAT(const std::string& key, const std::string& field, const float by, float& value);

	async_simple::coro::Lazy<bool> HKEYS(const std::string& key, std::vector<std::string>& fields);

	async_simple::coro::Lazy<bool> HLEN(const std::string& key, int& number);

	async_simple::coro::Lazy<bool> HMGET(const std::string& key, const std::vector<std::string>& fields, std::vector<std::string>& values);

	async_simple::coro::Lazy<bool> HMSET(const std::string& key, const std::vector<StringPair>& values);
	async_simple::coro::Lazy<bool> HMSET(const std::string& key, const std::vector<std::string>& fields, const std::vector<std::string>& values);

	async_simple::coro::Lazy<bool> HSET(const std::string& key, const std::string& field, const std::string& value);

	async_simple::coro::Lazy<bool> HSETNX(const std::string& key, const std::string& field, const std::string& value);

	async_simple::coro::Lazy<bool> HVALS(const std::string& key, std::vector<std::string>& values);

	async_simple::coro::Lazy<bool> HSTRLEN(const std::string& key, const std::string& field, int& length);

	///////// list //////////////

	async_simple::coro::Lazy<bool> LINDEX(const std::string& key, const int index, std::string& value);

	async_simple::coro::Lazy<bool> LLEN(const std::string& key, int& length);

	async_simple::coro::Lazy<bool> LPOP(const std::string& key, std::string& value);

	async_simple::coro::Lazy<int> LPUSH(const std::string& key, const std::string& value);

	async_simple::coro::Lazy<int> LPUSHX(const std::string& key, const std::string& value);

	async_simple::coro::Lazy<bool> LRANGE(const std::string& key, const int start, const int end, std::vector<std::string>& values);

	async_simple::coro::Lazy<bool> LSET(const std::string& key, const int index, const std::string& value);

	async_simple::coro::Lazy<bool> RPOP(const std::string& key, std::string& value);

	async_simple::coro::Lazy<int> RPUSH(const std::string& key, const std::string& value);

	async_simple::coro::Lazy<int> RPUSHX(const std::string& key, const std::string& value);

	/////////  set  //////////////

	async_simple::coro::Lazy<int> SADD(const std::string& key, const std::string& member);

	async_simple::coro::Lazy<bool> SCARD(const std::string& key, int& nCount);

	async_simple::coro::Lazy<bool> SISMEMBER(const std::string& key, const std::string& member);

	async_simple::coro::Lazy<bool> SMEMBERS(const std::string& key, std::vector<std::string>& output);

	async_simple::coro::Lazy<bool> SMOVE(const std::string& source_key, const std::string& dest_key, const std::string& member);

	async_simple::coro::Lazy<bool> SPOP(const std::string& key, std::string& output);

	async_simple::coro::Lazy<bool> SRANDMEMBER(const std::string& key, int count, std::vector<std::string>& output);

	async_simple::coro::Lazy<int> SREM(const std::string& key, const std::vector<std::string>& members);

	async_simple::coro::Lazy<bool> SUNION(const std::string& union_key1, const std::string& union_key2, std::vector<std::string>& output);

	async_simple::coro::Lazy<int> SUNIONSTORE(const std::string& dest_store_key, const std::string& union_key1, const std::string& union_key2);

	/////////  SortedSet //////////////

	async_simple::coro::Lazy<int> ZADD(const std::string& key, const std::string& member, const double score);

	async_simple::coro::Lazy<bool> ZCARD(const std::string& key, int& nCount);

	async_simple::coro::Lazy<bool> ZCOUNT(const std::string& key, const double start, const double end, int& nCount);

	async_simple::coro::Lazy<bool> ZINCRBY(const std::string& key, const std::string& member, const double score, double& newScore);

	async_simple::coro::Lazy<bool> ZRANGE(const std::string& key, const int start, const int end, StringScoreVector& values);

	async_simple::coro::Lazy<bool> ZRANGEBYSCORE(const std::string& key, const double start, const double end, StringScoreVector& values);

	async_simple::coro::Lazy<bool> ZRANK(const std::string& key, const std::string& member, int& rank);

	async_simple::coro::Lazy<bool> ZREM(const std::string& key, const std::string& member);

	async_simple::coro::Lazy<bool> ZREMRANGEBYRANK(const std::string& key, const int start, const int end);

	async_simple::coro::Lazy<bool> ZREMRANGEBYSCORE(const std::string& key, const double min, const double max);

	async_simple::coro::Lazy<bool> ZREVRANGE(const std::string& key, const int start, const int end, StringScoreVector& values);

	async_simple::coro::Lazy<bool> ZREVRANGEBYSCORE(const std::string& key, const double start, const double end, StringScoreVector& values);

	async_simple::coro::Lazy<bool> ZREVRANK(const std::string& key, const std::string& member, int& rank);

	async_simple::coro::Lazy<bool> ZSCORE(const std::string& key, const std::string& member, double& score);

	/////////  server  //////////////

	async_simple::coro::Lazy<bool> FLUSHALL();

	async_simple::coro::Lazy<bool> FLUSHDB();

	/////////  pubsub  //////////////
	async_simple::coro::Lazy<bool> PUBLISH(const std::string& key, const std::string& value);

	async_simple::coro::Lazy<bool> SUBSCRIBE(const std::string& key);

	async_simple::coro::Lazy<bool> UNSUBSCRIBE(const std::string& key);

	/////////  other  //////////////
	async_simple::coro::Lazy<bool> SELECTDB();

	async_simple::coro::Lazy<bool> INFO(const std::string& param, std::string& outstr);

private:
	bool AUTH(redisContext* conn, const std::string& auth);
	bool PING(RedisClient& client);


	bool initRedis();
	redisContext* makeConnection(const std::string& host, uint16_t port, int timeoutMs = 1500000);
	RedisClient* getConnection();
	RedisClient* getConnection(const std::string& key);
	RedisClient* getClusterConnection(const std::string& key);
	int checkClusterEnabled();
	bool setupRedisCluster();
	int getRedisClusterInfo();
	bool connectClusterNode();
	void keepAlive();

	RedisQueryCallback addTask(RedisTask* task);
	RedisQueryCallback& addCallback(RedisQueryCallback&& query);
	void processCallbacks();
	void processTask();
	void taskThrad();

private:

	std::atomic<bool> m_threadStop;
	std::unique_ptr<std::thread> m_taskThread;
	RedisClient m_redisClient;

	std::queue<RedisQueryCallback> m_callbacks;
	ConcurrentQueue<RedisTask*> m_queue;

	std::string m_auth;
	std::string m_host;
	uint16_t m_port;
	int m_pingCheckInterval;

	// for cluster
	bool m_clusterEnabled;
	constexpr static int s_maxRedisNodes = 16384;
	std::vector<RedisClusterShard> m_redisClusterShards;

	constexpr static std::string_view s_logCategory = "RedisModule";
};

}
