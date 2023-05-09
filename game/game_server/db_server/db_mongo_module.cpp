#include "game_server/db_server/db_mongo_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/db_server/db_server.h"
#include "game_common/bson_object.h"
#include "game_common/game_db_def.hpp"

#include <bson/bson.h>

namespace zq{


struct CollectionSetupData
{
	std::string dbName;
	std::string collectionName;
};


DBMongoModule::DBMongoModule(DBServer* thisServer)
	:
		m_thisServer(thisServer),
		m_mongoUrl(nullptr),
		m_mongoClient(nullptr)
{

}

DBMongoModule::~DBMongoModule()
{

}

bool DBMongoModule::init()
{
	using namespace std::placeholders;

	MessageHelper& messagehelper = MessageHelper::getInstance();
	messagehelper.registerHandler<&DBMongoModule::onS2SMongoInsertReq>(this, S2S::MSG_ID_DB_INSERT_REQ);
	messagehelper.registerHandler<&DBMongoModule::onS2SMongoRemoveReq>(this, S2S::MSG_ID_DB_REMOVE_REQ);
	messagehelper.registerHandler<&DBMongoModule::onS2SMongoSaveReq>(this, S2S::MSG_ID_DB_SAVE_REQ);
	messagehelper.registerHandler<&DBMongoModule::onS2SMongoFindReq>(this, S2S::MSG_ID_DB_FIND_REQ);
	messagehelper.registerHandler<&DBMongoModule::onS2SMongoBatchFindReq>(this, S2S::MSG_ID_DB_BATCH_FIND_REQ);

	if (!initMongo())
	{	
		return false;
	}

	std::vector<CollectionSetupData> allCollections
	{
		{ DB_NAME, COL_ACCOUNT },
		{ DB_NAME, COL_PLAYER },
		{ DB_NAME, COL_PLAYER_NAME},
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
		//testBatchFind();
	}

	return true;
}

bool DBMongoModule::finalize()
{
	for (auto& pair : m_collections)
	{
		mongoc_collection_destroy(pair.second);
	}
	mongoc_uri_destroy(m_mongoUrl);
	mongoc_client_destroy(m_mongoClient);
	mongoc_cleanup();
	return true;
}

bool DBMongoModule::initMongo()
{
	std::string ip = m_thisServer->getConfig().dbHost;
	uint16_t port = m_thisServer->getConfig().dbPort;
	std::string user_name = m_thisServer->getConfig().user;
	std::string pwd = m_thisServer->getConfig().pwd;

	if (user_name == "null")
	{
		m_url = std::format("mongodb://{}:{}", ip, port);
	}
	else
	{
		m_url = std::format("mongodb://{}:{}@{}:{}/?{}", user_name, pwd, ip, port, "authMechanism=SCRAM-SHA-1");
	}

	mongoc_init();
	m_mongoUrl = mongoc_uri_new(m_url.c_str());
	m_mongoClient = mongoc_client_new_from_uri(m_mongoUrl);
	mongoc_client_set_appname(m_mongoClient, "DBMongoModule");
	mongoc_log_set_handler(&DBMongoModule::mongoLog, this);

	return true;
}

bool DBMongoModule::setupCollection(const std::string& dbName, const std::string& collectionName)
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

bool DBMongoModule::mongoInsert(const std::string& dbName, const std::string& collectionName, BsonObject& insertor)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (nullptr == collection)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return false;
	}

    bson_error_t mongoError;
	bson_t bson;
	bson_init(&bson);
	insertor.convertToRawBson(bson);
	if (!mongoc_collection_insert_one(collection, &bson, nullptr, nullptr, &mongoError))
	{
		bson_destroy(&bson);
		LOG_ERROR(s_logCategory, "insert mongo data error, DBName:{}, CollectionName:{}, error:{}:{}:{}.\n", dbName, collectionName, mongoError.domain, mongoError.code, mongoError.message);
		return false;
	}

	bson_destroy(&bson);
	return true;
}

bool DBMongoModule::mongoRemove(const std::string& dbName, const std::string& collectionName, BsonObject& selector)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (nullptr == collection)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return false;
	}

	bson_error_t mongoError;
	bson_t bsonSelector;
	bson_init(&bsonSelector);
	selector.convertToRawBson(bsonSelector);

	if (!mongoc_collection_delete_one(collection, &bsonSelector, nullptr, nullptr, &mongoError))
	{
		LOG_ERROR(s_logCategory, "remove mongo data error, DBName:{}, CollectionName:{}, error:{}:{}:{}.\n", dbName, collectionName, mongoError.domain, mongoError.code, mongoError.message);
		bson_destroy(&bsonSelector);
		return false;
	}

	bson_destroy(&bsonSelector);
	return true;
}

