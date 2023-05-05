#pragma once


#include <async_simple/coro/Lazy.h>
#include <async_simple/coro/SyncAwait.h>

#include <chrono>
#include <deque>

#if defined(ENABLE_SSL)
#include <asio/ssl.hpp>
#endif

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/read_until.hpp>
#include <asio/write.hpp>


namespace zq {


template <typename Arg, typename Derived>
class CallbackAwaitorBase
{
private:
	template <typename Op>
	class CallbackAwaitorImpl
	{
	public:
		CallbackAwaitorImpl(Derived& awaitor, const Op& op) noexcept
				:
				awaitor(awaitor), op(op) {}

		constexpr bool await_ready() const noexcept { return false; }

		void await_suspend(std::coroutine_handle<> handle) noexcept
		{
			awaitor.coro_ = handle;
			op(AwaitorHandler{ &awaitor });
		}

		decltype(auto) await_resume() noexcept
		{
			if constexpr (std::is_void_v<Arg>)
			{
				return;
			}
			else
			{
				return std::move(awaitor.arg_);
			}
		}

		auto coAwait(async_simple::Executor* executor) const noexcept
		{
			return *this;
		}

	private:
		Derived& awaitor;
		const Op& op;
	};

public:
	class AwaitorHandler
	{
	public:
		AwaitorHandler(Derived* obj) :
				obj(obj) {}
		AwaitorHandler(AwaitorHandler&&) = default;
		AwaitorHandler(const AwaitorHandler&) = default;
		AwaitorHandler& operator=(const AwaitorHandler&) = default;
		AwaitorHandler& operator=(AwaitorHandler&&) = default;

		template <typename... Args>
		void setValueThenResume(Args&&... args) const
		{
			setValue(std::forward<Args>(args)...);
			resume();
		}

		template <typename... Args>
		void setValue(Args&&... args) const
		{
			if constexpr (!std::is_void_v<Arg>)
			{
				obj->arg_ = { std::forward<Args>(args)... };
			}
		}

		void resume() const { obj->coro_.resume(); }

	private:
		Derived* obj;
	};

	template <typename Op>
	CallbackAwaitorImpl<Op> await_resume(const Op& op) noexcept
	{
		return CallbackAwaitorImpl<Op>{ static_cast<Derived&>(*this), op };
	}

private:
	std::coroutine_handle<> coro_;
};

template <typename Arg>
class CallbackAwaitor : public CallbackAwaitorBase<Arg, CallbackAwaitor<Arg>>
{
	friend class CallbackAwaitorBase<Arg, CallbackAwaitor<Arg>>;

private:
	Arg arg_;
};

template <>
class CallbackAwaitor<void> : public CallbackAwaitorBase<void, CallbackAwaitor<void>>
{};



class AsioCoroutineUtil
{
public:

	static inline async_simple::coro::Lazy<std::error_code> asyncAccept( asio::ip::tcp::acceptor& acceptor, asio::ip::tcp::socket& socket) noexcept
	{
		CallbackAwaitor<std::error_code> awaitor;
		co_return co_await awaitor.await_resume([&](auto handler) {
			acceptor.async_accept(socket, [&, handler](const auto& ec) mutable {
				handler.setValueThenResume(ec);
			});
		});
	}


	template <typename Socket, typename AsioBuffer>
	static inline async_simple::coro::Lazy<std::pair<std::error_code, size_t>>
	asyncReadSome(Socket& socket, AsioBuffer&& buffer) noexcept
	{
		CallbackAwaitor<std::pair<std::error_code, size_t>> awaitor;
		co_return co_await awaitor.await_resume([&](auto handler) {
			socket.async_read_some(buffer, [&, handler](const auto& ec, auto size) {
				handler.setValueThenResume(ec, size);
			});
		});
	}

