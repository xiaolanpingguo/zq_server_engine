#pragma once


#include "server/network/base_connection.hpp"
#include "common/byte_convert.hpp"


namespace zq {


class TcpConnection : public BaseConnection
{
#pragma pack(1)
	struct Header
	{
		uint32_t bodyLen;
	};
#pragma pack()
	static constexpr int HEAD_LENTH = sizeof(Header);

public:

	explicit TcpConnection(asio::io_context& ioContext, asio::ip::tcp::socket&& socket, uint64_t connectionId) 
		:
			BaseConnection(ioContext, std::move(socket), connectionId)
	{
	}

	virtual ~TcpConnection()
	{
	}

	TcpConnection(const TcpConnection& c) = delete;
	const TcpConnection& operator=(const TcpConnection&) = delete;


	void sendData(const void* data, uint32_t len) override
	{
		if (data == nullptr || len <= 0 || len > g_maxPacketLenth)
		{
			return;
		}

		Header header = { len };
		MessageBuffer buffer(len + HEAD_LENTH);
		buffer.write(&header, HEAD_LENTH);
		buffer.write(data, header.bodyLen);
		queuePacket(std::move(buffer));
	}

protected:

	void asyncRead() override
	{
		if (!isOpen())
		{
			return;
		}

		readHeader();
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

					readBody(header->bodyLen);
				});
	}

	void readBody(uint32_t size)
	{
		if (m_body.size() < size)
		{
			m_body.resize(size);
		}

		asio::async_read(m_socket, asio::buffer(m_body.data(), size),
				[this, self = this->shared_from_this()](const asio::error_code& ec, std::size_t bytes_transferred) 
				{
					if (ec || !m_socket.is_open())
					{
						LOG_INFO("read body error: ip:{}:{}, error:{}", m_ip, m_port, ec.message())
						close();
						return;
					}

					if (m_onClientDataReceived)
					{
						m_onClientDataReceived(self, m_body.data(), m_body.size());
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

	constexpr static std::string_view s_logCategory = "TcpConnection";
};

}