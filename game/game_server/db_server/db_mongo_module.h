#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_connection.hpp"
#include "protocol/s2s/s2s_common.pb.h"
#include "protocol/s2s/db_mongo_proxy.pb.h"
#include "game_common/bson_object.h"

#include <mongoc/mongoc.h>


namespace zq {


class DBServer;
class DBMongoModule : public IModule
{
	INIT_MODULE_NAME(DBMongoModule);

public:
	DBMongoModule(DBServer* thisServer);
	~DBMongoModule();

public:
	bool init() override;
	bool finalize() override;

private:
	bool initMongo();
	bool setupCollection(const std::string& dbName, const std::string& collectionName);
	bool mongoInsert(const std::string& dbName, const std::string& collectionName, BsonObject& insertor);
	bool mongoRemove(const std::string& dbName, const std::string& collectionName, BsonObject& selector);
	bool mongoSave(const std::string& dbName, const std::string& collectionName, BsonObject& selector, BsonObject& updateor);
	bool mongoFind(const std::string& dbName, const std::string& collectionName, BsonObject& selector, BsonObject& option, std::vector<BsonObject>& result);
	bool mongoBatchFind(const std::string& dbName, const std::string& collectionName);
	mongoc_collection_t* getCollection(const std::string& dbName, const std::string& collectionName);
	static void mongoLog(mongoc_log_level_t logLevel, const char* logDomain, const char* message, void* userData);

	void testInsert();
	void testRemove();
	void testSave();
	void testFind();
	void testBatchFind();

private:
	void onS2SMongoInsertReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoRemoveReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoSaveReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoFindReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoBatchFindReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);

private:

	DBServer* m_thisServer;

	std::string m_url;
	mongoc_uri_t* m_mongoUrl;
	mongoc_client_t* m_mongoClient;

	std::map<std::pair<std::string, std::string>, mongoc_collection_t*> m_collections;
	constexpr static std::string_view s_logCategory = "DBMongoModule";
};

}
