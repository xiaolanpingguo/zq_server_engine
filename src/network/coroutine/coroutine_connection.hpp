#pragma once


#include "common/log.hpp"
#include "network/coroutine/asio_coroutine_util.hpp"
#include "network/network_config.hpp"


namespace zq{


class CoroutineConnection;
using CoroutineConnectionPtr = std::shared_ptr<CoroutineConnection>;
class CoroutineConnection : public std::enable_shared_from_this<CoroutineConnection>
{
public:

	// use for client
	using ConnectToServerCallbackT = std::function<void(CoroutineConnectionPtr connection, bool success, std::string errMsg)>;
	using DisconnectedFromServerCallbackT = std::function<void(CoroutineConnectionPtr connection)>;

	// use for server
	using ClientConnectedCallbackT = std::function<void(CoroutineConnectionPtr connection)>;
	using ClientDisconnectedCallbackT = std::function<void(CoroutineConnectionPtr connection)>;

	// use for client/server
	using DataReceivedCallbackT = std::function<async_simple::coro::Lazy<bool>(CoroutineConnectionPtr connection, uint16_t msgId, const void* data, uint32_t len)>;

	// use for server
	CoroutineConnection(asio::ip::tcp::socket&& socket, uint64_t connectionId, bool isClient = false) :
			m_socket(std::move(socket)),
			m_resolver(m_socket.get_executor()),
			m_connectionId(connectionId),
			m_isClient(isClient)
	{
	}

	// use for client
	CoroutineConnection(asio::io_context& ioContext, const std::string& host, uint16_t port, 
		const ConnectToServerCallbackT& cb1, const ClientDisconnectedCallbackT& cb2) :
			m_socket(ioContext),
			m_resolver(ioContext),
			m_connectionId(0),
			m_isClient(true),
			m_host(host),
			m_port(port),
			m_onConnectToServer(cb1),
			m_onDisconnectedFromServer(cb2)

	{
	}

	~CoroutineConnection()
	{
		asio::error_code igored_ec;
		m_socket.close(igored_ec);
	}

	// use for client
	void asyncConnect()
	{
		m_resolver.async_resolve(m_host, std::to_string(m_port),
				[this, self = this->shared_from_this()](const asio::error_code& ec, asio::ip::tcp::resolver::results_type results) 
				{
					if (ec)
					{
						LOG_ERROR(s_logCategory, "resolve failed, host:{}:{}", m_host, m_port);
						return;
					}

					//m_socket.as results
					m_socket.async_connect(results->endpoint(), [this, self = this->shared_from_this()](const asio::error_code& ec) 
					{
						if (ec)
						{
							LOG_INFO(s_logCategory, "connect {}:{} failed!", m_host, m_port);
							if (m_onConnectToServer)
							{
								m_onConnectToServer(nullptr, false, ec.message());
							}
							return;
						}

						asio::error_code err;
						m_socket.set_option(asio::ip::tcp::no_delay(true), err);
						if (err)
						{
							LOG_ERROR(s_logCategory, "socket set no delay failed, host:{}:{},  err:{}!", m_host, m_port, err.message());
							if (m_onConnectToServer)
							{
								m_onConnectToServer(nullptr, false, err.message());
							}
							return;
						}

						LOG_INFO(s_logCategory, "connect {}:{} success!", m_host, m_port);
						if (m_onConnectToServer)
						{
							m_onConnectToServer(self, true, "");
						}			
					});
				});
	}

	// use for server
	async_simple::coro::Lazy<void> start()
	{
		if (m_isClient)
		{
			co_return;
		}

		bool error = false;
		try
		{
			const auto& endpoint = asio::ip::tcp::endpoint(m_socket.remote_endpoint());
			m_host = endpoint.address().to_string();
			m_port = endpoint.port();
			m_socket.set_option(asio::ip::tcp::no_delay(true));
		}
		catch (asio::system_error& e)
		{
			LOG_ERROR(s_logCategory, "async_accept excption: {}:{}", e.what(), e.code().value());
			error = true;
		}

		if (error)
		{
			close();
			co_return;
		}

		if (m_onClientConnected)
		{
			m_onClientConnected(this->shared_from_this());
		}

		// in case use close socket on callback
		if (!isOpen())
		{
			co_return;
		}

		while (1)
		{
			char readHeaderBuffer[HEAD_LENTH] = { 0 };
			auto pair = co_await AsioCoroutineUtil::asyncRead(m_socket, asio::buffer(readHeaderBuffer, HEAD_LENTH));
			if (!checkError(pair.first))
			{
				LOG_ERROR(s_logCategory, "asyncRead head error: {}", pair.first.message());
				break;
			}

			Header* readHeader = (Header*)(readHeaderBuffer);
			if (readHeader->bodyLen == 0 || readHeader->bodyLen > g_maxPacketLenth)
			{
				LOG_INFO("read header error, body lenth exceeded limit: host:{}:{}, m_headerLength:{}", m_host, m_port, readHeader->bodyLen)
				break;
			}

			uint16_t msgId = readHeader->msgId;
			uint32_t bodyLen = readHeader->bodyLen;

			std::vector<char> readBodyBuffer;
			readBodyBuffer.resize(bodyLen);
			pair = co_await AsioCoroutineUtil::asyncRead(m_socket, asio::buffer(readBodyBuffer, bodyLen));
			if (!checkError(pair.first))
			{
				LOG_ERROR(s_logCategory, "asyncRead head error: {}", pair.first.message());
				break;
			}

			bool r = co_await m_onDataReceived(this->shared_from_this(), msgId, readBodyBuffer.data(), bodyLen);
			if (!r)
			{
				LOG_WARN(s_logCategory, "return false from user code, close socket:{}:{}", m_host, m_port);
				break;
			}
		}

		close();
	}

