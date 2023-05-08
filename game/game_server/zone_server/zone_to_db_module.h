#pragma once


#include "game_common/i_module.hpp"
#include "network/tcp_client.hpp"

namespace zq
{


class ZoneServer;
class WorldToDBModule : public IModule
{
	INIT_MODULE_NAME(WorldToDBModule);

public:
	WorldToDBModule(ZoneServer* thisServer);
	~WorldToDBModule();

public:
	bool init() override;
	bool finalize() override;

	void sendToDBServer(uint16_t msgId, const char* data, uint32_t len);

private:
	void onConnectToServerCallback(TcpConnectionPtr connection, const std::string& errorMsg);
	void onDisconnectedFromServer(TcpConnectionPtr connection);
	void onDataReceivedFromServer(TcpConnectionPtr connection, uint16_t msgId, const void* data, size_t len);

private:

private:
	ZoneServer* m_thisServer;
	std::shared_ptr<TcpClient<TcpConnection>> m_tcpClient;
	TcpConnectionPtr m_connection;

	constexpr static std::string_view s_logCategory = "WorldToDBModule";
};

} //namespace zq
