#include "db_redis/redis_module.h"
#include "db_redis/redis_commond.hpp"
#include "db_redis/redis_task.h"

#ifdef PLATFORM_WIN
#pragma warning(disable : 4200) // hiredis lib: nonstandard extension used: zero-sized array in struct/union
#endif

#include <hiredis/hiredis.h>


namespace zq{


#define GET_NAME(functionName) (#functionName)

//----------------------------string--------------------------------------
async_simple::coro::Lazy<bool> RedisModule::APPEND(const std::string &key, const std::string &value, int& length)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(APPEND));
    cmd << key;
    cmd << value;
	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		length = (int)result->integer;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::DECR(const std::string& key, int64_t& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(DECR));
    cmd << key;
	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}
	if (result->type == REDIS_REPLY_INTEGER)
	{
		value = result->integer;
		
		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::DECRBY(const std::string& key, const int64_t decrement, int64_t& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(DECRBY));
	cmd << key;
	cmd << decrement;
	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		value = result->integer;

		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::GETSET(const std::string& key, const std::string& value, std::string& oldValue)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(GETSET));
	cmd << key;
	cmd << value;
	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_STRING)
	{
		oldValue = result->str;

		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::INCR(const std::string& key, int64_t& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(INCR));
	cmd << key;
	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result->type == REDIS_REPLY_INTEGER)
	{
		value = result->integer;

		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::INCRBY(const std::string& key, const int64_t increment, int64_t& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(INCRBY));
	cmd << key;
	cmd << increment;
	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result->type == REDIS_REPLY_INTEGER)
	{
		value = result->integer;

		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::INCRBYFLOAT(const std::string& key, const float increment, float& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(INCRBYFLOAT));
	cmd << key;
	cmd << increment;
	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	bool success = false;
	if (result->type == REDIS_REPLY_STRING)
	{
		try
		{
			value = std::stof(result->str);
			success = true;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR(s_logCategory, "INCRBYFLOAT exception:{}.", e.what())
		}
	}

	co_return success;
}

async_simple::coro::Lazy<bool> RedisModule::SETEX(const std::string& key, const std::string& value, int time)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SETEX));
	cmd << key;
	cmd << value;
	cmd << time;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::SETNX(const std::string& key, const std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SETNX));
	cmd << key;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		success = (bool)result->integer;
	}

	co_return success;
}

async_simple::coro::Lazy<bool> RedisModule::STRLEN(const std::string& key, int& length)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(STRLEN));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		length = (int)result->integer;

		co_return length != 0;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::SET(const std::string &key, const std::string &value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory,"cant found redis client!, key:{}", key);
		co_return false;
	}

    RedisCommand cmd(GET_NAME(SET));
    cmd << key;
    cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::GET(const std::string& key, std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

    RedisCommand cmd(GET_NAME(GET));
    cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_STRING)
	{
		value = std::string(result->str, result->len);	
		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::MSET(const StringPairVector& values)
{
	if (m_clusterEnabled)
	{
		LOG_ERROR(s_logCategory, "MSET not supportted on cluster mode!");
		co_return false;
	}

	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!");
		co_return false;
	}

	RedisCommand cmd(GET_NAME(MSET));
	for (size_t i = 0; i < values.size(); ++i)
	{
		cmd << values[i].first;
		cmd << values[i].second;
	}

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	// always OK since MSET can't fail.
	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::MGET(const std::vector<std::string>& keys, std::vector<std::string>& values)
{
	if (m_clusterEnabled)
	{
		LOG_ERROR(s_logCategory, "MGET not supportted on cluster mode!");
		co_return false;
	}

	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!");
		co_return false;
	}

	RedisCommand cmd(GET_NAME(MGET));
	for (size_t i = 0; i < keys.size(); ++i)
	{
		cmd << keys[i];
	}

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < result->elements; k++)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				values.emplace_back(std::string(result->element[k]->str, result->element[k]->len));
			}
		}

		co_return true;
	}

	co_return false;
}

//----------------------------string  end--------------------------------------



