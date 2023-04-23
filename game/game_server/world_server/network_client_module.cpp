#include "game_server/world_server/network_client_module.h"

namespace zq
{

NetworkClientModule::NetworkClientModule(asio::io_context& ioContext) :
		m_ioContext(ioContext),
		m_tcpClient(nullptr),
		m_connection(nullptr)
{
}

NetworkClientModule::~NetworkClientModule()
{
}

bool NetworkClientModule::init(const TcpClientConfig& config)
{
	using namespace std::placeholders;
	m_tcpClient = std::make_shared<TcpClient>(m_ioContext, config.ip, config.port, ConnectionType::TcpConnection);
	m_tcpClient->setConnectedCb(std::bind(&NetworkClientModule::onConnectToServerCallback, this, _1, _2, _3));
	m_tcpClient->asyncConnect();

	return true;
}

void NetworkClientModule::run()
{
}

void NetworkClientModule::stop()
{
}

void NetworkClientModule::onConnectToServerCallback(ConnectionPtr connection, bool success, const std::string& errorMsg)
{
	if (!success || connection == nullptr)
	{
		LOG_INFO(s_logCategory, "connect to server failed, msg:{}", errorMsg);
		return;
	}

	m_connection = std::dynamic_pointer_cast<TcpConnection>(connection);
	m_connection->start();
	std::string str("hello");
	int a = 10;
	while (a>0)
	{
		m_connection->sendData(str.data(), str.size() + 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		a--;
	}
}


}
