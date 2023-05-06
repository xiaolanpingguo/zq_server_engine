#pragma once


#include <asio/io_context.hpp>
#include <cinatra/http_server.hpp>



namespace zq{


/*

	use :

	HttpServer* server = new HttpServer(m_thisServer->getIoContext());
	server->set_http_handler<cinatra::GET, cinatra::POST>("/", [](cinatra::request& req, cinatra::response& res) {
		res.set_status_and_content(cinatra::status_type::ok, "hello world");
	});
	server->listen("0.0.0.0", "9999");

*/

class ServerCommonIOService
{
public:
	explicit ServerCommonIOService(asio::io_context& ioContext) :
			m_ioContext(ioContext)
	{
	}

	void run() { }

	intptr_t run_one() { return 0; }

	intptr_t poll() { return 0; }

	intptr_t poll_one() { return 0; }

	void stop(){}

	asio::io_service& get_io_service() { return m_ioContext; }

private:

	asio::io_context& m_ioContext;
};

using HttpServer = cinatra::http_server_<cinatra::NonSSL, ServerCommonIOService>;
using HttpSslServer = cinatra::http_server_<cinatra::SSL, ServerCommonIOService>;


} //namespace zq