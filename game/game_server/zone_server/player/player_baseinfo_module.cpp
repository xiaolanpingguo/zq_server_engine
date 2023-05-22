#include "game_server/zone_server/player/player_baseinfo_module.h"
#include "game_server/zone_server/player/player.h"


namespace zq{


PlayerBaseInfoModule::PlayerBaseInfoModule(Player& player) :
	m_player(player)
{
}

PlayerBaseInfoModule::~PlayerBaseInfoModule()
{
}

bool PlayerBaseInfoModule::loadFromDB(const S2S::DBPlayerData& playerDBData)
{
	const S2S::DBPlayerBaseInfo& baseInfo = playerDBData.player_base_info();
	uint64_t uid = baseInfo.uid();
	uint64_t openid = baseInfo.openid();
	std::string nickname = baseInfo.nickname();
	std::string ip = baseInfo.ip();

	return true;
}

bool PlayerBaseInfoModule::saveToDB(S2S::DBPlayerData& playerDBData)
{
	S2S::DBPlayerBaseInfo* baseInfo = playerDBData.mutable_player_base_info();
	baseInfo->set_uid(1);
	baseInfo->set_openid(2);
	baseInfo->set_nickname("123");
	baseInfo->set_ip("456");

	return true;
}

} //namespace zq
