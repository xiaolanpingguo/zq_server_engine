#include "db_mongo/mongo_module.h"
#include "game_common/game_db_def.hpp"

#include <bson/bson.h>


namespace zq{


MongoModule::MongoModule(const std::string& user, const std::string& pwd, const std::string& host, uint16_t port) :
		m_threadStop(false),
		m_thr(std::bind(&MongoModule::taskThrad, this)),
		m_mongoUrl(nullptr),
		m_mongoClient(nullptr),
		m_user(user),
		m_pwd(pwd),
		m_host(host),
		m_port(port)
{
}

MongoModule::~MongoModule()
{
}

bool MongoModule::init()
{
	if (!initMongo())
	{
		return false;
	}

	struct CollectionSetupData
	{
		std::string dbName;
		std::string collectionName;
	};

	std::vector<CollectionSetupData> allCollections{
		{ DB_NAME, COL_ACCOUNT },
		{ DB_NAME, COL_PLAYER },
		{ DB_NAME, COL_PLAYER_NAME },
	};
	for (const auto& col : allCollections)
	{
		if (!setupCollection(col.dbName, col.collectionName))
		{
			return false;
		}
	}

	{
		//testInsert();
		//testRemove();
		//testSave();
		//testFind();
	}

	return true;
}

bool MongoModule::update()
{
	processCallbacks();
	return true;
}

bool MongoModule::finalize()
{
	m_threadStop = true;
	m_thr.join();
	for (auto& pair : m_collections)
	{
		mongoc_collection_destroy(pair.second);
	}
	mongoc_uri_destroy(m_mongoUrl);
	mongoc_client_destroy(m_mongoClient);
	mongoc_cleanup();
	return true;
}

MongoQueryCallback MongoModule::addTask(MongoTask* task)
{
	MongoQueryResultFuture result = task->getFuture();
	m_queue.push(task);
	return MongoQueryCallback(std::move(result));
}

MongoQueryCallback& MongoModule::addCallback(MongoQueryCallback&& query)
{
	m_callbacks.push(std::move(query));
	return m_callbacks.back();
}

async_simple::coro::Lazy<MongoResultPtr> MongoModule::insert(const std::string& dbName, const std::string& collectionName, BsonObjectPtr insertor)
{
	MongoTask* task = new MongoInsertTask(this, dbName, collectionName, insertor);
	CallbackAwaitor<MongoResultPtr> awaitor;
	co_return co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](MongoResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});
}

async_simple::coro::Lazy<MongoResultPtr> MongoModule::remove(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector)
{
	MongoTask* task = new MongoRemoveTask(this, dbName, collectionName, selector);
	CallbackAwaitor<MongoResultPtr> awaitor;
	co_return co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](MongoResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});
}

async_simple::coro::Lazy<MongoResultPtr> MongoModule::save(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, BsonObjectPtr updator)
{
	MongoTask* task = new MongoSaveTask(this, dbName, collectionName, selector, updator);
	CallbackAwaitor<MongoResultPtr> awaitor;
	co_return co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](MongoResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});
}

async_simple::coro::Lazy<MongoResultPtr> MongoModule::find(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, int limit, int skip)
{
	MongoTask* task = new MongoFindTask(this, dbName, collectionName, selector, limit, skip);
	CallbackAwaitor<MongoResultPtr> awaitor;
	co_return co_await awaitor.awaitResume([this, task](auto handler) {
		addCallback(addTask(task).withCallback([handler](MongoResultPtr result) {
			handler.setValueThenResume(result);
		}));
	});
}

bool MongoModule::initMongo()
{
	if (m_user == "null")
	{
		m_url = fmt::format("mongodb://{}:{}", m_host, m_port);
	}
	else
	{
		m_url = fmt::format("mongodb://{}:{}@{}:{}/?{}", m_user, m_pwd, m_host, m_port, "authMechanism=SCRAM-SHA-1");
	}

	mongoc_init();
	m_mongoUrl = mongoc_uri_new(m_url.c_str());
	m_mongoClient = mongoc_client_new_from_uri(m_mongoUrl);
	mongoc_client_set_appname(m_mongoClient, "MongoModule");
	mongoc_log_set_handler(&MongoModule::mongoLog, this);

	return true;
}

