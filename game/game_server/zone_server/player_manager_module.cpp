#include "game_server/zone_server/player_manager_module.h"
#include "game_server/zone_server/zone_server.h"
#include "game_server/zone_server/player/player.h"
#include "game_common/game_db_def.hpp"
#include "protocol/s2s/s2s_common.pb.h"
#include "protocol/c2s/c2s_login.pb.h"
#include "game_common/bson_object.h"
#include "db_mongo/mongo_module.h"


namespace zq{


PlayerManagerModule::PlayerManagerModule(ZoneServer* thisServer) :
		m_thisServer(thisServer)
{
}

PlayerManagerModule::~PlayerManagerModule()
{
}

bool PlayerManagerModule::init()
{
	return true;
}

bool PlayerManagerModule::update()
{
	return true;
}

bool PlayerManagerModule::finalize()
{
	for (auto& it : m_players)
	{
		delete it.second;
		it.second = nullptr;
	}

	return true;
}

Player* PlayerManagerModule::findPlayer(std::string profileId)
{
	auto it = m_players.find(profileId);
	if (it != m_players.end())
	{
		return it->second;
	}

	return nullptr;
}

async_simple::coro::Lazy<int> PlayerManagerModule::onPlayerLogin(const std::string& profileId, const SDKAccountInfo& sdkInfo, std::shared_ptr<TcpConnection> conn)
{
	Player* player = findPlayer(profileId);
	if (player != nullptr)
	{
		LOG_ERROR(s_logCategory, "player has exist!, id:{}.", profileId);
		co_return C2S::EC_GENERRAL_ERROR;
	}

	// get player db data
	BsonObjectPtr result = nullptr;
	int r = co_await getPlayerDBData(profileId, result);
	if (r == 0)
	{
		std::string playerBin = result->getBin(PALYER_KEY_BIN);
		S2S::DBPlayerData dbPlayerData;
		if (!dbPlayerData.ParseFromString(playerBin))
		{
			LOG_ERROR(s_logCategory, "ParseFromString failed:{}.", profileId);
			co_return C2S::EC_LOGIN_LOAD_DATA_FAILED;
		}

		player = new Player();
		player->setProfileId(profileId);
		player->setSDKAccountInfo(sdkInfo);
		player->setConnection(conn);
		if (!player->loadFromDB(dbPlayerData))
		{
			LOG_ERROR(s_logCategory, "player loadFromDB failed:{}.", profileId);
			delete player;
			co_return C2S::EC_LOGIN_LOAD_DATA_FAILED;
		}

		m_players[profileId] = player;
	}
	// new player, create and save data
	else
	{
		player = new Player();
		player->setProfileId(profileId);
		player->setSDKAccountInfo(sdkInfo);
		player->setConnection(conn);
		r = co_await savePlayerDBData(player);
		if (r != 0)
		{
			LOG_ERROR(s_logCategory, "player save to db failed:{}:{}.", profileId);
			delete player;
			co_return C2S::EC_LOGIN_LOAD_DATA_FAILED;
		}

		m_players[profileId] = player;
	}

	co_return C2S::EC_SUCCESS;
}

async_simple::coro::Lazy<int> PlayerManagerModule::getPlayerDBData(const std::string& profileId, std::shared_ptr<BsonObject> playerBin)
{
	if (profileId.empty())
	{
		co_return -1;
	}

	BsonObjectPtr selector = std::make_shared<BsonObject>();
	selector->appendString(PLAYER_KEY_PROFILE_ID, profileId);
	MongoResultPtr result = co_await m_thisServer->getModule<MongoModule>()->find(DB_NAME, COL_PLAYER, selector);
	if (!result->success)
	{
		LOG_ERROR(s_logCategory, "find player db failed:{}.", result->errorMsg);
		co_return -2;
	}

	if (result->foundResult.size() > 1)
	{
		LOG_ERROR(s_logCategory, "find player data error!, there are {} result!", result->foundResult.size());
		co_return -3;
	}

	if (result->foundResult.size() == 1)
	{
		playerBin = result->foundResult[0];
	}

	co_return 0;
}

async_simple::coro::Lazy<int> PlayerManagerModule::savePlayerDBData(Player* player)
{
	if (player == nullptr || player->getProfileId().empty())
	{
		LOG_ERROR(s_logCategory, "player == nullptr.");
		co_return -1;
	}

	const std::string& profileId = player->getProfileId();

	S2S::DBPlayerData dbPlayerData;
	if (!player->saveToDB(dbPlayerData))
	{
		LOG_ERROR(s_logCategory, "player save to db failed:{}.", profileId);
		co_return -1;
	}

	std::string playerBin;
	if (!dbPlayerData.SerializeToString(&playerBin))
	{
		LOG_ERROR(s_logCategory, "serialize player bin data error:{} ", profileId);
		co_return -1;
	}

	BsonObjectPtr selector = std::make_shared<BsonObject>();
	selector->appendString(PLAYER_KEY_PROFILE_ID, profileId);
	BsonObjectPtr updator = std::make_shared<BsonObject>();
	updator->appendString(PLAYER_KEY_PROFILE_ID, profileId);
	updator->appendBin(PALYER_KEY_BIN, playerBin.data(), playerBin.size());
	MongoResultPtr result = co_await m_thisServer->getModule<MongoModule>()->save(DB_NAME, COL_PLAYER, selector, updator);
	if (!result->success)
	{
		LOG_ERROR(s_logCategory, "save player db failed:{}, id:{}.", result->errorMsg, profileId);
		co_return -2;
	}

	co_return 0;
}

bool PlayerManagerModule::saveToDB()
{
	for (const auto& it : m_players)
	{
		savePlayerDBData(it.second).start([](auto&&) {});
	}

	return true;
}


}

