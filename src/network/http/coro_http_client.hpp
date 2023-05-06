#pragma once


#include <cinatra/coro_http_client.hpp>


namespace zq{


/*
    use:

	CoroHttpClient client(m_thisServer->getIoContext().get_executor());
	std::string uri = "http://www.baidu.com";
	auto result = co_await client.async_get(uri);
	if (result.net_err)
	{
		std::cout << result.net_err.message() << "\n";
	}
*/

using CoroHttpClient = cinatra::coro_http_client;

}