//----------------------------hash-------------------------------------
async_simple::coro::Lazy<int> RedisModule::HDEL(const std::string& key, const std::string& field)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HDEL));
	cmd << key;
	cmd << field;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<int> RedisModule::HDEL(const std::string& key, const std::vector<std::string>& fields)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HDEL));
	cmd << key;
	for (auto it = fields.begin(); it != fields.end(); ++it)
	{
		cmd << *it;
	}

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<bool> RedisModule::HEXISTS(const std::string& key, const std::string& field)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HEXISTS));
	cmd << key;
	cmd << field;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool exist = false;
	if (result->type == REDIS_REPLY_INTEGER && result->integer == 1)
	{
		exist = true;
	}

	co_return exist;
}

async_simple::coro::Lazy<bool> RedisModule::HGET(const std::string& key, const std::string& field, std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HGET));
	cmd << key;
	cmd << field;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_STRING)
	{
		value = std::string(result->str, result->len);
		co_return true;
	}
	else
	{
		co_return false;
	}
}

async_simple::coro::Lazy<bool> RedisModule::HGETALL(const std::string& key, std::vector<StringPair>& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HGETALL));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)result->elements; k = k + 2)
		{
			values.emplace_back(StringPair{ std::string(result->element[k]->str, result->element[k]->len),
					std::string(result->element[k + 1]->str, result->element[k + 1]->len) });
		}

		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::HINCRBY(const std::string& key, const std::string& field, const int by, int64_t& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HINCRBY));
	cmd << key;
	cmd << field;
	cmd << by;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		value = result->integer;

		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::HINCRBYFLOAT(const std::string& key, const std::string& field, const float by, float& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HINCRBYFLOAT));
	cmd << key;
	cmd << field;
	cmd << by;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (result->type == REDIS_REPLY_STRING)
	{
		try
		{
			value = std::stof(result->str);
			success = true;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR(s_logCategory, "HINCRBYFLOAT exception:{}.", e.what())
		}
	}

	co_return success;
}

async_simple::coro::Lazy<bool> RedisModule::HKEYS(const std::string& key, std::vector<std::string>& fields)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HKEYS));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)result->elements; k++)
		{
			fields.emplace_back(std::string(result->element[k]->str, result->element[k]->len));
		}

		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::HLEN(const std::string& key, int& number)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HLEN));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		number = (int)result->integer;
		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::HMGET(const std::string& key, const std::vector<std::string>& fields, std::vector<std::string>& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HMGET));
	cmd << key;
	for (size_t i = 0; i < fields.size(); ++i)
	{
		cmd << fields[i];
	}

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)result->elements; k++)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				values.emplace_back(std::string(result->element[k]->str, result->element[k]->len));
			}
		}

		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::HMSET(const std::string& key, const std::vector<StringPair>& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HMSET));
	cmd << key;
	for (size_t i = 0; i < values.size(); ++i)
	{
		cmd << values[i].first;
		cmd << values[i].second;
	}

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::HMSET(const std::string& key, const std::vector<std::string>& fields, const std::vector<std::string>& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	if (fields.size() != values.size())
	{
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HMSET));
	cmd << key;
	for (size_t i = 0; i < values.size(); ++i)
	{
		cmd << fields[i];
		cmd << values[i];
	}

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::HSET(const std::string& key, const std::string& field, const std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HSET));
	cmd << key;
	cmd << field;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		success = (bool)result->integer;
	}

	co_return success;
}

async_simple::coro::Lazy<bool> RedisModule::HSETNX(const std::string &key, const std::string &field, const std::string &value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HSETNX));
	cmd << key;
	cmd << field;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		success = (bool)result->integer;
	}

	co_return success;
}

async_simple::coro::Lazy<bool> RedisModule::HVALS(const std::string& key, std::vector<std::string>& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HVALS));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)result->elements; k++)
		{
			values.emplace_back(std::string(result->element[k]->str, result->element[k]->len));
		}
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::HSTRLEN(const std::string &key, const std::string &field, int& length)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(HSTRLEN));
	cmd << key;
	cmd << field;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		length = (int)result->integer;
		co_return true;
	}

	co_return false;
}

//----------------------------hash end-------------------------------------



//----------------------------key--------------------------------------
async_simple::coro::Lazy<int> RedisModule::DEL(const std::string &key)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(DEL));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	int num = false;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<bool> RedisModule::EXISTS(const std::string &key)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(EXISTS));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool exist = false;
	if (REDIS_REPLY_INTEGER == result->type && 1 == result->integer)
	{
		exist = true;
	}

	co_return exist;
}

