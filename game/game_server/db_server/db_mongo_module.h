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
	mongoc_collection_t* getCollection(std::string& dbName, std::string& collectionName);
	bool mongoInsert(std::string& dbName, std::string& collectionName, BsonObject& bsonObject);
	bool mongoRemove(std::string& dbName, std::string& collectionName, BsonObject& selector);
	bool mongoSave(std::string& dbName, std::string& collectionName, BsonObject& selector, BsonObject& updateor);
	bool mongoUpdate(std::string& dbName, std::string& collectionName, BsonObject& selector, BsonObject& updateor);
	bool mongoFind(std::string& dbName, std::string& collectionName, BsonObject* selector, std::vector<BsonObject>& result);
	bool mongoBatchFind(std::string& dbName, std::string& collectionName);
	static void mongoLog(mongoc_log_level_t logLevel, const char* logDomain, const char* message, void* userData);

private:
	void onS2SMongoInsertReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoRemoveReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoSaveReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoUpdateReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoFindReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);
	void onS2SMongoBatchFindReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg);

private:

	DBServer* m_thisServer;

	std::string m_url;
	mongoc_uri_t* m_mongoUrl;
	mongoc_client_t* m_mongoClient;

	constexpr static std::string_view s_logCategory = "DBMongoModule";
};

}