	template <typename Socket, typename AsioBuffer>
	static inline async_simple::coro::Lazy<std::pair<std::error_code, size_t>> asyncRead(Socket& socket, AsioBuffer&& buffer) noexcept
	{
		CallbackAwaitor<std::pair<std::error_code, size_t>> awaitor;
		co_return co_await awaitor.await_resume([&](auto handler) {
			asio::async_read(socket, buffer, [&, handler](const auto& ec, auto size) {
				handler.setValueThenResume(ec, size);
			});
		});
	}

	template <typename Socket, typename AsioBuffer>
	static inline async_simple::coro::Lazy<std::pair<std::error_code, size_t>> asyncRead(Socket& socket, AsioBuffer& buffer, size_t sizeToRead) noexcept
	{
		CallbackAwaitor<std::pair<std::error_code, size_t>> awaitor;
		co_return co_await awaitor.await_resume([&](auto handler) {
			asio::async_read(socket, buffer, asio::transfer_exactly(sizeToRead),
					[&, handler](const auto& ec, auto size) {
						handler.setValueThenResume(ec, size);
					});
		});
	}

	template <typename Socket, typename AsioBuffer>
	static inline async_simple::coro::Lazy<std::pair<std::error_code, size_t>>
	asyncReadUntil(Socket& socket, AsioBuffer& buffer, asio::string_view delim) noexcept
	{
		CallbackAwaitor<std::pair<std::error_code, size_t>> awaitor;
		co_return co_await awaitor.await_resume([&](auto handler) {
			asio::async_read_until(socket, buffer, delim,
					[&, handler](const auto& ec, auto size) {
						handler.setValueThenResume(ec, size);
					});
		});
	}

	template <typename Socket, typename AsioBuffer>
	static inline async_simple::coro::Lazy<std::pair<std::error_code, size_t>> asyncWrite(Socket& socket, AsioBuffer&& buffer) noexcept
	{
		CallbackAwaitor<std::pair<std::error_code, size_t>> awaitor;
		co_return co_await awaitor.await_resume([&](auto handler) {
			asio::async_write(socket, buffer, [&, handler](const auto& ec, auto size) {
				handler.setValueThenResume(ec, size);
			});
		});
	}

	static inline async_simple::coro::Lazy<std::error_code> asyncConnect(asio::ip::tcp::socket& socket, const std::string& host, const std::string& port) noexcept
	{
		CallbackAwaitor<std::error_code> awaitor;
		auto ioContext = socket.get_executor();
		asio::ip::tcp::resolver resolver(ioContext);
		asio::ip::tcp::resolver::iterator iterator;
		auto ec = co_await awaitor.await_resume([&](auto handler) {
			resolver.async_resolve(host, port, [&, handler](auto ec, auto it) {
				iterator = it;
				handler.setValueThenResume(ec);
			});
		});

		if (ec)
		{
			co_return ec;
		}

		co_return co_await awaitor.await_resume([&](auto handler) {
			asio::async_connect(socket, iterator,
					[&, handler](const auto& ec, const auto&) mutable {
						handler.setValueThenResume(ec);
					});
		});
	}

	template <typename Socket>
	static inline async_simple::coro::Lazy<void> asyncClose(Socket& socket) noexcept
	{
		CallbackAwaitor<void> awaitor;
		auto& executor = socket.get_executor();
		co_return co_await awaitor.await_resume([&](auto handler) {
			asio::post(executor, [&, handler]() {
				asio::error_code ignored_ec;
				socket.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
				socket.close(ignored_ec);
				handler.resume();
			});
		});
	}

#if defined(ENABLE_SSL)
	static inline async_simple::coro::Lazy<std::error_code> asyncHandshake(auto& ssl_stream, asio::ssl::stream_base::handshake_type type) noexcept
	{
		CallbackAwaitor<std::error_code> awaitor;
		co_return co_await awaitor.await_resume([&](auto handler) {
			ssl_stream->async_handshake(type, [&, handler](const auto& ec) {
				handler.setValueThenResume(ec);
			});
		});
	}
#endif

};

}
