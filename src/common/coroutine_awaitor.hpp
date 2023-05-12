#pragma once


#include <async_simple/coro/Lazy.h>
#include <async_simple/coro/SyncAwait.h>


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
	CallbackAwaitorImpl<Op> awaitResume(const Op& op) noexcept
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

}