async_simple::coro::Lazy<bool> RedisModule::EXPIRE(const std::string &key, const unsigned int secs)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(EXPIRE));
	cmd << key;
	cmd << secs;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == result->type && 1 == result->integer)
	{
		success = true;
	}

	co_return success;
}

async_simple::coro::Lazy<bool> RedisModule::EXPIREAT(const std::string &key, const int64_t unixTime)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(EXPIREAT));
	cmd << key;
	cmd << unixTime;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == result->type && 1 == result->integer)
	{
		success = true;
	}

	co_return success;
}

async_simple::coro::Lazy<bool> RedisModule::PERSIST(const std::string &key)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(PERSIST));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == result->type && 1 == result->integer)
	{
		success = true;
	}

	co_return success;
}

async_simple::coro::Lazy<int> RedisModule::TTL(const std::string& key)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(TTL));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int leftTime = -1;
	if (REDIS_REPLY_INTEGER == result->type)
	{
		leftTime = (int)result->integer;
	}

	co_return leftTime;
}

//----------------------------key end--------------------------------------


//----------------------------list--------------------------------------
async_simple::coro::Lazy<bool> RedisModule::LINDEX(const std::string &key, const int index, std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(LINDEX));
	cmd << key;
	cmd << index;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_STRING)
	{
		value = std::string(result->str, result->len);
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::LLEN(const std::string &key, int& length)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(LLEN));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		length = (int)result->integer;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::LPOP(const std::string &key, std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(LPOP));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_STRING)
	{
		value = std::string(result->str, result->len);
	}

	co_return true;
}

async_simple::coro::Lazy<int> RedisModule::LPUSH(const std::string& key, const std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(LPUSH));
	cmd << key;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<int> RedisModule::LPUSHX(const std::string& key, const std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(LPUSHX));
	cmd << key;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<bool> RedisModule::LRANGE(const std::string& key, const int start, const int end, std::vector<std::string>& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	if (end - start <= 0)
	{
		co_return false;
	}

	RedisCommand cmd(GET_NAME(LRANGE));
	cmd << key;
	cmd << start;
	cmd << end;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)result->elements; k++)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				values.emplace_back(std::string(result->element[k]->str, result->element[k]->len));
			}
		}
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::LSET(const std::string &key, const int index, const std::string &value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(LSET));
	cmd << key;
	cmd << index;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::RPOP(const std::string& key, std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(RPOP));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_STRING)
	{
		value = std::string(result->str, result->len);
	}

	co_return true;
}

async_simple::coro::Lazy<int> RedisModule::RPUSH(const std::string& key, const std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(RPUSH));
	cmd << key;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<int> RedisModule::RPUSHX(const std::string& key, const std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(RPUSHX));
	cmd << key;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

//----------------------------list end--------------------------------------


//----------------------------pubsub--------------------------------------
async_simple::coro::Lazy<bool> RedisModule::PUBLISH(const std::string& key, const std::string& value)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(PUBLISH));
	cmd << key;
	cmd << value;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::SUBSCRIBE(const std::string& key)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SUBSCRIBE));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::UNSUBSCRIBE(const std::string& key)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(UNSUBSCRIBE));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

//----------------------------pubsub end--------------------------------------


//----------------------------server--------------------------------------
async_simple::coro::Lazy<bool> RedisModule::FLUSHALL()
{
	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!");
		co_return false;
	}

	RedisCommand cmd(GET_NAME(FLUSHALL));

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::FLUSHDB()
{
	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!");
		co_return false;
	}

	RedisCommand cmd(GET_NAME(FLUSHDB));

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

bool RedisModule::AUTH(redisContext* conn, const std::string& auth)
{
	if (conn == nullptr || auth.empty())
	{
		return false;
	}

	std::string strCmd = "AUTH " + auth;
	redisReply* result = (redisReply*)redisCommand(conn, strCmd.c_str());
	if (result == nullptr)
	{
		return false;
	}

	if (result->type == REDIS_REPLY_STATUS)
	{
		if (std::string("OK") == std::string(result->str, result->len) ||
			std::string("ok") == std::string(result->str, result->len))
		{
			freeReplyObject(result);
			return true;
		}
	}

	freeReplyObject(result);
	return false;
}

