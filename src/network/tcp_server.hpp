#pragma once


#include "network/tcp_connection.hpp"
#include "network/custom_connection.hpp"


namespace zq {


template <typename ConnectionT>
class TcpServer
{
public:
	static_assert(std::is_base_of_v<BaseConnection<ConnectionT>, ConnectionT>);

	using ClientConnectedCallbackT = typename ConnectionT::ClientConnectedCallbackT;
	using ClientDisconnectedCallbackT = typename ConnectionT::ClientDisconnectedCallbackT;
	using DataReceivedCallbackT = typename ConnectionT::DataReceivedCallbackT;

	TcpServer(asio::io_context& ioContext, const std::string& host, uint16_t port) 
		:
			m_ioContext(ioContext),
			m_acceptor(m_ioContext),
			m_socket(ioContext),
			m_host(host),
			m_port(port),
			m_closed(true),
			m_connectionId(0)
    {
    }
	~TcpServer()
	{

	}

	bool start()
	{
		if (!m_closed)
		{
			return false;
		}

		if (!listen())
		{
			return false;
		}

		m_closed = false;
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
	void setClientDataReceivedCb(const DataReceivedCallbackT& cb) { m_onDataReceived = cb; }

private:
	bool listen()
	{
		asio::error_code errorCode;
		asio::ip::tcp::endpoint endpoint{ asio::ip::address::from_string(m_host, errorCode), m_port };
		if (errorCode)
		{
			LOG_ERROR(s_logCategory, "listen error address::from_string, host:{}:{}, errormsg: {}", m_host, m_port, errorCode.message());
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
			LOG_ERROR(s_logCategory, "Could not bind to host:{}:{}, error:{}", m_host, m_port, errorCode.message());
			return false;
		}

		m_acceptor.listen(asio::socket_base::max_connections, errorCode);
		if (errorCode)
		{
			LOG_ERROR(s_logCategory, "acceptor.listen error,host:{}:{}, errormsg: {}.", m_host, m_port, errorCode.message().c_str());
			return false;
		}

		LOG_INFO(s_logCategory, "start listen on: {}:{}.", m_host, m_port);
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
				std::shared_ptr<ConnectionT> conn = std::make_shared<ConnectionT>(m_ioContext, std::move(m_socket), m_connectionId++, false);
				conn->setClientConnectedCb(m_onClientConnected);
				conn->setClientDisconnectedCb(m_onClientDisconnected);
				conn->setDataReceivedCb(m_onDataReceived);
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

	std::string m_host;
	uint16_t m_port;
	bool m_closed;
	asio::io_context& m_ioContext;
	asio::ip::tcp::acceptor m_acceptor;

	int64_t m_connectionId;
	asio::ip::tcp::socket m_socket;

	ClientConnectedCallbackT m_onClientConnected;
	ClientDisconnectedCallbackT m_onClientDisconnected;
	DataReceivedCallbackT m_onDataReceived;

	constexpr static std::string_view s_logCategory = "TcpServer";
};


}

