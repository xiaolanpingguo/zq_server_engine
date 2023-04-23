#pragma once


#include "server/network/base_connection.hpp"


namespace zq {


class CustomConnection : public BaseConnection
{
public:

	explicit CustomConnection(asio::io_context& ioContext, asio::ip::tcp::socket&& socket, uint64_t connectionId) 
		:
			BaseConnection(ioContext, std::move(socket), connectionId)
	{

	}
	virtual ~CustomConnection()
	{
	}

	CustomConnection(const CustomConnection& c) = delete;
	const CustomConnection& operator=(const CustomConnection&) = delete;

	void sendData(const void* data, uint32_t len) override
	{
		if (data == nullptr || len <= 0)
		{
			return;
		}

		MessageBuffer buffer(len);
		buffer.write(data, len);
		queuePacket(std::move(buffer));
	}

protected:

	void asyncRead() override
	{
		if (!isOpen())
		{
			return;
		}

		auto self(shared_from_this());
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
					if (m_onClientDataReceived)
					{
						m_onClientDataReceived(self, m_readBuffer.getReadPointer(), m_readBuffer.getActiveSize());
					}

					// if user close the socket on callback
					if (!isOpen())
					{
						return;
					}

					asyncRead();
				});
	}

private:

	MessageBuffer m_readBuffer;

	constexpr static std::string_view s_logCategory = "SocketConnection";
};

}