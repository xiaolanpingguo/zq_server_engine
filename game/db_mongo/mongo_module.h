#pragma once


#include "common/common.hpp"
#include "common/concurrent_queue.hpp"
#include "game_common/i_module.hpp"
#include "db_mongo/mongo_task.h"
#include "common/coroutine_awaitor.hpp"

#include <mongoc/mongoc.h>

namespace zq{


class MongoTask;
class MongoInsertTask;
class MongoRemoveTask;
class MongoSaveTask;
class MongoFindTask;
class MongoModule : public IModule
{
	INIT_MODULE_NAME(MongoModule);

	friend class MongoInsertTask;
	friend class MongoRemoveTask;
	friend class MongoSaveTask;
	friend class MongoFindTask;

public:
	MongoModule(const std::string& user, const std::string& pwd, const std::string& host, uint16_t port, const std::vector<std::pair<std::string, std::string>>& collections);
	~MongoModule();

public:
	bool init() override;
	bool update() override;
	bool finalize() override;

	bool setupCollection(const std::string& dbName, const std::string& collectionName);

	async_simple::coro::Lazy<MongoResultPtr> insert(const std::string& dbName, const std::string& collectionName, BsonObjectPtr insertor);
	async_simple::coro::Lazy<MongoResultPtr> remove(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector);
	async_simple::coro::Lazy<MongoResultPtr> save(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, BsonObjectPtr updator);
	async_simple::coro::Lazy<MongoResultPtr> find(const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, int limit = 0, int skip = 0);

private:
	bool initMongo();
	bool mongoInsert(const std::string& dbName, const std::string& collectionName, BsonObject& insertor, std::string& errorMsg);
	bool mongoRemove(const std::string& dbName, const std::string& collectionName, BsonObject& selector, std::string& errorMsg);
	bool mongoSave(const std::string& dbName, const std::string& collectionName, BsonObject& selector, BsonObject& updator, std::string& errorMsg);
	bool mongoFind(const std::string& dbName, const std::string& collectionName, BsonObject& selector, std::vector<BsonObjectPtr>& result, std::string& errorMsg, int limit = 0, int skip = 0);
	void ping();

private:

	mongoc_database_t* getDb(const std::string& dbName);
	mongoc_collection_t* getCollection(const std::string& dbName, const std::string& collectionName);

    MongoQueryCallback addTask(MongoTask* task);
	MongoQueryCallback& addCallback(MongoQueryCallback&& query);
	void processCallbacks();
	void processTask();

	void keepAlive();
	void taskThrad();

private:

	std::atomic<bool> m_threadStop;
	std::unique_ptr<std::thread> m_taskThread;

	mongoc_uri_t* m_mongoUrl;
	mongoc_client_t* m_mongoClient;

	std::string m_user;
	std::string m_pwd;
	std::string m_host;
	uint16_t m_port;

	std::string m_url;
	uint64_t m_lastActiveTime;

	std::queue<MongoQueryCallback> m_callbacks;
	ConcurrentQueue<MongoTask*> m_queue;

	std::vector<std::pair<std::string, std::string>> m_initCollectionsConfig;

	std::vector<std::pair<std::string, std::string>> m_vecColl;
	std::map<std::string, mongoc_database_t*> m_databases;
	std::map<std::pair<std::string, std::string>, mongoc_collection_t*> m_collections;
	constexpr static std::string_view s_logCategory = "MongoModule";
};

}
