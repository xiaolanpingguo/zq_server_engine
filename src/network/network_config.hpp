#pragma once

#include <limits>
#include <string>

namespace zq
{


// my encode/decode policy
#pragma pack(1)
struct Header
{
	uint32_t bodyLen;
	uint16_t msgId;
};
#pragma pack()
static constexpr int HEAD_LENTH = sizeof(Header);


constexpr int g_maxPacketLenth = 65535;


}