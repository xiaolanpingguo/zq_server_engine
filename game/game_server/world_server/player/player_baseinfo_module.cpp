#include "game_server/world_server/player/player_baseinfo_module.h"
#include "game_server/world_server/player/player.h"


namespace zq{


PlayerBaseInfoModule::PlayerBaseInfoModule(Player& player) :
	m_player(player)
{
}

PlayerBaseInfoModule::~PlayerBaseInfoModule()
{
}

bool PlayerBaseInfoModule::loadFromDB(const DB::DBPlayer& playerDBData)
{
	const DB::DBPlayerBaseInfo& baseInfo = playerDBData.player_base_info();
	uint64 uid = baseInfo.uid();
	uint64 openid = baseInfo.openid();
	std::string nickname = baseInfo.nickname();
	std::string ip = baseInfo.ip();

	return true;
}

bool PlayerBaseInfoModule::saveToDB(DB::DBPlayer& playerDBData)
{
	DB::DBPlayerBaseInfo* baseInfo = playerDBData.mutable_player_base_info();
	baseInfo->set_uid(1);
	baseInfo->set_openid(2);
	baseInfo->set_nickname("123");
	baseInfo->set_ip("456");

	return true;
}

} //namespace zq