bool MongoModule::setupCollection(const std::string& dbName, const std::string& collectionName)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (collection != nullptr)
	{
		LOG_ERROR(s_logCategory, "setup a duplicate collection: db:{}, collectionName:{}.\n", dbName, collectionName);
		return false;
	}

	collection = mongoc_client_get_collection(m_mongoClient, dbName.c_str(), collectionName.c_str());
	if (collection == nullptr)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return false;
	}

	m_collections[{ dbName, collectionName }] = collection;
	return true;
}

bool MongoModule::mongoInsert(const std::string& dbName, const std::string& collectionName, BsonObject& insertor, std::string& errorMsg)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (nullptr == collection)
	{
		errorMsg = fmt::format("[mongo insert]cannot find collection: dbName:{}, collectionName:{}.", dbName, collectionName);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		return false;
	}

	bson_error_t mongoError;
	bson_t bson;
	bson_init(&bson);
	insertor.convertToRawBson(bson);
	if (!mongoc_collection_insert_one(collection, &bson, nullptr, nullptr, &mongoError))
	{
		bson_destroy(&bson);
		errorMsg = fmt::format("insert mongo data error, dbName:{}, collectionName:{}, error:{}:{}:{}.", dbName, collectionName, mongoError.domain, mongoError.code, mongoError.message);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		return false;
	}

	bson_destroy(&bson);

	return true;
}

bool MongoModule::mongoRemove(const std::string& dbName, const std::string& collectionName, BsonObject& selector, std::string& errorMsg)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (nullptr == collection)
	{
		errorMsg = fmt::format("[mongo insert]cannot find collection: dbName:{}, collectionName:{}.", dbName, collectionName);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		return false;
	}

	bson_error_t mongoError;
	bson_t bsonSelector;
	bson_init(&bsonSelector);
	selector.convertToRawBson(bsonSelector);

	if (!mongoc_collection_delete_one(collection, &bsonSelector, nullptr, nullptr, &mongoError))
	{
		errorMsg = fmt::format("remove mongo data error, dbName:{}, collectionName:{}, error:{}:{}:{}.", dbName, collectionName, mongoError.domain, mongoError.code, mongoError.message);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		bson_destroy(&bsonSelector);
		return false;
	}

	bson_destroy(&bsonSelector);
	return true;
}

bool MongoModule::mongoSave(const std::string& dbName, const std::string& collectionName, BsonObject& selector, BsonObject& updator, std::string& errorMsg)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (nullptr == collection)
	{
		errorMsg = fmt::format("[mongo insert]cannot find collection: dbName:{}, collectionName:{}.", dbName, collectionName);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		return false;
	}

	if (selector.empty())
	{
		errorMsg = "selector cannot be empty, specify an empty document will replace the first document returned in the collection.";
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		return false;
	}

	bson_error_t mongoError;
	bson_t bsonSelector;
	bson_t bsonUpdateort;
	bson_t bsonOpt;
	bson_init(&bsonSelector);
	bson_init(&bsonUpdateort);
	bson_init(&bsonOpt);
	BSON_APPEND_BOOL(&bsonOpt, "upsert", 1);

	selector.convertToRawBson(bsonSelector);
	updator.convertToRawBson(bsonUpdateort);

	if (!mongoc_collection_replace_one(collection, &bsonSelector, &bsonUpdateort, &bsonOpt, nullptr, &mongoError))
	{
		errorMsg = fmt::format("save mongo data error, dbName:{}, collectionName:{}, error:{}:{}:{}.", dbName, collectionName, mongoError.domain, mongoError.code, mongoError.message);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		bson_destroy(&bsonOpt);
		bson_destroy(&bsonSelector);
		bson_destroy(&bsonUpdateort);
		return false;
	}

	bson_destroy(&bsonOpt);
	bson_destroy(&bsonSelector);
	bson_destroy(&bsonUpdateort);
	return true;
}

