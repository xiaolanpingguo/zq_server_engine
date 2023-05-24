#pragma once


#include "common/message_buffer.hpp"
#include "common/log.hpp"
#include "network/network_config.hpp"
#include <assert.h>
#include <time.h>
#include <queue>

#include <asio/ip/tcp.hpp>
#include <asio/write.hpp>


namespace zq {


template <typename ConnectionT>
class BaseConnection : public std::enable_shared_from_this<ConnectionT>
{
public:

	explicit BaseConnection(asio::io_context& ioContext, asio::ip::tcp::socket&& socket, uint64_t connectionId, bool isClient = false) :
			m_ioContext(ioContext),
			m_socket(std::move(socket)),
			m_connectionId(connectionId),
			m_closed(true),
			m_isWritingAsync(false),
			m_isClient(isClient),
			m_delayClose(false)
	{

	}
	virtual ~BaseConnection()
	{
		close();
	}

	BaseConnection(const BaseConnection& c) = delete;
	const BaseConnection& operator=(const BaseConnection&) = delete;

	void start()
	{
		try
		{
			const auto& endpoint = asio::ip::tcp::endpoint(m_socket.remote_endpoint());
			m_host = endpoint.address().to_string();
			m_port = endpoint.port();
			m_socket.non_blocking(true);
			m_socket.set_option(asio::ip::tcp::no_delay(true));
			m_closed = false;

			if (m_isClient)
			{
				onConnectToServer();
			}
			else
			{
				onClientConnect();
			}

			// in case use close socket on callback
			if (!isOpen())
			{
				return;
			}

			asyncRead();
		}
		catch (asio::system_error& e)
		{
			LOG_ERROR(s_logCategory, "async_accept excption: {}:{}", e.what(), e.code().value());
		}
	}

	bool isOpen() { return !m_closed && !m_delayClose && m_socket.is_open(); }

	void close()
	{
		if (m_closed)
		{
			return;
		}

		m_closed = true;

		if (m_isClient)
		{
			this->onDisconnectFromServer();
		}
		else
		{
			this->onClientDisconnect();
		}

		asio::error_code ec;
		m_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
		m_socket.close(ec);
	}

	void delayClose()
	{
		m_delayClose = true;
		asio::error_code ec;
		m_socket.shutdown(asio::ip::tcp::socket::shutdown_receive, ec);
	}

	uint64_t getConnectionId() { return m_connectionId; }
	const std::string& getHost() { return m_host; }
	int getPort() { return m_port; }

protected:

	virtual void asyncRead() = 0;
	virtual void onClientConnect() = 0;
	virtual void onClientDisconnect() = 0;

	// as client
	virtual void onConnectToServer() = 0;
	virtual void onDisconnectFromServer() = 0;

	void queuePacket(MessageBuffer&& buffer)
	{
		m_writeQueue.push(std::move(buffer));
		asyncWrite();
	}

	void asyncWrite()
	{
		if (m_isWritingAsync)
		{
			return;
		}

		m_isWritingAsync = true;

		MessageBuffer& buffer = m_writeQueue.front();
		asio::async_write(m_socket, asio::buffer(buffer.getReadPointer(), buffer.getActiveSize()), std::bind(&BaseConnection::writeHandler, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	}

	void writeHandler(asio::error_code error, std::size_t transferedBytes)
	{
		if (error)
		{
			close();
			return;
		}

		if (!isOpen())
		{
			return;
		}

		if (m_delayClose && m_writeQueue.empty())
		{
			close();
			return;
		}

		m_isWritingAsync = false;

		// asio will ensure transferedBytes == getActiveSize(),
		// so the following if condition will succeed
		m_writeQueue.front().readCompleted(transferedBytes);
		assert(m_writeQueue.front().getActiveSize() == 0 && "m_writeQueue.front().getActiveSize() != 0");
		if (m_writeQueue.front().getActiveSize() == 0)
		{
			m_writeQueue.pop();
		}

		if (!m_writeQueue.empty())
		{
			asyncWrite();
		}
	}

protected:

	asio::io_context& m_ioContext;
	asio::ip::tcp::socket m_socket;
	uint64_t m_connectionId;
	std::string m_host;
	uint16_t m_port;

	bool m_closed;
	bool m_isWritingAsync;
	std::queue<MessageBuffer> m_writeQueue;

	bool m_isClient;
	bool m_delayClose;

	constexpr static std::string_view s_logCategory = "BaseConnection";
};

}