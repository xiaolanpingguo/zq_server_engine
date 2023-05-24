#include "game_common/command_module.h"
#include "common/string.hpp"
#if defined(PLATFORM_WIN)
#include <windows.h>
#endif


namespace zq{


#if defined(PLATFORM_WIN)
static bool readWinConsole(std::string& str)
{
	static char buf[64] = { 0 };
	HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
	DWORD read = 0;

	if (!ReadConsoleA(hConsole, buf, sizeof(buf) - 1, &read, nullptr) || read == 0)
	{
		return false;
	}

	str = buf;
	memset(buf, 0, sizeof(buf));
	return true;
}

static void shutdownCLIThread(std::thread& cliThread)
{
	if (!CancelSynchronousIo(cliThread.native_handle()))
	{
		// if CancelSynchronousIo() fails, print the error and try with old way
		DWORD errorCode = GetLastError();

		// if CancelSynchronousIo fails with ERROR_NOT_FOUND then there was nothing to cancel, proceed with shutdown
		if (errorCode != ERROR_NOT_FOUND)
		{
			LPCSTR errorBuffer;
			DWORD numCharsWritten = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr, errorCode, 0, (LPTSTR)&errorBuffer, 0, nullptr);
			if (!numCharsWritten)
				errorBuffer = "Unknown error";

			if (numCharsWritten)
				LocalFree((LPSTR)errorBuffer);

			// send keyboard input to safely unblock the CLI thread
			INPUT_RECORD b[4];
			HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
			b[0].EventType = KEY_EVENT;
			b[0].Event.KeyEvent.bKeyDown = TRUE;
			b[0].Event.KeyEvent.uChar.AsciiChar = 'X';
			b[0].Event.KeyEvent.wVirtualKeyCode = 'X';
			b[0].Event.KeyEvent.wRepeatCount = 1;

			b[1].EventType = KEY_EVENT;
			b[1].Event.KeyEvent.bKeyDown = FALSE;
			b[1].Event.KeyEvent.uChar.AsciiChar = 'X';
			b[1].Event.KeyEvent.wVirtualKeyCode = 'X';
			b[1].Event.KeyEvent.wRepeatCount = 1;

			b[2].EventType = KEY_EVENT;
			b[2].Event.KeyEvent.bKeyDown = TRUE;
			b[2].Event.KeyEvent.dwControlKeyState = 0;
			b[2].Event.KeyEvent.uChar.AsciiChar = '\r';
			b[2].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
			b[2].Event.KeyEvent.wRepeatCount = 1;
			b[2].Event.KeyEvent.wVirtualScanCode = 0x1c;

			b[3].EventType = KEY_EVENT;
			b[3].Event.KeyEvent.bKeyDown = FALSE;
			b[3].Event.KeyEvent.dwControlKeyState = 0;
			b[3].Event.KeyEvent.uChar.AsciiChar = '\r';
			b[3].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
			b[3].Event.KeyEvent.wVirtualScanCode = 0x1c;
			b[3].Event.KeyEvent.wRepeatCount = 1;
			DWORD numb;
			WriteConsoleInput(hStdIn, b, 4, &numb);
		}
	}
}

#endif

CommandModule::CommandModule() :
		m_threadStop(false)
{
}

CommandModule::~CommandModule()
{
}

bool CommandModule::init()
{
	m_thread = std::make_unique<std::thread>(std::bind(&CommandModule::cmdThrad, this));
	return true;
}

bool CommandModule::update(uint64_t delta)
{
	processCmd();
	return true;
}

bool CommandModule::finalize()
{
	m_threadStop = true;
	m_queue.stopWait();
	if (m_thread)
	{
		shutdownCLIThread(*m_thread);
		m_thread->join();
	}

	return true;
}

bool CommandModule::registerCmd(const std::string& cmd, const CmdCallBackFunT& cb)
{
	auto it = m_handlers.find(cmd);
	if (it != m_handlers.end())
	{
		return false;
	}

	m_handlers[cmd] = cb;
	return true;
}

void CommandModule::dispatch(const std::string& strCmd)
{
	try
	{
		if (strCmd.empty())
		{
			return;
		}

		LOG_INFO(s_logCategory, "processing cmd:{}", strCmd);
		std::string cmd = strCmd;
		std::vector<std::string> args = strutils::split<std::string>(strCmd, " ");
		if (!args.empty())
		{
			cmd = args[0];
			args.erase(args.begin());
		}

		auto it = m_handlers.find(cmd);
		if (it != m_handlers.end())
		{
			it->second(args);
		}
		else
		{
			LOG_WARN(s_logCategory, "can't find cmd:{}, please check if the cmd has registed", strCmd);
		}
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(s_logCategory, "process cmd error, cmd:{}, exception:{}", strCmd, e.what());
	}
}

void CommandModule::processCmd()
{
	std::string cmd;
	if (m_queue.pop(cmd))
	{
		dispatch(cmd);
	}
}

void CommandModule::cmdThrad()
{
	static char buf[64] = { 0 };
	while (!m_threadStop)
	{
		fflush(stdout);

		std::string command;

#if defined(PLATFORM_WIN)
		if (!readWinConsole(command))
		{
			continue;
		}
#else
		std::this_thread::sleep_for(std::chrono::milliseconds(33));
		//char* command_str = readline("");
		//::rl_bind_key('\t', ::rl_complete);
		//if (command_str != nullptr)
		//{
		//	command = command_str;
		//	free(command_str);
		//}
#endif

		if (command.empty())
		{
			continue;
		}

		m_queue.push(command);
	}
}


}