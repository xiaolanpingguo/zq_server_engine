#include "game_common/gid.h"



namespace zq{


bool Gid::ms_isInit = false;
uint16_t Gid::ms_loopSeq = 0;
uint64_t Gid::ms_adapativeTime = 0;
uint32_t Gid::ms_gidHigh32 = 0;

bool Gid::initialize(const AppId& appId)
{
	if (ms_isInit)
	{
		return false;
	}

	ms_loopSeq = 0;
	ms_adapativeTime = time(nullptr);

	ms_gidHigh32 = 0;
	uint32_t tmpGIDMask = 0;

	// funcId    zoneId   instanceId      loop sequence
	// 00000     000       00000000      0000000000000000
	tmpGIDMask = appId.funcId;
	ms_gidHigh32 |= (tmpGIDMask << 27);

	tmpGIDMask = appId.zoneId;
	ms_gidHigh32 |= (tmpGIDMask << 24);

	tmpGIDMask = appId.instanceId;
	ms_gidHigh32 |= (tmpGIDMask << 16);

	ms_isInit = true;
	return true;
}

std::string Gid::genGid()
{
	ms_gidHigh32 |= ms_loopSeq;

	std::string gid = std::to_string(ms_gidHigh32) + std::to_string(ms_adapativeTime);

	// generate the next gid to be used
	uint64_t now = time(nullptr);
	if (now <= ms_adapativeTime)
	{
		ms_loopSeq++;

		// reach a circal
		if (ms_loopSeq == 0)
		{
			ms_adapativeTime++;
		}
	}
	else
	{
		ms_adapativeTime++;
	}

	return gid;
}


} //namespace zq