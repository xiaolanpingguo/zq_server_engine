#pragma once


#include "server/network/tcp_connection.hpp"
#include "server/network/custom_connection.hpp"


namespace zq {


class TcpServer
{
public:
	TcpServer(asio::io_context& ioContext, const std::string& ip, uint16_t port, ConnectionType type) 
		:
			m_ioContext(ioContext),
			m_acceptor(m_ioContext),
			m_socket(ioContext),
			m_ip(ip),
			m_port(port),
			m_type(type)
    {
    }
	~TcpServer()
	{

	}

	bool start()
	{
		if (!listen())
		{
			return false;
		}

		accept();
		return true;
	}

    void stop()
    {
		if (m_closed)
		{
			return;
		}

		m_closed = true;
		asio::error_code err;
		m_acceptor.close(err);
    }

	void setClientConnectedCb(const ClientConnectedCallbackT& cb) { m_onClientConnected = cb; }
	void setClientDisconnectedCb(const ClientDisconnectedCallbackT& cb) { m_onClientDisconnected = cb; }
	void setClientDataReceivedCb(const ClientDataReceiveCallbackT& cb) { m_onClientDataReceived = cb; }

private:
	bool listen()
	{
		asio::error_code errorCode;
		asio::ip::tcp::endpoint endpoint{ asio::ip::address::from_string(m_ip, errorCode), m_port };
		if (errorCode)
		{
			LOG_ERROR(s_logCategory, "listen error address::from_string, ip:{}:{}, errormsg: {}", m_ip, m_port, errorCode.message());
			return false;
		}

		m_acceptor.open(endpoint.protocol(), errorCode);
		if (errorCode)
		{
			LOG_ERROR(s_logCategory, "Failed to open acceptor, errormsg: {}", errorCode.message());
			return false;
		}

		m_acceptor.bind(endpoint, errorCode);
		if (errorCode)
		{
			LOG_ERROR(s_logCategory, "Could not bind to ip:{}:{}, error:{}", m_ip, m_port, errorCode.message());
			return false;
		}

		m_acceptor.listen(asio::socket_base::max_connections, errorCode);
		if (errorCode)
		{
			LOG_ERROR(s_logCategory, "acceptor.listen error,ip:{}:{}, errormsg: {}.", m_ip, m_port, errorCode.message().c_str());
			return false;
		}

		LOG_INFO(s_logCategory, "start listen on: {}:{}.", m_ip, m_port);
		return true;
	}

	void accept()
	{
		m_acceptor.async_accept(m_socket, [this](const asio::error_code& ec)
		{
			if (!m_acceptor.is_open())
			{
				return;
			}

			if (!ec)
			{
				std::shared_ptr<BaseConnection> conn = nullptr;
				if (m_type == ConnectionType::TcpConnection)
				{
					conn = std::make_shared<TcpConnection>(m_ioContext, std::move(m_socket), m_connectionId);
				}
				else if (m_type == ConnectionType::CustomConnection)
				{
					conn = std::make_shared<CustomConnection>(m_ioContext, std::move(m_socket), m_connectionId);
				}
				else
				{
					LOG_ERROR(s_logCategory, "connection type not supported: {}", (int)m_type);
					return;
				}

				conn->setClientConnectedCb(m_onClientConnected);
				conn->setClientDisconnectedCb(m_onClientDisconnected);
				conn->setClientDataReceivedCb(m_onClientDataReceived);
				conn->start();
			}
			else
			{
				LOG_ERROR(s_logCategory, "async_accept error: {}:{}", ec.message(), ec.value());
			}

			if (!m_closed)
			{
				accept();
			}
		});
	}

private:

	std::string m_ip;
	uint16_t m_port = 0;
	bool m_closed = false;
	asio::io_context& m_ioContext;
	asio::ip::tcp::acceptor m_acceptor;

	int64_t m_connectionId = 0;
	asio::ip::tcp::socket m_socket;

	ClientConnectedCallbackT m_onClientConnected;
	ClientDisconnectedCallbackT m_onClientDisconnected;
	ClientDataReceiveCallbackT m_onClientDataReceived;
	ConnectionType m_type;

	constexpr static std::string_view s_logCategory = "TcpServer";
};


}

