#pragma once


#include "common/common.hpp"
#include "game_common/appid.h"



namespace zq{


//  5          3           8              16              64
// funcId    zoneId   instanceId      loop sequence     timestamp
// 95-91     90-88       87-80          79-64			63-0

class Gid
{
public:

	static bool initialize(const AppId& appId);
	static std::string genGid();

private:

	static bool ms_isInit;
	static uint16_t ms_loopSeq;
	static uint64_t ms_adapativeTime;
	static uint32_t ms_gidHigh32;
};


}

