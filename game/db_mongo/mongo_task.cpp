#include "db_mongo/mongo_task.h"
#include "db_mongo/mongo_module.h"


namespace zq{


MongoTask::MongoTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName) :
		m_mongoModule(mongoModule),
		m_dbName(dbName),
		m_collectionName(collectionName)
{
	m_result = std::make_unique<QueryResultPromise>();
}



// insert
MongoInsertTask::MongoInsertTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr insertor) :
		MongoTask(mongoModule, dbName, collectionName),
		m_insertor(insertor)
{
}

void MongoInsertTask::execute()
{
	MongoResultPtr result = std::make_shared<MongoResult>();
	result->success = m_mongoModule->mongoInsert(m_dbName, m_collectionName, *m_insertor, result->errorMsg);
	m_result->set_value(result);
}



// remove
MongoRemoveTask::MongoRemoveTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector) :
		MongoTask(mongoModule, dbName, collectionName),
		m_selector(selector)
{
}

void MongoRemoveTask::execute()
{
	MongoResultPtr result = std::make_shared<MongoResult>();
	result->success = m_mongoModule->mongoRemove(m_dbName, m_collectionName, *m_selector, result->errorMsg);
	m_result->set_value(result);
}



// save
MongoSaveTask::MongoSaveTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, BsonObjectPtr updator) :
		MongoTask(mongoModule, dbName, collectionName),
		m_selector(selector),
		m_updator(updator)
{
}

void MongoSaveTask::execute()
{
	MongoResultPtr result = std::make_shared<MongoResult>();
	result->success = m_mongoModule->mongoSave(m_dbName, m_collectionName, *m_selector, *m_updator, result->errorMsg);
	m_result->set_value(result);
}



// find
MongoFindTask::MongoFindTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, int limit, int skip) :
		MongoTask(mongoModule, dbName, collectionName),
		m_selector(selector),
		m_limit(limit),
		m_skip(skip)
{
}

void MongoFindTask::execute()
{
	MongoResultPtr findResult = std::make_shared<MongoResult>();
	findResult->success = m_mongoModule->mongoFind(m_dbName, m_collectionName, *m_selector, findResult->result, findResult->errorMsg, m_limit, m_skip);
	m_result->set_value(findResult);
}



}