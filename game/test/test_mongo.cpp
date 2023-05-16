#include "test.h"
#include "db_mongo/mongo_module.h"
using namespace zq;


void testInsert(MongoModule& m)
{
	BsonObjectPtr insertor = std::make_shared<BsonObject>();
	insertor->appendString("test_insert1", "abcdef");
	insertor->appendInt32("test_insert2", 1234567);
	std::string dbName = "zq";
	std::string coName = "player122";

	m.insert(dbName, coName, insertor).start([](async_simple::Try<MongoResultPtr> v) {
		try
		{
			MongoResultPtr result = v.value();
			if (!result->success)
			{
				LOG_ERROR("[mongo test]", "insert error: {}", result->errorMsg);
				return;
			}
			LOG_INFO("[mongo test]", "insert success!");
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("[mongo test]", "insert exception: {}", e.what());
		}
	});
}

void testRemove(MongoModule& m)
{
	BsonObjectPtr selector = std::make_shared<BsonObject>();
	selector->appendString("test_insert11", "abcdef1");
	std::string dbName = "zq";
	std::string coName = "player";

	m.remove(dbName, coName, selector).start([](async_simple::Try<MongoResultPtr> v) {
		try
		{
			MongoResultPtr result = v.value();
			if (!result->success)
			{
				LOG_ERROR("[mongo test]", "remove error: {}", result->errorMsg);
				return;
			}
			LOG_INFO("[mongo test]", "remove success!");
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("[mongo test]", "remove exception: {}", e.what());
		}
	});
}

void testSave(MongoModule& m)
{
	BsonObjectPtr selector = std::make_shared<BsonObject>();
	BsonObjectPtr updatetor = std::make_shared<BsonObject>();
	selector->appendString("test_save34", "abcde");
	updatetor->appendString("test_save35", "abcdef");
	std::string dbName = "zq";
	std::string coName = "player";

	m.save(dbName, coName, selector, updatetor).start([](async_simple::Try<MongoResultPtr> v) {
		try
		{
			MongoResultPtr result = v.value();
			if (!result->success)
			{
				LOG_ERROR("[mongo test]", "save error: {}", result->errorMsg);
				return;
			}
			LOG_INFO("[mongo test]", "save success!");
		}
		catch (const std::exception& e)
		{
			LOG_ERROR("[mongo test]", "save exception: {}", e.what());
		}
	});
}

void testFind(MongoModule& m)
{
	{
		std::vector<BsonObject> result;
		BsonObjectPtr selector = std::make_shared<BsonObject>();
		std::string dbName = "zq";
		std::string coName = "player";

		m.find(dbName, coName, selector).start([](async_simple::Try<MongoResultPtr> v) {
			MongoResultPtr result = v.value();
			if (!result->success)
			{
				LOG_ERROR("[mongo test]", "find error: {}", result->errorMsg);
				return;
			}
			LOG_INFO("[mongo test]", "find success, result num:{}", result->foundResult.size());
			for (auto& obj : result->foundResult)
			{
				LOG_INFO(s_logCategory, "details:{}", obj->debugPrint());
			}
		});
	}

	{
		std::vector<BsonObject> result;
		BsonObjectPtr selector = std::make_shared<BsonObject>();
		selector->appendString("test_save1", "123333");
		std::string dbName = "zq";
		std::string coName = "player";

		m.find(dbName, coName, selector).start([](async_simple::Try<MongoResultPtr> v) {
			MongoResultPtr result = v.value();
			if (!result->success)
			{
				LOG_ERROR("[mongo test]", "find error: {}", result->errorMsg);
				return;
			}
			LOG_INFO("[mongo test]", "find success, result num:{}", result->foundResult.size());
			for (auto& obj : result->foundResult)
			{
				LOG_INFO(s_logCategory, "details:{}", obj->debugPrint());
			}
		});
	}
}

bool testMongo()
{
	std::string user = "null";
	std::string pwd = "null";
	std::string host = "127.0.0.1";
	uint16_t port = 27017;

	std::vector<std::pair<std::string, std::string>> collections{
		{ "zq", "player" },
	};
	MongoModule m(user, pwd, host, port, collections);
	m.init();

	testInsert(m);

	while (1)
	{
		m.update();
	}

	return true;
}