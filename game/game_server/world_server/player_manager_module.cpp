#include "game_server/world_server/player_manager_module.h"
#include "game_server/world_server/world_server.h"
#include "game_server/world_server/player/player.h"
#include "game_common/game_db_def.hpp"
#include "protocol/s2s/db_mongo_proxy.pb.h"


namespace zq{


PlayerManagerModule::PlayerManagerModule(WorldServer* thisServer) :
		m_thisServer(thisServer)
{
}

PlayerManagerModule::~PlayerManagerModule()
{
	for (auto& it : m_players)
	{
		delete it.second;
		it.second = nullptr;
	}
}

bool PlayerManagerModule::saveToDB()
{
	for (const auto& it : m_players)
	{
		DB::DBPlayer playerDBData;
		if (!it.second->saveToDB(playerDBData))
		{
			LOG_ERROR(s_logCategory, "save player db data error:");
			continue;
		}

		//DB::MongoDBMsg dbMsg;
		//dbMsg.set_cmd_type(DB::MONGO_MSG_SAVE_REQ);

		//DB::MongoSaveReq* saveReq = dbMsg.mutable_save_req();
		//saveReq->set_db_name(DB_NAME);
		//saveReq->set_col_name(DB_PLAYER);

		//DB::ProtoBsonObj* protoDoc = saveReq->mutable_doc();
		//DB::ProtoBsonData* pProtoData = protoDoc->add_bson_data_list();
		//pProtoData->set_data_type(iDataType);
		//pProtoData->set_key(pBsonData->GetKey());

		//if (pUserData)
		//{
		//	*saveReq->mutable_user_data() = *pUserData;
		//}
	}
	return true;
}


}

