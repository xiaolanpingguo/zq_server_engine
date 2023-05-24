#pragma once


#include "common/common.hpp"
#include "common/concurrent_queue.hpp"
#include "game_common/i_module.hpp"


namespace zq{


using CmdCallBackFunT = std::function<void(const std::vector<std::string>&)>;
class CommandModule : public IModule
{
	INIT_MODULE_NAME(CommandModule);

public:
	CommandModule();
	~CommandModule();

public:
	bool init() override;
	bool update(uint64_t delta) override;
	bool finalize() override;

	bool registerCmd(const std::string& cmd, const CmdCallBackFunT& cb);

private:

	void dispatch(const std::string& cmd);
	void processCmd();
	void cmdThrad();

private:

	std::atomic<bool> m_threadStop;
	std::unique_ptr<std::thread> m_thread;
	ConcurrentQueue<std::string> m_queue;

	std::unordered_map<std::string, CmdCallBackFunT> m_handlers;
	constexpr static std::string_view s_logCategory = "CommandModule";
};

}
