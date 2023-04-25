#pragma once

#include <limits>
#include <string>

namespace zq
{

    
enum class ConnectionType
{
	TcpConnection,
	RpcConnection,
	CustomConnection,
};

constexpr int g_maxPacketLenth = 65535;


}