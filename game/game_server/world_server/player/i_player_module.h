#pragma once


#include "game_common/i_module.hpp"
#include "common/common.hpp"
#include "protocol/s2s/db_player.pb.h"


namespace zq {


class IPlayerModule : public IModule
{
public:
	virtual ~IPlayerModule(){};
	virtual bool loadFromDB(const DB::DBPlayer& playerDBData) = 0;
	virtual bool saveToDB(DB::DBPlayer& playerDBData) = 0;
};


}
