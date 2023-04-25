#pragma once


#include "network/base_connection.hpp"


namespace zq {


class CustomConnection;
using CustomConnectionPtr = std::shared_ptr<CustomConnection>;
class CustomConnection : public BaseConnection<CustomConnection>
{
public:

	// use for server
	using ClientConnectedCallbackT = std::function<void(CustomConnectionPtr connection)>;
	using ClientDisconnectedCallbackT = std::function<void(CustomConnectionPtr connection)>;

	// use for client
	using ConnectToServerCallbackT = std::function<void(CustomConnectionPtr connection, const std::string& errorMsg)>;
	using DisconnectFromServerCallbackT = std::function<void(CustomConnectionPtr connection)>;

	// use for client/server
	using DataReceivedCallbackT = std::function<void(CustomConnectionPtr connection, const void* data, uint32_t len)>;

	explicit CustomConnection(asio::io_context& ioContext, asio::ip::tcp::socket&& socket, uint64_t connectionId, bool isClient = false) 
		:
			BaseConnection<CustomConnection>(ioContext, std::move(socket), connectionId, isClient)
	{

	}
	virtual ~CustomConnection()
	{
	}

	CustomConnection(const CustomConnection& c) = delete;
	const CustomConnection& operator=(const CustomConnection&) = delete;

	void sendData(const void* data, uint32_t len)
	{
		if (data == nullptr || len <= 0)
		{
			return;
		}

		MessageBuffer buffer(len);
		buffer.write(data, len);
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

		m_readBuffer.normalize();
		m_readBuffer.ensureFreeSpace();
		m_socket.async_read_some(asio::buffer(m_readBuffer.getWritePointer(), m_readBuffer.getRemainingSpace()),
				[this, self = shared_from_this()](asio::error_code error, size_t transferredBytes)
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

					m_readBuffer.writeCompleted(transferredBytes);
					if (m_onDataReceived)
					{
						m_onDataReceived(self, m_readBuffer.getReadPointer(), (uint32_t)m_readBuffer.getActiveSize());
					}

					// if user close the socket on callback
					if (!isOpen())
					{
						return;
					}

					asyncRead();
				});
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

private:

	// use for client/server
	DataReceivedCallbackT m_onDataReceived;

	// use for server
	ClientConnectedCallbackT m_onClientConnected;
	ClientDisconnectedCallbackT m_onClientDisconnected;

	// use for client
	ConnectToServerCallbackT m_onConnectToServer;
	DisconnectFromServerCallbackT m_onDisconnectFromServer;

	MessageBuffer m_readBuffer;

	constexpr static std::string_view s_logCategory = "SocketConnection";
};

}