async_simple::coro::Lazy<bool> RedisModule::SELECTDB()
{
	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!");
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SELECTDB));

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::INFO(const std::string& param, std::string& outstr)
{
	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!");
		co_return false;
	}

	RedisCommand cmd(GET_NAME(INFO));
	cmd << param;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_STRING)
	{
		outstr = result->str;
	}

	co_return true;
}
//----------------------------end--------------------------------------


//----------------------------set--------------------------------------
async_simple::coro::Lazy<int> RedisModule::SADD(const std::string& key, const std::string& member)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SADD));
	cmd << key;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<bool> RedisModule::SCARD(const std::string& key, int& nCount)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SCARD));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		nCount = (int)result->integer;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::SISMEMBER(const std::string& key, const std::string& member)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SISMEMBER));
	cmd << key;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER && result->integer == 1)
	{
		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::SMEMBERS(const std::string& key, std::vector<std::string>& output)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SMEMBERS));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < result->elements; k++)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				output.emplace_back(std::string(result->element[k]->str, result->element[k]->len));
			}
		}
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::SMOVE(const std::string& source_key, const std::string& dest_key, const std::string& member)
{
	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, source_key:{}", source_key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SMOVE));
	cmd << source_key;
	cmd << dest_key;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		co_return (bool) result->integer;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::SPOP(const std::string& key, std::string& output)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SPOP));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_STRING)
	{
		output = std::string(result->str, result->len);
		co_return true;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::SRANDMEMBER(const std::string& key, int count, std::vector<std::string>& output)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SRANDMEMBER));
	cmd << key;
	cmd << count;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < result->elements; k++)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				output.emplace_back(std::string(result->element[k]->str, result->element[k]->len));
			}
		}
	}

	co_return true;
}

async_simple::coro::Lazy<int> RedisModule::SREM(const std::string& key, const std::vector<std::string>& members)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SREM));
	cmd << key;
	for (const auto& member : members)
	{
		cmd << member;
	}

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<bool> RedisModule::SUNION(const std::string& union_key1, const std::string& union_key2, std::vector<std::string>& output)
{
	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, union_key1:{}", union_key1);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SUNION));
	cmd << union_key1;
	cmd << union_key2;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < result->elements; k++)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				output.emplace_back(std::string(result->element[k]->str, result->element[k]->len));
			}
		}
	}

	co_return true;
}

async_simple::coro::Lazy<int> RedisModule::SUNIONSTORE(const std::string& dest_store_key, const std::string& union_key1, const std::string& union_key2)
{
	RedisClient* client = getConnection();
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, dest_store_key:{}", dest_store_key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(SUNIONSTORE));
	cmd << dest_store_key;
	cmd << union_key1;
	cmd << union_key2;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}
//----------------------------set end--------------------------------------


//----------------------------sort--------------------------------------
async_simple::coro::Lazy<int> RedisModule::ZADD(const std::string& key, const std::string& member, const double score)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZADD));
	cmd << key;
	cmd << score;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return -1;
	}

	int num = 0;
	if (result->type == REDIS_REPLY_INTEGER)
	{
		num = (int)result->integer;
	}

	co_return num;
}

async_simple::coro::Lazy<bool> RedisModule::ZCARD(const std::string& key, int &nCount)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZCARD));
	cmd << key;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		nCount = (int)result->integer;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::ZCOUNT(const std::string& key, const double start, const double end, int& nCount)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZCOUNT));
	cmd << key;
	cmd << start;
	cmd << end;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		nCount = (int)result->integer;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::ZINCRBY(const std::string& key, const std::string& member, const double score, double& newScore)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << score;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (result->type == REDIS_REPLY_STRING)
	{
		try
		{
			newScore = std::stod(result->str);
			success = true;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR(s_logCategory, "ZINCRBY exception:{}.", e.what())
		}
	}

	co_return success;
}

