#pragma once


#include "common/common.hpp"
#include "common/concurrent_queue.hpp"
#include "game_common/i_module.hpp"
#include "db_mongo/mongo_task.h"
#include "common/coroutine_awaitor.hpp"

#include <mongoc/mongoc.h>


namespace zq{


class MongoTask;
class MongoModule : public IModule
{
	INIT_MODULE_NAME(MongoModule);

public:
	MongoModule(const std::string& user, const std::string& pwd, const std::string& host, uint16_t port);
	~MongoModule();

public:
	bool init() override;
	bool update() override;
	bool finalize() override;

    MongoQueryCallback addTask(MongoTask* task);
	MongoQueryCallback& addCallback(MongoQueryCallback&& query);

	async_simple::coro::Lazy<MongoResultPtr> insert(const std::string& dbName, const std::string& collectionName, BsonObjectPtr insertor);
	async_simple::coro::Lazy<MongoResultPtr> remove(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector);
	async_simple::coro::Lazy<MongoResultPtr> save(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, BsonObjectPtr updator);
	async_simple::coro::Lazy<MongoResultPtr> find(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, int limit = 0, int skip = 0);

public:
	bool initMongo();
	bool setupCollection(const std::string& dbName, const std::string& collectionName);
	bool mongoInsert(const std::string& dbName, const std::string& collectionName, BsonObject& insertor, std::string& errorMsg);
	bool mongoRemove(const std::string& dbName, const std::string& collectionName, BsonObject& selector, std::string& errorMsg);
	bool mongoSave(const std::string& dbName, const std::string& collectionName, BsonObject& selector, BsonObject& updator, std::string& errorMsg);
	bool mongoFind(const std::string& dbName, const std::string& collectionName, BsonObject& selector, std::vector<BsonObjectPtr>& result, std::string& errorMsg, int limit = 0, int skip = 0);
	bool mongoBatchFind(const std::string& dbName, const std::string& collectionName);

private:
	static void mongoLog(mongoc_log_level_t logLevel, const char* logDomain, const char* message, void* userData);
	mongoc_collection_t* getCollection(const std::string& dbName, const std::string& collectionName);
	void processCallbacks();
	void taskThrad();

	void testInsert();
	void testRemove();
	void testSave();
	void testFind();

private:

	bool m_threadStop;
	std::thread m_thr;

	mongoc_uri_t* m_mongoUrl;
	mongoc_client_t* m_mongoClient;

	std::string m_user;
	std::string m_pwd;
	std::string m_host;
	uint16_t m_port;

	std::string m_url;

	std::queue<MongoQueryCallback> m_callbacks;
	ConcurrentQueue<MongoTask*> m_queue;

	std::map<std::pair<std::string, std::string>, mongoc_collection_t*> m_collections;
	constexpr static std::string_view s_logCategory = "MongoModule";
};

}
