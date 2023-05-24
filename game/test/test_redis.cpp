#include "test.h"
#include "db_redis/redis_module.h"
using namespace zq;


async_simple::coro::Lazy<void> testSet(RedisModule& m)
{
	co_await m.SET("key12", "abc");
	while (1)
	{
		std::string value;
		co_await m.GET("key12", value);
	
		LOG_INFO(s_logCategory, "redis get a value:{}.", value);

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	LOG_INFO(s_logCategory, "redis testSet finish.");
	co_return;
}

bool testRedis()
{
	std::string auth = "zq";
	std::string host = "192.168.0.109";
	uint16_t port = 7000;
	RedisModule m(auth, host, port);
	if (!m.init())
	{
		return false;
	}

	testSet(m).start([](auto&&){});
	while (1)
	{
		m.update(0);
	}

	return true;
}