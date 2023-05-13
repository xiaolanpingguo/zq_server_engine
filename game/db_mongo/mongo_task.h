#pragma once


#include "game_common/bson_object.h"
#include "game_common/query_callback.hpp"


namespace zq{


struct MongoResult
{
	bool success;
	std::string errorMsg;
	std::vector<BsonObjectPtr> foundResult; // for find
};


using MongoResultPtr = std::shared_ptr<MongoResult>;
using MongoQueryCallback = QueryCallback<MongoResultPtr>;
using QueryResultFuture = typename MongoQueryCallback::QueryResultFuture;
using QueryResultPromise = typename MongoQueryCallback::QueryResultPromise;


class MongoModule;
class MongoTask
{
public:
	MongoTask(MongoModule* mongoModule, const std::string& dbName, const std::string& collectionName);
	virtual ~MongoTask() {}
	virtual void execute() = 0;
	QueryResultFuture getFuture() { return m_result->get_future(); }

protected:
	std::unique_ptr<QueryResultPromise> m_result;

	std::string m_dbName;
	std::string m_collectionName;
	MongoModule* m_mongoModule;
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
	int m_limit;
	int m_skip;
	BsonObjectPtr m_selector;
};

}
