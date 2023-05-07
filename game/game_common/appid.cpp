#include "game_common/appid.h"
#include "common/string.hpp"


namespace zq{


bool AppId::init(const std::string& appId)
{
	std::vector<uint8_t> result = strutils::split<uint8_t>(appId, ".");
	if (result.size() != 4)
	{
		return false;
	}
	
	uint8_t worldId = result[0];
	uint8_t funcId = result[1];
	uint8_t zoneId = result[2];
	uint8_t instanceId = result[3];

	if (funcId > 32 || funcId < 1)
	{
		return false;
	}

	this->worldId = worldId;
	this->funcId = funcId;
	this->zoneId = zoneId;
	this->instanceId = instanceId;
	this->strAppId = appId;
	this->intAppId = AppId::makeId(worldId, funcId, zoneId, instanceId);
	return true;
}

std::string AppId::convertIntToStr(int appid)
{
	int worldId = getWorldId(appid);
	int funcId = getFuncId(appid);
	int zoneId = getZoneId(appid);
	int instId = getInstId(appid);
	return std::vformat("{}.{}.{}.{}", std::make_format_args(worldId, funcId, zoneId, instId));
}

int AppId::getWorldId(int appid)
{
	return (appid & 0xFF);
}

int AppId::getFuncId(int appid)
{
	return ((appid >> 8) & 0xFF);
}

int AppId::getZoneId(int appid)
{
	return ((appid >> 16) & 0xFF);
}

int AppId::getInstId(int appid)
{
	return ((appid >> 24) & 0xFF);
}

int AppId::makeId(uint8_t worldId, uint8_t funcId, uint8_t zoneId, uint8_t instId)
{
	uint32_t intAppId = instId;
	intAppId = intAppId << 8;
	intAppId |= zoneId;
	intAppId = intAppId << 8;
	intAppId |= funcId;
	intAppId = intAppId << 8;
	intAppId |= worldId;

	return intAppId;
}

} //namespace zq