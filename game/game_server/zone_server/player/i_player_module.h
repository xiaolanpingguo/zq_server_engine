#pragma once


#include "game_common/i_module.hpp"
#include "common/common.hpp"
#include "protocol/s2s/db_player.pb.h"


namespace zq {


class IPlayerModule : public IModule
{
public:
	virtual ~IPlayerModule(){};
	virtual bool loadFromDB(const S2S::DBPlayerData& dbPlayerData) = 0;
	virtual bool saveToDB(S2S::DBPlayerData& dbPlayerData) = 0;
};


}
