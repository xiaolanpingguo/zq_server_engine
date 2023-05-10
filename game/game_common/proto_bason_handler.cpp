#include "game_common//proto_bason_handler.h"
#include "protocol/s2s/s2s_common.pb.h"
#include "protocol/s2s/db_mongo_proxy.pb.h"


namespace zq{


bool ProtoBasonHandler::requestInsert(const std::string& dbName, const std::string& collectionName, BsonObject& insertor, S2S::MongoUserData* userData)
{
	S2S::MongoDBMsg dbMsg;
	S2S::MongoInsertReq* req = dbMsg.mutable_insert_req();
	req->set_db_name(dbName);
	req->set_col_name(collectionName);

	S2S::ProtoBsonObj* protoDoc = req->mutable_doc();
	insertor.convertToProtoBson(*protoDoc);

	if (userData)
	{
		S2S::MongoUserData* data = dbMsg.mutable_user_data();
		data->CopyFrom(*userData);
	}

	std::string strDBMsg;
	if (!dbMsg.SerializeToString(&strDBMsg))
	{
		LOG_ERROR(s_logCategory, "serialize db msg error!");
		return false;
	}

	if (!sendToDBServer(S2S::MSG_ID_DB_INSERT_REQ, strDBMsg.data(), (uint32_t)strDBMsg.size()))
	{
		LOG_ERROR(s_logCategory, "send to db server failed!");
		return false;
	}

	return true;
}

bool ProtoBasonHandler::requestRemove(const std::string& dbName, const std::string& collectionName, BsonObject& selector, S2S::MongoUserData* userData)
{
	S2S::MongoDBMsg dbMsg;
	S2S::MongoRemoveReq* req = dbMsg.mutable_remove_req();
	req->set_db_name(dbName);
	req->set_col_name(collectionName);

	if (userData)
	{
		S2S::MongoUserData* data = dbMsg.mutable_user_data();
		data->CopyFrom(*userData);
	}

	S2S::ProtoBsonObj* protoSelector = req->mutable_selector();
	selector.convertToProtoBson(*protoSelector);

	std::string strDBMsg;
	if (!dbMsg.SerializeToString(&strDBMsg))
	{
		LOG_ERROR(s_logCategory, "serialize player bin data error:");
		return false;
	}

	if (!sendToDBServer(S2S::MSG_ID_DB_REMOVE_REQ, strDBMsg.data(), (uint32_t)strDBMsg.size()))
	{
		LOG_ERROR(s_logCategory, "send to db server failed!");
		return false;
	}

	return true;
}

bool ProtoBasonHandler::requestSave(const std::string& dbName, const std::string& collectionName, BsonObject& selector, BsonObject& updateor, S2S::MongoUserData* userData)
{
	S2S::MongoDBMsg dbMsg;
	S2S::MongoSaveReq* req = dbMsg.mutable_save_req();
	req->set_db_name(dbName);
	req->set_col_name(collectionName);

	if (userData)
	{
		S2S::MongoUserData* data = dbMsg.mutable_user_data();
		data->CopyFrom(*userData);
	}

	S2S::ProtoBsonObj* protoSelector = req->mutable_selector();
	selector.convertToProtoBson(*protoSelector);

	S2S::ProtoBsonObj* protoUpdator = req->mutable_updator();
	updateor.convertToProtoBson(*protoUpdator);

	std::string strDBMsg;
	if (!dbMsg.SerializeToString(&strDBMsg))
	{
		LOG_ERROR(s_logCategory, "serialize player bin data error:");
		return false;
	}

	if (!sendToDBServer(S2S::MSG_ID_DB_SAVE_REQ, strDBMsg.data(), (uint32_t)strDBMsg.size()))
	{
		LOG_ERROR(s_logCategory, "send to db server failed!");
		return false;
	}

	return true;
}

bool ProtoBasonHandler::requestFind(const std::string& dbName, const std::string& collectionName, BsonObject& selector, S2S::MongoUserData* userData, int limit, int skip)
{
	S2S::MongoDBMsg dbMsg;
	S2S::MongoFindReq* req = dbMsg.mutable_find_req();
	req->set_db_name(dbName);
	req->set_col_name(collectionName);
	req->set_limit(limit);
	req->set_skip(skip);

	if (userData)
	{
		S2S::MongoUserData* data = dbMsg.mutable_user_data();
		data->CopyFrom(*userData);
	}

	if (!selector.empty())
	{
		S2S::ProtoBsonObj* protoSelector = req->mutable_selector();
		selector.convertToProtoBson(*protoSelector);
	}

	std::string strDBMsg;
	if (!dbMsg.SerializeToString(&strDBMsg))
	{
		LOG_ERROR(s_logCategory, "serialize player bin data error:");
		return false;
	}

	if (!sendToDBServer(S2S::MSG_ID_DB_FIND_REQ, strDBMsg.data(), (uint32_t)strDBMsg.size()))
	{
		LOG_ERROR(s_logCategory, "send to db server failed!");
		return false;
	}

	return true;
}


}

