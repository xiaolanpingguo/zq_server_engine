#pragma once


#include "network/coroutine/coroutine_connection.hpp"
#include "network/coroutine/asio_executor.hpp"


namespace zq
{

class CoroutineServer
{
public:
	CoroutineServer(asio::io_context& ioContext, const std::string& host, uint16_t port) :
			m_ioContext(ioContext),
			m_acceptor(m_ioContext),
			m_executor(m_ioContext),
			m_host(host),
			m_port(port),
			m_connectionId(0)
	{
	}

	~CoroutineServer()
	{
	}

	async_simple::coro::Lazy<bool> start()
	{
		if (!listen())
		{
			co_return false;
		}

		while (1)
		{
			if (!m_acceptor.is_open())
			{
				LOG_ERROR(s_logCategory, "acceptor as closed!!!!.");
				co_return false;
			}

			asio::ip::tcp::socket socket(m_ioContext);
			auto ec = co_await AsioCoroutineUtil::asyncAccept(m_acceptor, socket);
			if (ec)
			{
				LOG_ERROR(s_logCategory , "Accept failed, error: {}.", ec.message());
				continue;
			}

			std::cout << "New client coming.\n";

			auto conn = std::make_shared<CoroutineConnection>(std::move(socket), m_connectionId++, false);
			conn->start().via(&m_executor).detach();
		}
	}

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

private:
	asio::io_context& m_ioContext;
	asio::ip::tcp::acceptor m_acceptor;
	AsioExecutor m_executor;

	int64_t m_connectionId;
	std::string m_host;
	uint16_t m_port;

	constexpr static std::string_view s_logCategory = "CoroutineServer";
};

} //namespace zq
