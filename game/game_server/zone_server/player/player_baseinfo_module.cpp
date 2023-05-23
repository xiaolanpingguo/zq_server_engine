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

bool PlayerBaseInfoModule::loadFromDB(const S2S::DBPlayerData& dbPlayerData)
{
	const S2S::DBPlayerBaseInfo& baseInfo = dbPlayerData.player_base_info();
	std::string sdkUserId = baseInfo.sdk_user_id();
	std::string sdkToken = baseInfo.sdk_token();
	uint32_t channelId = baseInfo.channel_id();
	std::string nickname = baseInfo.nickname();
	std::string ip = baseInfo.ip();
	return true;
}

bool PlayerBaseInfoModule::saveToDB(S2S::DBPlayerData& dbPlayerData)
{
	S2S::DBPlayerBaseInfo* baseInfo = dbPlayerData.mutable_player_base_info();
	baseInfo->set_sdk_user_id("123");
	baseInfo->set_sdk_token("456");
	baseInfo->set_channel_id(1);
	baseInfo->set_nickname("nickname");
	baseInfo->set_ip("127.0.0.1");

	return true;
}

} //namespace zq
