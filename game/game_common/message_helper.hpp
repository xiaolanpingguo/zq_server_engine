#pragma once


#include "game_common/message_router.hpp"
#include "network/tcp_connection.hpp"


namespace zq {


class MessageHelper
{
public:
	static MessageHelper& getInstance()
	{
		static MessageHelper ins;
		return ins;
	}

	template <auto Func>
	bool registerHandler(class_type_t<decltype(Func)>* self, const MessageRouter<TcpConnection>::HandlerKeyT& key)
	{
		return m_messageRouter.registerHandler<Func>(self, key);
	}

	template <auto Func>
	bool registerHandler(const MessageRouter<TcpConnection>::HandlerKeyT& key)
	{
		return m_messageRouter.registerHandler<Func>(key);
	}

	template <auto Func>
	bool registerHandler1(const MessageRouter<TcpConnection>::HandlerKeyT& key)
	{
		return m_messageRouter.registerHandler<Func>(key);
	}

	bool dispatch(TcpConnectionPtr connection, const MessageRouter<TcpConnection>::HandlerKeyT& key, const char* data, uint32_t len)
	{
		return m_messageRouter.dispatch(connection, key, data, len);
	}

	template <typename ProtoObject>
	bool sendPacket(TcpConnectionPtr connection, uint16_t msgId, ProtoObject& packet)
	{
		std::string strData;
		if (!packet.SerializeToString(&strData))
		{
			LOG_ERROR(s_logCategory, "s2sPackage.SerializeToString failed, msgId:{}.", msgId)
			return false;
		}

		connection->sendData(msgId, strData.data(), (uint32_t)strData.size());
		return true;
	}

private:
	MessageRouter<TcpConnection> m_messageRouter;
	constexpr static std::string_view s_logCategory = "MessageRouter";
};

}