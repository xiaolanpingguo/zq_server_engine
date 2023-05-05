#pragma once


#include "common/log.hpp"
#include <async_simple/Executor.h>
#include <asio/io_context.hpp>
#include <asio/post.hpp>


namespace zq{


class AsioExecutor : public async_simple::Executor
{
public:
	AsioExecutor(asio::io_context& io_context) :
			io_context_(io_context) {}

	virtual bool schedule(Func func) override
	{
		asio::post(io_context_, std::move(func));
		return true;
	}

private:
	asio::io_context& io_context_;
};

} //namespace zq