	template <typename T>
	async_simple::coro::Lazy<bool> request(uint16_t msgId, const void* data, uint32_t len, T& obj)
	{
		if (isOpen())
		{
			LOG_ERROR(s_logCategory, "you need to call send if your socket is opened.");
			co_return false;
		}

		if (data == nullptr || len <= 0 || len > g_maxPacketLenth)
		{
			LOG_ERROR(s_logCategory, "data is null or len is illegal: {}", len);
			co_return false;
		}

		auto ec = co_await AsioCoroutineUtil::asyncConnect(m_socket, m_host, std::to_string(m_port));
		if (!checkError(ec))
		{
			close();
			LOG_ERROR(s_logCategory, "Connect error: {}", ec.message());
			co_return false;
		}

		LOG_INFO(s_logCategory, "connect {}:{} success!", m_host, m_port);

		bool success = co_await send(msgId, data, len, obj);
		close();
		co_return success;
	}

	template<typename T>
	async_simple::coro::Lazy<bool> send(uint16_t msgId, const void* data, uint32_t len, T& obj)
	{
		if (data == nullptr || len == 0 || len > g_maxPacketLenth)
		{
			LOG_ERROR(s_logCategory, "data is null or len is illegal: {}", len);
			co_return false;
		}

		if (!isOpen())
		{
			LOG_ERROR(s_logCategory, "socket is not open: host:{};{}", m_host, m_port);
			co_return false;
		}

		// write packet
		Header writeHeader = { len, msgId };
		std::vector<char> writeBuffer;
		writeBuffer.resize(HEAD_LENTH + len);
		memcpy(writeBuffer.data(), &writeHeader, HEAD_LENTH);
		memcpy(writeBuffer.data() + HEAD_LENTH, data, len);

		auto pair = co_await AsioCoroutineUtil::asyncWrite(m_socket, asio::buffer(writeBuffer.data(), writeBuffer.size()));
		if (!checkError(pair.first))
		{
			close();
			LOG_ERROR(s_logCategory, "asyncWrite error: {}", pair.first.message());
			co_return false;
		}

		// read packet head
		char readHeaderBuffer[HEAD_LENTH] = { 0 };
		pair = co_await AsioCoroutineUtil::asyncRead(m_socket, asio::buffer(readHeaderBuffer, HEAD_LENTH));
		if (!checkError(pair.first))
		{
			close();
			LOG_ERROR(s_logCategory, "asyncRead head error: {}", pair.first.message());
			co_return false;
		}

		Header* readHeader = (Header*)(readHeaderBuffer);
		if (readHeader->bodyLen == 0 || readHeader->bodyLen > g_maxPacketLenth)
		{
			LOG_INFO("read header error, body lenth exceeded limit: host:{}:{}, m_headerLength:{}", m_host, m_port, readHeader->bodyLen)
			close();
			co_return false;
		}

		uint16_t readMsgId = readHeader->msgId;
		uint32_t bodyLen = readHeader->bodyLen;

		// read packet body
		std::vector<char> readBodyBuffer;
		readBodyBuffer.resize(bodyLen);
		pair = co_await AsioCoroutineUtil::asyncRead(m_socket, asio::buffer(readBodyBuffer, bodyLen));
		if (!checkError(pair.first))
		{
			close();
			LOG_ERROR(s_logCategory, "asyncRead head error: {}", pair.first.message());
			co_return false;
		}

		if (!obj.ParseFromArray(readBodyBuffer.data(), (uint32_t)readBodyBuffer.size()))
		{
			co_return false;
		}

		LOG_INFO(s_logCategory, "coroutine client send data finished, m_host:{}:{}", m_host, m_port);
		co_return true;
	}

	bool isOpen() { return m_socket.is_open(); }

	void close()
	{
		if (!isOpen())
		{
			return;
		}

		if (m_isClient)
		{
			if (m_onDisconnectedFromServer)
			{
				m_onDisconnectedFromServer(this->shared_from_this());
			}
		}
		else
		{
			if (m_onClientDisconnected)
			{
				m_onClientDisconnected(this->shared_from_this());
			}
		}

		asio::error_code ec;
		m_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
		m_socket.close(ec);
	}

private:
	bool checkError(const std::error_code& ec)
	{
		if (ec)
		{
			return false;
		}

		if (!m_socket.is_open())
		{
			LOG_ERROR(s_logCategory, "socket has closed.");
			return false;
		}

		return true;
	}

private:

	// for client
	asio::ip::tcp::resolver m_resolver;

	asio::ip::tcp::socket m_socket;
	std::string m_host;
	uint16_t m_port;
	uint64_t m_connectionId;

	bool m_isClient;

	// use for client/server
	DataReceivedCallbackT m_onDataReceived;

	// use for server
	ClientConnectedCallbackT m_onClientConnected;
	ClientDisconnectedCallbackT m_onClientDisconnected;

	// use for client
	ConnectToServerCallbackT m_onConnectToServer;
	DisconnectedFromServerCallbackT m_onDisconnectedFromServer;

	constexpr static std::string_view s_logCategory = "CoroutineClient";
};

} //namespace zq
