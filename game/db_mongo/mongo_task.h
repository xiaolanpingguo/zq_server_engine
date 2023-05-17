#pragma once


#include "game_common/bson_object.h"
#include "game_common/query_callback.hpp"


namespace zq{


struct MongoResult
{
	bool success = false;
	std::string errorMsg;
	std::vector<BsonObjectPtr> foundResult; // for find and saveIfNotExist
};


using MongoResultPtr = std::shared_ptr<MongoResult>;
using MongoQueryCallback = QueryCallback<MongoResultPtr>;
using MongoQueryResultFuture = typename MongoQueryCallback::QueryResultFuture;
using MongoQueryResultPromise = typename MongoQueryCallback::QueryResultPromise;


class MongoModule;
class MongoTask
{
public:
	MongoTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName);
	virtual ~MongoTask() {}
	virtual void execute() = 0;
	MongoQueryResultFuture getFuture() { return m_result->get_future(); }

protected:
	std::unique_ptr<MongoQueryResultPromise> m_result;

	MongoModule* m_mongoModule;
	std::string m_dbName;
	std::string m_collectionName;
};



class MongoInsertTask : public MongoTask
{
public:
	MongoInsertTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr insertor);
	virtual ~MongoInsertTask() {}

	void execute() override;

protected:

	BsonObjectPtr m_insertor;
};



class MongoRemoveTask : public MongoTask
{
public:
	MongoRemoveTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr obj);
	virtual ~MongoRemoveTask() {}

	void execute() override;

protected:
	BsonObjectPtr m_selector;
};



class MongoSaveTask : public MongoTask
{
public:
	MongoSaveTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, BsonObjectPtr updator);
	virtual ~MongoSaveTask() {}

	void execute() override;

protected:
	BsonObjectPtr m_selector;
	BsonObjectPtr m_updator;
};




class MongoFindTask : public MongoTask
{
public:
	MongoFindTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, int limit = 0, int skip = 0);
	virtual ~MongoFindTask() {}

	void execute() override;

protected:
	BsonObjectPtr m_selector;
	int m_limit;
	int m_skip;
};



class MongoSaveIfNotExistTask : public MongoTask
{
public:
	MongoSaveIfNotExistTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName, BsonObjectPtr selector, BsonObjectPtr updator);
	virtual ~MongoSaveIfNotExistTask() {}

	void execute() override;

protected:
	BsonObjectPtr m_selector;
	BsonObjectPtr m_updator;
};

}
