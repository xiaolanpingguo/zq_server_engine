#pragma once


#include "server/network/tcp_server.hpp"


namespace zq {

class TcpServer;
struct TcpServerConfig
{
	std::string ip;
	uint16_t port = 0;
};

class NetworkServerModule
{
public:
	NetworkServerModule(asio::io_context& ioContext);
	~NetworkServerModule();

public:
	bool init(const TcpServerConfig& config);
	void run();
	void stop();

private:

	void onClientConnected(ConnectionPtr connection);
	void onClientDisconnected(ConnectionPtr connection);
	void onClientDataReceived(ConnectionPtr connection, const void* data, size_t len);

private:

	asio::io_context& m_ioContext;
	std::unique_ptr<TcpServer> m_tcpServer;

	constexpr static std::string_view s_logCategory = "NetworkServerModule";
};

}
