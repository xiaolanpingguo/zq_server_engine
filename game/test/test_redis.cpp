#include "test.h"
#include "db_redis/redis_module.h"
using namespace zq;


bool testRedis()
{
	std::string auth = "null";
	std::string host = "192.168.0.109";
	uint16_t port = 7001;
	RedisModule m(auth, host, port);
	m.init();

	while (1)
	{
		m.update();
	}

	return true;
}