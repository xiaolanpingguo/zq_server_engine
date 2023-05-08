#include "game_server/db_server/db_mongo_module.h"
#include "game_common/message_helper.hpp"
#include "game_server/db_server/db_server.h"
#include "game_common/bson_object.h"

#include <bson/bson.h>

namespace zq{



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
	messagehelper.registerHandler<&DBMongoModule::onS2SMongoUpdateReq>(this, S2S::MSG_ID_DB_UPDATE_REQ);
	messagehelper.registerHandler<&DBMongoModule::onS2SMongoFindReq>(this, S2S::MSG_ID_DB_FIND_REQ);
	messagehelper.registerHandler<&DBMongoModule::onS2SMongoBatchFindReq>(this, S2S::MSG_ID_DB_BATCH_FIND_REQ);

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

bool DBMongoModule::finalize()
{
	mongoc_uri_destroy(m_mongoUrl);
	mongoc_client_destroy(m_mongoClient);
	mongoc_cleanup();
	return true;
}

bool DBMongoModule::initMongo()
{
	return true;
}

mongoc_collection_t* DBMongoModule::getCollection(std::string& dbName, std::string& collectionName)
{
	mongoc_collection_t* collection = mongoc_client_get_collection(m_mongoClient, dbName.c_str(), collectionName.c_str());
	if (nullptr == collection)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return nullptr;
	}

	return collection;
}

bool DBMongoModule::mongoInsert(std::string& dbName, std::string& collectionName, BsonObject& bsonObject)
{
	mongoc_collection_t* collection = mongoc_client_get_collection(m_mongoClient, dbName.c_str(), collectionName.c_str());
	if (nullptr == collection)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return false;
	}

    bson_error_t mongoError;
	bson_t bson;
	bson_init(&bson);
	bsonObject.convertToRawBson(bson);
	if (!mongoc_collection_insert_one(collection, &bson, nullptr, nullptr, &mongoError))
	{
		bson_destroy(&bson);
		LOG_ERROR(s_logCategory, "insert mongo data error, DBName:{}, CollectionName:{}, error:{}:{}:{}.\n", dbName, collectionName, mongoError.domain, mongoError.code, mongoError.message);
		return false;
	}

	bson_destroy(&bson);
	return true;
}

bool DBMongoModule::mongoRemove(std::string& dbName, std::string& collectionName, BsonObject& selector)
{
	mongoc_collection_t* collection = mongoc_client_get_collection(m_mongoClient, dbName.c_str(), collectionName.c_str());
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

bool DBMongoModule::mongoSave(std::string& dbName, std::string& collectionName, BsonObject& selector, BsonObject& updateor)
{
	mongoc_collection_t* collection = mongoc_client_get_collection(m_mongoClient, dbName.c_str(), collectionName.c_str());
	if (nullptr == collection)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
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

bool DBMongoModule::mongoUpdate(std::string& dbName, std::string& collectionName, BsonObject& selector, BsonObject& updateor)
{
	mongoc_collection_t* collection = mongoc_client_get_collection(m_mongoClient, dbName.c_str(), collectionName.c_str());
	if (nullptr == collection)
	{
		LOG_ERROR(s_logCategory, "Get Collection Failed DBName:{}, CollectionName:{}.\n", dbName, collectionName);
		return false;
	}

	bson_error_t mongoError;
	bson_t bsonSelector;
	bson_t bsonUpdateort;
	bson_t bsonChild;
	bson_init(&bsonSelector);
	bson_init(&bsonUpdateort);
	bson_init(&bsonChild);

	selector.convertToRawBson(bsonSelector);
	updateor.convertToRawBson(bsonUpdateort);

	bson_append_document_begin(&bsonUpdateort, "$set", -1, &bsonChild);
	bson_append_document_end(&bsonUpdateort, &bsonChild);

	if (!mongoc_collection_update_one(collection, &bsonSelector, &bsonUpdateort, nullptr, nullptr, &mongoError))
	{
		LOG_ERROR(s_logCategory, "update mongo data error, DBName:{}, CollectionName:{}, error:{}:{}:{}.\n", dbName, collectionName, mongoError.domain, mongoError.code, mongoError.message);
		bson_destroy(&bsonSelector);
		bson_destroy(&bsonUpdateort);
		bson_destroy(&bsonChild);
		return false;
	}

	bson_destroy(&bsonSelector);
	bson_destroy(&bsonUpdateort);
	bson_destroy(&bsonChild);

	return true;
}

bool DBMongoModule::mongoFind(std::string& dbName, std::string& collectionName, BsonObject* selector, std::vector<BsonObject>& result)
{
	mongoc_collection_t* collection = mongoc_client_get_collection(m_mongoClient, dbName.c_str(), collectionName.c_str());
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

bool DBMongoModule::mongoBatchFind(std::string& dbName, std::string& collectionName)
{
	return true;
}

void DBMongoModule::mongoLog(mongoc_log_level_t logLevel, const char* logDomain, const char* message, void* userData)
{
	LOG_INFO("mongoLog", "mongoLog: logLevel:{}, logDomain:{}, message:{}.\n", (int)logLevel, logDomain, message);
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

void DBMongoModule::onS2SMongoUpdateReq(TcpConnectionPtr connection, const S2S::MongoDBMsg& msg)
{
	const S2S::MongoUpdateReq& req = msg.update_req();
	std::string dbName = req.db_name();
	std::string collectionName = req.col_name();

	BsonObject selector;
	selector.convertFromProtoBson(req.selector());
	BsonObject updator;
	updator.convertFromProtoBson(req.updator());
	mongoUpdate(dbName, collectionName, selector, updator);
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

