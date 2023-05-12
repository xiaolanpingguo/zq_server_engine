#pragma once


#include "common/coroutine_awaitor.hpp"

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/read_until.hpp>
#include <asio/write.hpp>

#if defined(ENABLE_SSL)
#include <asio/ssl.hpp>
#endif


namespace zq {


class AsioCoroutineUtil
{
public:

	static inline async_simple::coro::Lazy<std::error_code> asyncAccept( asio::ip::tcp::acceptor& acceptor, asio::ip::tcp::socket& socket) noexcept
	{
		CallbackAwaitor<std::error_code> awaitor;
		co_return co_await awaitor.awaitResume([&](auto handler) {
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
		co_return co_await awaitor.awaitResume([&](auto handler) {
			socket.async_read_some(buffer, [&, handler](const auto& ec, auto size) {
				handler.setValueThenResume(ec, size);
			});
		});
	}

	template <typename Socket, typename AsioBuffer>
	static inline async_simple::coro::Lazy<std::pair<std::error_code, size_t>> asyncRead(Socket& socket, AsioBuffer&& buffer) noexcept
	{
		CallbackAwaitor<std::pair<std::error_code, size_t>> awaitor;
		co_return co_await awaitor.awaitResume([&](auto handler) {
			asio::async_read(socket, buffer, [&, handler](const auto& ec, auto size) {
				handler.setValueThenResume(ec, size);
			});
		});
	}

	template <typename Socket, typename AsioBuffer>
	static inline async_simple::coro::Lazy<std::pair<std::error_code, size_t>> asyncRead(Socket& socket, AsioBuffer& buffer, size_t sizeToRead) noexcept
	{
		CallbackAwaitor<std::pair<std::error_code, size_t>> awaitor;
		co_return co_await awaitor.awaitResume([&](auto handler) {
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
		co_return co_await awaitor.awaitResume([&](auto handler) {
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
		co_return co_await awaitor.awaitResume([&](auto handler) {
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
		auto ec = co_await awaitor.awaitResume([&](auto handler) {
			resolver.async_resolve(host, port, [&, handler](auto ec, auto it) {
				iterator = it;
				handler.setValueThenResume(ec);
			});
		});

		if (ec)
		{
			co_return ec;
		}

		co_return co_await awaitor.awaitResume([&](auto handler) {
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
		co_return co_await awaitor.awaitResume([&](auto handler) {
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
		co_return co_await awaitor.awaitResume([&](auto handler) {
			ssl_stream->async_handshake(type, [&, handler](const auto& ec) {
				handler.setValueThenResume(ec);
			});
		});
	}
#endif

};

}
