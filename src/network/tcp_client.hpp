#pragma once


#include "network/tcp_connection.hpp"
#include "network/custom_connection.hpp"

#include <asio/connect.hpp>


namespace zq {


template <typename ConnectionT>
class TcpClient : public std::enable_shared_from_this<TcpClient<ConnectionT>>
{
public:

	static_assert(std::is_base_of_v<BaseConnection<ConnectionT>, ConnectionT>);

	using ConnectToServerCallbackT = typename ConnectionT::ConnectToServerCallbackT;
	using DisconnectFromServerCallbackT = typename ConnectionT::DisconnectFromServerCallbackT;
	using DataReceivedCallbackT = typename ConnectionT::DataReceivedCallbackT;

    TcpClient(asio::io_context& ioContext, const std::string& host, uint16_t port, int connectTimeoutMs = 3000, int reconnectSecondsInterval = 5)
		:
			m_ioContext(ioContext),
			m_socket(m_ioContext),
			m_resolver(m_ioContext),
			m_reconnectTimer(m_ioContext),
			m_host(host),
			m_port(port),
			m_connectTimeoutMs(connectTimeoutMs),
			m_reconnectSecondsInterval(reconnectSecondsInterval)
	{
    }

	~TcpClient()
	{
		asio::error_code igored_ec;
		m_socket.close(igored_ec);
	}

	void asyncConnect()
	{
		std::shared_ptr<asio::steady_timer> timer = std::make_shared<asio::steady_timer>(m_ioContext);
		timer->expires_after(std::chrono::milliseconds(m_connectTimeoutMs));
		timer->async_wait(std::bind(&TcpClient::timeout, this->shared_from_this(), std::placeholders::_1));

		resetSocket();
		m_resolver.async_resolve(m_host, std::to_string(m_port),
				[this, self = this->shared_from_this(), timer](const asio::error_code& ec, asio::ip::tcp::resolver::results_type results) 
				{
					if (ec)
					{
						LOG_ERROR(s_logCategory, "resolve failed, host:{}:{}", m_host, m_port);
						if (timer)
						{
							asio::error_code ignore;
							timer->cancel(ignore);
						}

						reconnect();
						return;
					}

					asio::async_connect(m_socket, results, [this, self = this->shared_from_this(), timer](const asio::error_code& ec, const asio::ip::tcp::endpoint&) 
					{
						if (timer)
						{
							asio::error_code ignore;
							timer->cancel(ignore);
						}
		
						if (!ec)
						{
							LOG_INFO(s_logCategory, "connect {}:{} success!", m_host, m_port);

							std::shared_ptr<ConnectionT> conn = std::make_shared<ConnectionT>(m_ioContext, std::move(m_socket), 0, true);
							conn->setConnectToServerCb(m_onConnectToServer);
							conn->setDisconnectFromServerCb(std::bind(&TcpClient::onDisconnectedFromServer, this->shared_from_this(), std::placeholders::_1));
							conn->setDataReceivedCb(m_onDataReceived);
							conn->start();
						}
						else
						{
							// The async_connect() function automatically opens the socket at the start
							// of the asynchronous operation. If the socket is closed at this time then
							// the timeout handler must have run first.
							if (!m_socket.is_open())
							{
								LOG_ERROR(s_logCategory, "connecte {}:{} time out!", m_host, m_port);
							}

							LOG_ERROR(s_logCategory, "asyncConnect error: {}", ec.message());
							if (m_onConnectToServer)
							{
								m_onConnectToServer(nullptr, fmt::format("connecte {}:{} failed, error msg:{}!", m_host, m_port, ec ? ec.message() : "unkown error."));
							}

							reconnect();
						}
					});
				});
	}

	void setConnectToServerCb(const ConnectToServerCallbackT& cb) { m_onConnectToServer = cb; }
	void setDisconnectFromServerCb(const DisconnectFromServerCallbackT& cb) { m_onDisconnectFromServer = cb; }
	void setDataReceivedCb(const DataReceivedCallbackT& cb) { m_onDataReceived = cb; }

private:

	void resetSocket()
	{
		asio::error_code igored_ec;
		m_socket.close(igored_ec);
		m_socket = decltype(m_socket)(m_ioContext);
		if (!m_socket.is_open())
		{
			m_socket.open(asio::ip::tcp::v4());
		}
	}

	void timeout(const asio::error_code& ec)
	{
		if (!m_socket.is_open())
		{
			return;
		}

		if (ec)
		{
			LOG_ERROR(s_logCategory, "error occurred on connect timout, error: {}", ec.message());
			return;
		}

		m_socket.close();
		LOG_ERROR(s_logCategory, "connect to server timout...");
	}

	void reconnect()
	{
		asio::error_code ignore;
		m_reconnectTimer.cancel(ignore);
		m_reconnectTimer.expires_after(std::chrono::seconds(m_reconnectSecondsInterval));
		m_reconnectTimer.async_wait(std::bind(&TcpClient::asyncConnect, this->shared_from_this()));
		LOG_ERROR(s_logCategory, "will to reconnecte to {}:{} in {} seconds later", m_host, m_port, m_reconnectSecondsInterval);
	}

	void onDisconnectedFromServer(std::shared_ptr<ConnectionT> connection)
	{
		if (m_onDisconnectFromServer)
		{
			m_onDisconnectFromServer(connection);
		}

		reconnect();
	}

private:

	asio::io_context& m_ioContext;
	asio::ip::tcp::socket m_socket;
	asio::ip::tcp::resolver m_resolver;
	asio::steady_timer m_reconnectTimer;

	std::string m_host;
	uint16_t m_port;

	int m_connectTimeoutMs;
	int m_reconnectSecondsInterval;

	ConnectToServerCallbackT m_onConnectToServer;
	DisconnectFromServerCallbackT m_onDisconnectFromServer;
	DataReceivedCallbackT m_onDataReceived;

	constexpr static std::string_view s_logCategory = "TcpClientService";
};


}

