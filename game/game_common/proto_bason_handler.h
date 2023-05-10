#pragma once


#include "game_common/bson_object.h"
#include <assert.h>
#include <unordered_map>


namespace S2S
{
class MongoUserData;
}

namespace zq {


class ProtoBasonHandler
{
public:
	using InsertResCbT = std::function<void(bool, const S2S::MongoUserData&)>;
	using RemoveResCbT = std::function<void(bool, const S2S::MongoUserData&)>;
	using SaveResCbT = std::function<void(bool, const S2S::MongoUserData&)>;
	using FindResCbT = std::function<void(bool, const S2S::MongoUserData&, const std::vector<BsonObject>&)>;
	using BatchFindResCbT =std::function<void(bool, const S2S::MongoUserData&, const std::vector<BsonObject>&)>;

	virtual ~ProtoBasonHandler() = default;
	virtual bool requestInsert(const std::string& dbName, const std::string& collectionName, BsonObject& insertor, S2S::MongoUserData* userData = nullptr);
	virtual bool requestRemove(const std::string& dbName, const std::string& collectionName, BsonObject& selector, S2S::MongoUserData* userData = nullptr);
	virtual bool requestSave(const std::string& dbName, const std::string& collectionName, BsonObject& selector, BsonObject& updateor, S2S::MongoUserData* userData = nullptr);
	virtual bool requestFind(const std::string& dbName, const std::string& collectionName, BsonObject& selector, S2S::MongoUserData* userData = nullptr, int limit = 0, int skip = 0);
	virtual bool sendToDBServer(uint16_t msgId, const char* data, uint32_t len) = 0;

	void registerInsertResCb(int cmd, const InsertResCbT& cb) { auto it = m_insertResCallbacks.emplace(cmd, cb); assert(it.second);}
	void registerRemoveResCb(int cmd, const RemoveResCbT& cb) { auto it = m_removeResCallbacks.emplace(cmd, cb); assert(it.second);}
	void registerSaveResCb(int cmd, const SaveResCbT& cb) { auto it = m_saveResCallbacks.emplace(cmd, cb); assert(it.second);}
	void registerFindResCb(int cmd, const FindResCbT& cb) { auto it = m_findResCallbacks.emplace(cmd, cb); assert(it.second);}
	void registerBatchFindResCb(int cmd, const BatchFindResCbT& cb) { auto it = m_batchFindResCallbacks.emplace(cmd, cb); assert(it.second);}

protected:

	std::unordered_map<int, InsertResCbT> m_insertResCallbacks;
	std::unordered_map<int, RemoveResCbT> m_removeResCallbacks;
	std::unordered_map<int, SaveResCbT> m_saveResCallbacks;
	std::unordered_map<int, FindResCbT> m_findResCallbacks;
	std::unordered_map<int, BatchFindResCbT> m_batchFindResCallbacks;

	constexpr static std::string_view s_logCategory = "ProtoBasonHandlern";
};

}
