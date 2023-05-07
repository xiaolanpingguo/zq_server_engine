#pragma once


#include "common/common.hpp"


// appid: x.x.x.x: worldId.funcionId.zoneId.instanceId
namespace zq{


enum AppFunctionId : uint8_t
{
	MASER_SERVER = 0,
	ZONE_SERVER = 1,
	LOGIN_SERVER = 2,
	DB_SERVER = 3,

	SERVER_MAX = 31,
};

struct AppId
{
	bool init(const std::string& appid);

	static std::string convertIntToStr(int appid);
	static int getWorldId(int appid);
	static int getFuncId(int appid);
	static int getZoneId(int appid);
	static int getInstId(int appid);
	static int makeId(uint8_t worldId, uint8_t funcId, uint8_t zoneId, uint8_t instId);

	std::string strAppId;
	uint32_t intAppId;
	uint8_t worldId;
	uint8_t funcId;
	uint8_t zoneId;
	uint8_t instanceId;
};

}