bool MongoModule::mongoFind(const std::string& dbName, const std::string& collectionName, BsonObject& selector, std::vector<BsonObjectPtr>& result, std::string& errorMsg, int limit, int skip)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (nullptr == collection)
	{
		errorMsg = fmt::format("[mongo insert]cannot find collection: dbName:{}, collectionName:{}.", dbName, collectionName);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		return false;
	}

	bson_t baseQuery;
	bson_t bsonOpt;
	bson_init(&baseQuery);
	bson_init(&bsonOpt);

	selector.convertToRawBson(baseQuery);

	BSON_APPEND_INT32(&bsonOpt, "limit", limit);
	BSON_APPEND_INT32(&bsonOpt, "skip", skip);

	int num = 0;
	bool success = false;
	do
	{
		mongoc_cursor_t* pCursor = mongoc_collection_find_with_opts(collection, &baseQuery, &bsonOpt, nullptr);
		if (pCursor == nullptr)
		{
			break;
		}

		const bson_t* doc;
		while (mongoc_cursor_next(pCursor, &doc))
		{
			if (doc != nullptr)
			{
				BsonObjectPtr bsonObj = std::make_shared<BsonObject>();
				bsonObj->convertFromRawBson(*doc);
				result.emplace_back(bsonObj);
			}
			num++;
		}

		mongoc_cursor_destroy(pCursor);

		success = true;
	}
	while (0);

	bson_destroy(&bsonOpt);
	bson_destroy(&baseQuery);

	if (!success)
	{
		errorMsg = fmt::format("find mongo data error, dbName:{}, collectionName:{}.", dbName, collectionName);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		return false;
	}

	return true;
}

bool MongoModule::mongoBatchFind(const std::string& dbName, const std::string& collectionName)
{
	return true;
}

mongoc_collection_t* MongoModule::getCollection(const std::string& dbName, const std::string& collectionName)
{
	auto pair = std::make_pair(dbName, collectionName);
	auto it = m_collections.find(pair);
	if (it == m_collections.end())
	{
		return nullptr;
	}

	return it->second;
}

void MongoModule::mongoLog(mongoc_log_level_t logLevel, const char* logDomain, const char* message, void* userData)
{
	LOG_INFO("mongoLog", "mongoLog: logLevel:{}, logDomain:{}, message:{}.\n", (int)logLevel, logDomain, message);
}

void MongoModule::testInsert()
{
	BsonObjectPtr insertor = std::make_shared<BsonObject>();
	insertor->appendString("test_insert1", "abcdef");
	insertor->appendInt32("test_insert2", 1234567);
	std::string dbName = "zq";
	std::string coName = "player";

	insert(dbName, coName, insertor).start([](async_simple::Try<MongoResultPtr> v) {
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

void MongoModule::testRemove()
{
	BsonObjectPtr selector = std::make_shared<BsonObject>();
	selector->appendString("test_insert1", "abcdef");
	std::string dbName = "zq";
	std::string coName = "player";

	remove(dbName, coName, selector).start([](async_simple::Try<MongoResultPtr> v) {
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

void MongoModule::testSave()
{
	BsonObjectPtr selector = std::make_shared<BsonObject>();
	BsonObjectPtr updatetor = std::make_shared<BsonObject>();
	selector->appendString("test_save34", "abcde");
	updatetor->appendString("test_save35", "abcdef");
	std::string dbName = "zq";
	std::string coName = "player";

	save(dbName, coName, selector, updatetor).start([](async_simple::Try<MongoResultPtr> v) {
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

void MongoModule::testFind()
{
	{
		std::vector<BsonObject> result;
		BsonObjectPtr selector = std::make_shared<BsonObject>();
		std::string dbName = "zq";
		std::string coName = "player";

		find(dbName, coName, selector).start([](async_simple::Try<MongoResultPtr> v) {

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

		find(dbName, coName, selector).start([](async_simple::Try<MongoResultPtr> v) {

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

void MongoModule::processCallbacks()
{
	while (!m_callbacks.empty())
	{
		MongoQueryCallback& callback = m_callbacks.front();
		if (callback.invokeIfReady())
		{
			m_callbacks.pop();
		}
	}
}

void MongoModule::taskThrad()
{
	while (1)
	{
		MongoTask* task;

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