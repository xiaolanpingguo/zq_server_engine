#include "db_mongo/mongo_module.h"
#include "game_common/game_db_def.hpp"

#include <bson/bson.h>


namespace zq{


static void mongoLog(mongoc_log_level_t logLevel, const char* logDomain, const char* message, void* userData)
{
	LOG_INFO("mongoInternalLog", "mongoLog: logLevel:{}, logDomain:{}, message:{}.\n", (int)logLevel, logDomain, message);
}

MongoModule::MongoModule(const std::string& user, const std::string& pwd, const std::string& host, uint16_t port) :
		m_threadStop(false),
		m_mongoUrl(nullptr),
		m_mongoClient(nullptr),
		m_user(user),
		m_pwd(pwd),
		m_host(host),
		m_port(port),
		m_lastActiveTime(time(nullptr))
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

	m_taskThread = std::make_unique<std::thread>(std::bind(&MongoModule::taskThrad, this));

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
	m_queue.stopWait();
	if (m_taskThread && m_taskThread->joinable())
	{
		m_taskThread->join();
	}

	for (auto& pair : m_collections)
	{
		if (pair.second)
		{
			mongoc_collection_destroy(pair.second);
		}
	}

	for (auto& pair : m_databases)
	{
		if (pair.second)
		{
			mongoc_database_destroy(pair.second);
		}
	}

	if (m_mongoUrl)
	{
		mongoc_uri_destroy(m_mongoUrl);
	}

	if (m_mongoClient)
	{
		mongoc_client_destroy(m_mongoClient);
	}

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
	mongoc_log_set_handler(mongoLog, this);

	return true;
}

bool MongoModule::setupCollection(const std::string& dbName, const std::string& collectionName)
{
	bool success = false;
	do 
	{
		mongoc_database_t* db = getDb(dbName);
		if (db == nullptr)
		{
			db = mongoc_client_get_database(m_mongoClient, dbName.c_str());
			if (db == nullptr)
			{
				LOG_ERROR(s_logCategory, "get db failed dbName:{}, collectionName:{}.", dbName, collectionName);
				break;
			}

			m_databases[dbName] = db;
		}

		mongoc_collection_t* collection = getCollection(dbName, collectionName);
		if (collection == nullptr)
		{
			collection = mongoc_client_get_collection(m_mongoClient, dbName.c_str(), collectionName.c_str());
			if (collection == nullptr)
			{
				LOG_ERROR(s_logCategory, "get collection failed dbName:{}, collectionName:{}.", dbName, collectionName);
				break;
			}

			m_vecColl.emplace_back(std::make_pair(dbName, collectionName));
			m_collections[std::make_pair(dbName, collectionName)] = collection;
		}

		success = true;
	} while (0);
	
	if (!success)
	{
		for (auto it = m_collections.begin(); it != m_collections.end(); it++)
		{
			mongoc_collection_destroy(it->second);
		}

		for (auto it = m_databases.begin(); it != m_databases.end(); it++)
		{
			mongoc_database_destroy(it->second);
		}

		return false;
	}

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

	m_lastActiveTime = time(nullptr);

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

	m_lastActiveTime = time(nullptr);

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

	m_lastActiveTime = time(nullptr);

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

	m_lastActiveTime = time(nullptr);

	if (!success)
	{
		errorMsg = fmt::format("find mongo data error, dbName:{}, collectionName:{}.", dbName, collectionName);
		LOG_ERROR(s_logCategory, "{}", errorMsg);
		return false;
	}

	return true;
}

void MongoModule::ping()
{
	bool connected = true;
	for (auto& ele : m_databases)
	{
		bson_t bsonPing;
		bson_error_t error;

		bson_init(&bsonPing);
		bson_append_int32(&bsonPing, "ping", 4, 1);
		bool r = mongoc_database_command_with_opts(ele.second, &bsonPing, nullptr, nullptr, nullptr, &error);
		bson_destroy(&bsonPing);

		if (!r)
		{
			connected = false;
			LOG_ERROR(s_logCategory, "ping failure, mongo client has disConnect!, dbname:{} ", ele.first);
			break;
		}
	}

	if (connected)
	{
		return;
	}

	for (auto it = m_collections.begin(); it != m_collections.end(); it++)
	{
		mongoc_collection_destroy(it->second);
	}

	for (auto it = m_databases.begin(); it != m_databases.end(); it++)
	{
		mongoc_database_destroy(it->second);
	}
	m_collections.clear();
	m_databases.clear();

	mongoc_client_t* newConn = mongoc_client_new_from_uri(m_mongoUrl);
	if (newConn == nullptr)
	{
		LOG_ERROR(s_logCategory, "create a new mongo client failed!, url:{} ", m_url);
		return;
	}

	if (m_mongoClient)
	{
		mongoc_client_destroy(m_mongoClient);
		m_mongoClient = nullptr;
	}

	m_mongoClient = newConn;
	for (const auto& pair : m_vecColl)
	{
		if (!setupCollection(pair.first, pair.second))
		{
			LOG_ERROR(s_logCategory, "setup collection failed!, db:{}, coll:{}", pair.first, pair.second);
			return;
		}
	}
}

mongoc_database_t* MongoModule::getDb(const std::string& dbName)
{
	auto it = m_databases.find(dbName);
	if (it == m_databases.end())
	{
		return nullptr;
	}

	return it->second;
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

void MongoModule::processTask()
{
	MongoTask* task = nullptr;
	if (m_queue.pop(task))
	{
		task->execute();
		delete task;
		task = nullptr;
	}
}

void MongoModule::keepAlive()
{
	constexpr static int s_interVal = 5;
	time_t now = time(nullptr);
	if (now - m_lastActiveTime >= s_interVal)
	{
		m_lastActiveTime = now;
		ping();
	}
}

void MongoModule::taskThrad()
{
	while (!m_threadStop || !m_queue.empty())
	{
		keepAlive();
		processTask();
	}
}

}