bool DBMongoModule::mongoSave(const std::string& dbName, const std::string& collectionName, BsonObject& selector, BsonObject& updateor)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (nullptr == collection)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return false;
	}

	if (selector.empty())
	{
		LOG_ERROR(s_logCategory, "selector cannot be empty, specify an empty document will replace the first document returned in the collection.\n");
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
	updateor.convertToRawBson(bsonUpdateort);

	if (!mongoc_collection_replace_one(collection, &bsonSelector, &bsonUpdateort, &bsonOpt, nullptr, &mongoError))
	{
		LOG_ERROR(s_logCategory, "save mongo data error, DBName:{}, CollectionName:{}, error:{}:{}:{}.\n", dbName, collectionName, mongoError.domain, mongoError.code, mongoError.message);
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

bool DBMongoModule::mongoFind(const std::string& dbName, const std::string& collectionName, BsonObject* selector, std::vector<BsonObject>& result)
{
	mongoc_collection_t* collection = getCollection(dbName, collectionName);
	if (nullptr == collection)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return false;
	}

	bson_t baseQuery;
	bson_init(&baseQuery);
	if (selector)
	{
		selector->convertToRawBson(baseQuery);
	}

	int num = 0;
	bool success = false;
	do
	{
		mongoc_cursor_t* pCursor = mongoc_collection_find_with_opts(collection, &baseQuery, nullptr, nullptr);
		if (pCursor == nullptr)
		{
			break;
		}

		const bson_t* doc;
		while (mongoc_cursor_next(pCursor, &doc))
		{
			if (doc != nullptr)
			{
				BsonObject bsonObj;
				bsonObj.convertFromRawBson(*doc);
				result.emplace_back(bsonObj);
			}
			num++;
		}

		mongoc_cursor_destroy(pCursor);

		success = true;
	}
	while (0);

	bson_destroy(&baseQuery);

	if (!success)
	{
		LOG_ERROR(s_logCategory, "find mongo data error, DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return false;
	}

	return true;
}

bool DBMongoModule::mongoBatchFind(const std::string& dbName, const std::string& collectionName)
{
	return true;
}

mongoc_collection_t* DBMongoModule::getCollection(const std::string& dbName, const std::string& collectionName)
{
	auto pair = std::make_pair(dbName, collectionName);
	auto it = m_collections.find(pair);
	if (it == m_collections.end())
	{
		return nullptr;
	}

	return it->second;
}

void DBMongoModule::mongoLog(mongoc_log_level_t logLevel, const char* logDomain, const char* message, void* userData)
{
	LOG_INFO("mongoLog", "mongoLog: logLevel:{}, logDomain:{}, message:{}.\n", (int)logLevel, logDomain, message);
}

void DBMongoModule::testInsert()
{
	BsonObject obj;
	obj.appendString("test_insert1", "abcdef");
	obj.appendInt32("test_insert2", 1234567);
	mongoInsert("zq", "player1", obj);
}

void DBMongoModule::testRemove()
{
	BsonObject selecotr;
	selecotr.appendString("dwadaw", "abcdef");
	mongoRemove("zq", "player", selecotr);
}

void DBMongoModule::testSave()
{
	BsonObject selector, updatetor;
	selector.appendString("test_save34", "abcde");
	updatetor.appendString("test_save35", "abcdef");
	mongoSave("zq", "player1", selector, updatetor);
}

void DBMongoModule::testFind()
{
	std::vector<BsonObject> result;
	mongoFind("zq", "player1", nullptr, result);

	LOG_INFO(s_logCategory, "find all, result num:{}", result.size());
	for (auto& obj : result)
	{
		LOG_INFO(s_logCategory, "details:{}", obj.debugPrint());
	}

	result.clear();
	BsonObject selector;
	selector.appendString("test_save1", "123333");
	mongoFind("zq", "player1", &selector, result);

	LOG_INFO(s_logCategory, "find a match, result num:{}", result.size());
	for (auto& obj : result)
	{
		LOG_INFO(s_logCategory, "details:{}", obj.debugPrint());
	}
}

void DBMongoModule::testBatchFind()
{

}

void DBMongoModule::onS2SMongoInsertReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg)
{
	const S2S::MongoInsertReq& req = msg.insert_req();
	std::string dbName = req.db_name();
	std::string collectionName = req.col_name();

	BsonObject bsonObject;
	bsonObject.convertFromProtoBson(req.doc());
	mongoInsert(dbName, collectionName, bsonObject);
}

void DBMongoModule::onS2SMongoRemoveReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg)
{
	const S2S::MongoRemoveReq& req = msg.remove_req();
	std::string dbName = req.db_name();
	std::string collectionName = req.col_name();

	BsonObject selector;
	selector.convertFromProtoBson(req.selector());
	mongoRemove(dbName, collectionName, selector);
}

void DBMongoModule::onS2SMongoSaveReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg)
{
	const S2S::MongoSaveReq& req = msg.save_req();
	std::string dbName = req.db_name();
	std::string collectionName = req.col_name();

	BsonObject selector;
	selector.convertFromProtoBson(req.selector());
	BsonObject replacement;
	replacement.convertFromProtoBson(req.replacement());
	mongoSave(dbName, collectionName, selector, replacement);
}

void DBMongoModule::onS2SMongoFindReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg)
{
	const S2S::MongoFindReq& req = msg.find_req();
	std::string dbName = req.db_name();
	std::string collectionName = req.col_name();

	std::vector<BsonObject> result;
	mongoFind(dbName, collectionName, nullptr, result);
}

void DBMongoModule::onS2SMongoBatchFindReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg)
{
	const S2S::MongoBatchFindReq& req = msg.batch_find_req();
	std::string dbName = req.db_name();
	std::string collectionName = req.col_name();
}



}

