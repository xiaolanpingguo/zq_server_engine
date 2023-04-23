#pragma once


#include "server/network/tcp_connection.hpp"
#include "server/network/custom_connection.hpp"


namespace zq {


using ConnectToServerCallbackT = std::function<void(ConnectionPtr connection, bool success, const std::string& errorMsg)>;
class TcpClient : public std::enable_shared_from_this<TcpClient>
{
public:

    TcpClient(asio::io_context& ioContext, const std::string& ip, uint16_t port, ConnectionType type, int connectTimeoutMs = 1000) 
		:
			m_ioContext(ioContext),
			m_socket(m_ioContext),
			m_resolver(m_ioContext),
			m_deadline(m_ioContext),
			m_ip(ip),
			m_port(port),
			m_type(type),
			m_connectTimeoutMs(connectTimeoutMs)
	{
    }

	~TcpClient()
	{
		asio::error_code igored_ec;
		m_socket.close(igored_ec);
		m_deadline.cancel(igored_ec);
	}

	void asyncConnect()
	{
		asio::error_code errorCode;
		auto addr = asio::ip::address::from_string(m_ip, errorCode);
		if (errorCode)
		{
			LOG_ERROR(s_logCategory, "asyncConnect address::from_string: {}", errorCode.message());
			return;
		}

		m_deadline.cancel();
		m_deadline.expires_after(std::chrono::milliseconds(m_connectTimeoutMs));
		m_deadline.async_wait(std::bind(&TcpClient::checkDeadline, this->shared_from_this()));

		resetSocket();
		m_resolver.async_resolve(m_ip, std::to_string(m_port),
				[this, self = shared_from_this()](const asio::error_code& ec, asio::ip::tcp::resolver::results_type results) 
				{
					if (ec)
					{
						LOG_ERROR(s_logCategory, "resolve failed, ip:{}:{}", m_ip, m_port);
						return;
					}

					asio::async_connect(m_socket, results, [this, self = shared_from_this()](const asio::error_code& ec, const asio::ip::tcp::endpoint&) 
					{
						if (m_socket.is_open() && !ec)
						{
							LOG_INFO(s_logCategory, "connect {}:{} success!", m_ip, m_port);

							ConnectionPtr connection = nullptr;
							if (m_type == ConnectionType::TcpConnection)
							{
								connection = std::make_shared<TcpConnection>(m_ioContext, std::move(m_socket), 0);
							}
							else if (m_type == ConnectionType::CustomConnection)
							{
								connection = std::make_shared<CustomConnection>(m_ioContext, std::move(m_socket), 0);
							}
							else
							{
								LOG_ERROR(s_logCategory, "connection type not supported: {}", (int)m_type);
								assert(0);
								return;
							}

							if (m_connectCb)
							{
								m_connectCb(connection, true, "");
							}
						}
						else
						{
							// The async_connect() function automatically opens the socket at the start
							// of the asynchronous operation. If the socket is closed at this time then
							// the timeout handler must have run first.
							if (!m_socket.is_open())
							{
								LOG_ERROR(s_logCategory, "connecte {}:{} time out!", m_ip, m_port);
							}

							if (ec)
							{
								LOG_ERROR(s_logCategory, "asyncConnect error: {}", ec.message());
							}

							if (m_connectCb)
							{
								m_connectCb(nullptr, false, std::format("connecte {}:{} failed, error msg:{}!", m_ip, m_port, ec ? ec.message() : "unkown error."));
							}

							if (m_retryCount <= 0)
							{
								return;
							}

							if (m_retryCount > 0)
							{
								m_retryCount--;
							}

							asyncConnect();
						}
					});
				});
	}

	void setConnectedCb(const ConnectToServerCallbackT& cb) { m_connectCb = cb; }

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

	void checkDeadline()
	{
		if (!m_socket.is_open())
		{
			return;
		}

		// Check whether the deadline has passed. We compare the deadline against
		// the current time since a new asynchronous operation may have moved the
		// deadline before this actor had a chance to run.
		if (m_deadline.expiry() <= asio::steady_timer::clock_type::now())
		{
			// The deadline has passed. The socket is closed so that any outstanding
			// asynchronous operations are cancelled.
			m_socket.close();

			// There is no longer an active deadline. The expiry is set to the
			// maximum time point so that the actor takes no action until a new
			// deadline is set.
			m_deadline.expires_at(asio::steady_timer::time_point::max());
		}

		// Put the actor back to sleep.
		m_deadline.async_wait(std::bind(&TcpClient::checkDeadline, this->shared_from_this()));
	}

private:

	asio::io_context& m_ioContext;
	asio::ip::tcp::resolver m_resolver;
	asio::ip::tcp::socket m_socket;
	asio::steady_timer m_deadline;

	std::string m_ip;
	uint16_t m_port = 0;

	int m_connectTimeoutMs = 5000;
	int m_retryCount = 3;

	std::function<void(ConnectionPtr connection, bool success, const std::string& errorMsg)> m_connectCb;

	ConnectionType m_type;
	constexpr static std::string_view s_logCategory = "TcpClientService";
};


}

