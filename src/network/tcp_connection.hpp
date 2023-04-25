#pragma once


#include "network/base_connection.hpp"
#include "common/byte_convert.hpp"

#include <asio/read.hpp>


namespace zq {

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
class TcpConnection : public BaseConnection<TcpConnection>
{
#pragma pack(1)
	struct Header
	{
		uint32_t bodyLen;
		uint16_t msgId;
	};
#pragma pack()
	static constexpr int HEAD_LENTH = sizeof(Header);

public:

	// use for server
	using ClientConnectedCallbackT = std::function<void(TcpConnectionPtr connection)>;
	using ClientDisconnectedCallbackT = std::function<void(TcpConnectionPtr connection)>;

	// use for client
	using ConnectToServerCallbackT = std::function<void(TcpConnectionPtr connection, const std::string& errorMsg)>;
	using DisconnectFromServerCallbackT = std::function<void(TcpConnectionPtr connection)>;

	// use for client/server
	using DataReceivedCallbackT = std::function<void(TcpConnectionPtr connection, uint16_t msgId, const void* data, uint32_t len)>;

	explicit TcpConnection(asio::io_context& ioContext, asio::ip::tcp::socket&& socket, uint64_t connectionId, bool isClient = false) 
		:
			BaseConnection<TcpConnection>(ioContext, std::move(socket), connectionId, isClient)
	{
	}

	virtual ~TcpConnection()
	{
	}

	TcpConnection(const TcpConnection& c) = delete;
	const TcpConnection& operator=(const TcpConnection&) = delete;

	void sendData(uint16_t msgId, const void* data, uint32_t len)
	{
		if (data == nullptr || len <= 0 || len > g_maxPacketLenth)
		{
			return;
		}

		Header header = { len, msgId };
		MessageBuffer buffer(len + HEAD_LENTH);
		buffer.write(&header, HEAD_LENTH);
		buffer.write(data, header.bodyLen);
		queuePacket(std::move(buffer));
	}

	void setClientConnectedCb(const ClientConnectedCallbackT& cb) { m_onClientConnected = cb; }
	void setClientDisconnectedCb(const ClientDisconnectedCallbackT& cb) { m_onClientDisconnected = cb; }
	void setDataReceivedCb(const DataReceivedCallbackT& cb) { m_onDataReceived = cb; }
	void setConnectToServerCb(const ConnectToServerCallbackT& cb) { m_onConnectToServer = cb; }
	void setDisconnectFromServerCb(const DisconnectFromServerCallbackT& cb) { m_onDisconnectFromServer = cb; }

protected:

	void asyncRead() override
	{
		if (!isOpen())
		{
			return;
		}

		readHeader();
	}

	void onClientConnect() override
	{
		if (m_onClientConnected)
		{
			m_onClientConnected(shared_from_this());
		}
	}

	void onClientDisconnect() override
	{
		if (m_onClientDisconnected)
		{
			m_onClientDisconnected(shared_from_this());
		}
	}

	// as client
	void onConnectToServer() override
	{
		if (m_onConnectToServer)
		{
			m_onConnectToServer(shared_from_this(), "");
		}
	}

	// as client
	void onDisconnectFromServer() override
	{
		if (m_onDisconnectFromServer)
		{
			m_onDisconnectFromServer(shared_from_this());
		}
	}

    void readHeader()
	{
		memset(m_head, 0, HEAD_LENTH);
		asio::async_read(m_socket, asio::buffer(m_head, HEAD_LENTH),
			[this, self = this->shared_from_this()](const asio::error_code& ec, std::size_t) 
				{
					if (ec || !m_socket.is_open())
					{
						LOG_INFO("read header error: ip:{}:{}, error:{}", m_ip, m_port, ec.message())
						close();
						return;
					}

					Header* header = (Header*)(m_head);
					if (header->bodyLen == 0 || header->bodyLen > g_maxPacketLenth)
					{
						LOG_INFO("read header error, body lenth exceeded limit: ip:{}:{}, m_headerLength:{}", m_ip, m_port, header->bodyLen)
						close();
						return;
					}

					readBody(header->msgId, header->bodyLen);
				});
	}

	void readBody(uint16_t msgId, uint32_t size)
	{
		if (m_body.size() < size)
		{
			m_body.resize(size);
		}

		asio::async_read(m_socket, asio::buffer(m_body.data(), size),
				[this, self = this->shared_from_this(), msgId](const asio::error_code& ec, std::size_t bytes_transferred) 
				{
					if (ec || !m_socket.is_open())
					{
						LOG_INFO("read body error: ip:{}:{}, error:{}", m_ip, m_port, ec.message())
						close();
						return;
					}

					if (m_onDataReceived)
					{
						m_onDataReceived(self, msgId, m_body.data(), (uint32_t)m_body.size());
					}

					// if user close the socket on callback
					if (!isOpen())
					{
						return;
					}

					readHeader();
				});
	}

private:

	char m_head[HEAD_LENTH];
	std::vector<char> m_body;

	// use for client/server
	DataReceivedCallbackT m_onDataReceived;

	// use for server
	ClientConnectedCallbackT m_onClientConnected;
	ClientDisconnectedCallbackT m_onClientDisconnected;

	// use for client
	ConnectToServerCallbackT m_onConnectToServer;
	DisconnectFromServerCallbackT m_onDisconnectFromServer;

	constexpr static std::string_view s_logCategory = "TcpConnection";
};

}