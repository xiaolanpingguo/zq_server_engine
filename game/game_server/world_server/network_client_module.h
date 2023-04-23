#pragma once


#include "server/network/tcp_client.hpp"


namespace zq
{

struct TcpClientConfig
{
	std::string ip;
	uint16_t port = 0;
};

class NetworkClientModule
{
public:
	NetworkClientModule(asio::io_context& ioContext);
	~NetworkClientModule();

public:
	bool init(const TcpClientConfig& config);
	void run();
	void stop();

private:
	void onConnectToServerCallback(ConnectionPtr connection, bool success, const std::string& errorMsg);

private:
	asio::io_context& m_ioContext;
	std::shared_ptr<TcpClient> m_tcpClient;
	std::shared_ptr<TcpConnection> m_connection;

	constexpr static std::string_view s_logCategory = "NetworkServerModule";
};

} //namespace zq
