#pragma once


#include "common/type_traits.hpp"
#include "common/log.hpp"

#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>


namespace zq {

template <typename ConnectionT>
class MessageRouter
{
private:
	using HandlerT = std::function<void(std::shared_ptr<ConnectionT>, const char*, uint32_t)>;
	using HandlerKeyT = uint32_t;

	struct ProtobufferProtocol
	{
		template <typename T>
		static bool deserialize(T& t, const char* data, uint32_t len)
		{
			if (!t.ParseFromArray(data, len))
			{
				return false;
			}

			return true;
		}

		template <typename T>
		static bool serialize(const T& t, std::string& data)
		{
			if (!t.SerializeToString(&data))
			{
				return false;
			}

			return true;
		}
	};


public:

	static MessageRouter<ConnectionT>* getInstance()
	{
		static MessageRouter<ConnectionT> ins;
		return &ins;
	}

	HandlerT getHandler(HandlerKeyT id)
	{
		if (auto it = m_handlers.find(id); it != m_handlers.end())
		{
			return it->second;
		}

		return nullptr;
	}

	template <auto Func>
	bool registerHandler(class_type_t<decltype(Func)>* self, const HandlerKeyT& key)
	{
		return registHandlerImpl<Func>(self, key);
	}

	template <auto Func>
	bool registerHandler(const HandlerKeyT& key)
	{
		return registHandlerImpl<Func>(key);
	}

	bool dispatch(std::shared_ptr<ConnectionT> connection, const HandlerKeyT& key, const char* data, uint32_t len)
	{
		auto handler = getHandler(key);
		if (!handler)
		{
			LOG_WARN(s_logCategory, "cannot found msgId:{}.", key);
			return false;
		}

		handler(connection, data, len);
		return true;
	}

	template <typename Connection, typename ProtoObject>
	bool sendPacket(Connection connection, uint16_t msgId, ProtoObject& packet)
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
	template <auto Fun, typename Self>
	bool registHandlerImpl(Self* self, const HandlerKeyT& key)
	{
		auto it = m_handlers.emplace(key, [self, this](std::shared_ptr<ConnectionT> connection, const char* data, uint32_t len) 
			{
			return std::visit([this, connection, data, len, self]<typename ProtoObject>(const ProtoObject& obj) mutable
					{
						return execute<ProtoObject, Fun>(connection, data, len, self);
					},
					m_pbSerialize);
			});
		if (!it.second)
		{
			return false;
		}

		return true;
	}

	template <auto Fun>
	bool registHandlerImpl(const HandlerKeyT& key)
	{
		static_assert(!std::is_member_function_pointer_v<decltype(Fun)>, "register member function but lack of the object pointer");
		using return_type = function_return_type_t<decltype(Fun)>;

		auto it = m_handlers.emplace(key, [this](std::shared_ptr<ConnectionT> connection, const char* data, uint32_t len) 
			{
				return std::visit([this, connection, data, len]<typename ProtoObject>(const ProtoObject& obj) mutable 
						{
							return execute<ProtoObject, Fun>(connection, data, len);
						},
						m_pbSerialize);
			});
		if (!it.second)
		{
			return false;
		}

		return true;
	}

	template <typename ProtoObject, auto Fun, typename Self = void>
	inline bool execute(std::shared_ptr<ConnectionT> connection, const char* data, uint32_t len, Self* self = nullptr)
	{
		using FunT = decltype(Fun);
		using ParamType = function_parameters_t<FunT>;
		using ReturnType = function_return_type_t<FunT>;

		constexpr size_t size = std::tuple_size_v<ParamType>;
		static_assert(size == 2, "param number must be 2 for message handler");

		auto args = ParamType{};

		using First = std::tuple_element_t<0, ParamType>;
		using Second = std::tuple_element_t<1, ParamType>;

		First& conn = std::get<0>(args);
		conn = connection;

		Second& protoObj = std::get<1>(args);
		bool ok = ProtoObject::deserialize(protoObj, data, len);
		if (!ok)
		{
			return false;
		}

		if constexpr (std::is_void_v<Self>)
		{
			std::apply(Fun, connection, protoObj);
		}
		else
		{
			if (self)
			{
				auto& o = *self;

				std::apply(Fun, std::tuple_cat(std::forward_as_tuple(o), std::move(args)));
			}
		}

		return true;
	}

private:

	std::unordered_map<HandlerKeyT, HandlerT> m_handlers;
	std::variant<ProtobufferProtocol> m_pbSerialize;

	constexpr static std::string_view s_logCategory = "MessageRouter";
};

}