async_simple::coro::Lazy<bool> RedisModule::ZRANGE(const std::string& key, const int start, const int end, StringScoreVector& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZRANGE));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < result->elements; k = k + 2)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				StringScorePair vecPair;
				vecPair.first = std::string(result->element[k]->str, result->element[k]->len);
				try
				{
					vecPair.second = std::stod(result->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
				catch (const std::exception& e)
				{
					LOG_ERROR(s_logCategory, "ZRANGE exception:{}.", e.what());
					values.clear();
					co_return false;
				}
			}
		}
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::ZRANGEWITHSCORE(const std::string& key, const int start, const int end, StringScoreVector& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < result->elements; k = k + 2)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				StringScorePair vecPair;
				vecPair.first = std::string(result->element[k]->str, result->element[k]->len);
				try
				{
					vecPair.second = std::stod(result->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
				catch (const std::exception& e)
				{
					LOG_ERROR(s_logCategory, "ZRANGEBYSCORE exception:{}.", e.what());
					values.clear();
					co_return false;
				}
			}
		}
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::ZRANK(const std::string & key, const std::string & member, int& rank)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZRANK));
	cmd << key;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		rank = (int)result->integer;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::ZREM(const std::string & key, const std::string & member)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZREM));
	cmd << key;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		co_return (bool)result->integer;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::ZREMRANGEBYRANK(const std::string & key, const int start, const int end)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZREMRANGEBYRANK));
	cmd << key;
	cmd << start;
	cmd << end;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		co_return (bool)result->integer;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::ZREMRANGEBYSCORE(const std::string & key, const double start, const double end)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZREMRANGEBYSCORE));
	cmd << key;
	cmd << start;
	cmd << end;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		co_return (bool)result->integer;
	}

	co_return false;
}

async_simple::coro::Lazy<bool> RedisModule::ZREVRANGE(const std::string& key, const int start, const int end, StringScoreVector& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZREVRANGE));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < result->elements; k = k + 2)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				StringScorePair vecPair;
				vecPair.first = std::string(result->element[k]->str, result->element[k]->len);
				try
				{
					vecPair.second = std::stod(result->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
				catch (const std::exception& e)
				{
					LOG_ERROR(s_logCategory, "ZREVRANGE exception:{}.", e.what());
					values.clear();
					co_return false;
				}
			}
		}
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, StringScoreVector& values)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZREVRANGEBYSCORE));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < result->elements; k = k + 2)
		{
			if (result->element[k]->type == REDIS_REPLY_STRING)
			{
				StringScorePair vecPair;
				vecPair.first = std::string(result->element[k]->str, result->element[k]->len);
				try
				{
					vecPair.second = std::stod(result->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
				catch (const std::exception& e)
				{
					LOG_ERROR(s_logCategory, "ZREVRANGEBYSCORE exception:{}.", e.what());
					values.clear();
					co_return false;
				}
			}
		}
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::ZREVRANK(const std::string& key, const std::string& member, int& rank)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZREVRANK));
	cmd << key;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
			}));
		});

	if (result == nullptr)
	{
		co_return false;
	}

	if (result->type == REDIS_REPLY_INTEGER)
	{
		rank = (int)result->integer;
	}

	co_return true;
}

async_simple::coro::Lazy<bool> RedisModule::ZSCORE(const std::string& key, const std::string& member, double& score)
{
	RedisClient* client = getConnection(key);
	if (client == nullptr)
	{
		LOG_ERROR(s_logCategory, "cant found redis client!, key:{}", key);
		co_return false;
	}

	RedisCommand cmd(GET_NAME(ZSCORE));
	cmd << key;
	cmd << member;

	std::string strCmd = cmd.serialize();
	std::shared_ptr<RedisTask> task = std::make_shared<RedisTask>(this, *client, strCmd);

	CallbackAwaitor<RedisResultPtr> awaitor;
	RedisResultPtr result = co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](RedisResultPtr result) {
			handler.setValueThenResume(result);
			}));
		});

	if (result == nullptr)
	{
		co_return false;
	}

	bool success = false;
	if (result->type == REDIS_REPLY_STRING)
	{
		try
		{
			score = std::stod(result->str);
			success = true;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR(s_logCategory, "ZSCORE exception:{}.", e.what())
		}
	}

	co_return success;
}
//----------------------------sort end--------------------------------------



bool RedisModule::PING(RedisClient& client)
{
	if (client.conn == nullptr || !client.connected)
	{
		return false;
	}

	redisReply* result = (redisReply*)redisCommand(client.conn, "PING");
	if (result == nullptr)
	{
		return false;
	}

	client.lastActiveTime = time(nullptr);
	if (result->type == REDIS_REPLY_STATUS)
	{
		if (strcmp(result->str, "PONG") == 0)
		{
			freeReplyObject(result);
			return true;
		}
	}

	freeReplyObject(result);
	return false;
}

}
