#include "game_server/zone_server/player_manager_module.h"
#include "game_server/zone_server/zone_server.h"
#include "game_server/zone_server/player/player.h"
#include "game_common/game_db_def.hpp"
#include "protocol/s2s/s2s_common.pb.h"
#include "protocol/s2s/db_mongo_proxy.pb.h"
#include "game_common/bson_object.h"


namespace zq{


PlayerManagerModule::PlayerManagerModule(ZoneServer* thisServer) :
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
		BsonObject bsonObj;

		S2S::DBPlayer playerDBData;
		if (!it.second->saveToDB(playerDBData))
		{
			LOG_ERROR(s_logCategory, "save player db data error:");
			continue;
		}

		std::string playerBin;
		if (!playerDBData.SerializeToString(&playerBin))
		{
			LOG_ERROR(s_logCategory, "serialize player bin data error:");
			continue;
		}

		bsonObj.appendBin(PALYER_KEY_BIN, playerBin.data(), playerBin.size());

		S2S::MongoDBMsg dbMsg;
		S2S::MongoSaveReq* saveReq = dbMsg.mutable_save_req();
		saveReq->set_db_name(DB_NAME);
		saveReq->set_col_name(COL_PLAYER);

		S2S::ProtoBsonObj* updator = saveReq->mutable_updator();
		bsonObj.convertToProtoBson(*updator);

		std::string strDBMsg;
		if (!dbMsg.SerializeToString(&strDBMsg))
		{
			LOG_ERROR(s_logCategory, "serialize player bin data error:");
			continue;
		}
	}

	return true;
}


}

