#pragma once


#include <stdint.h>
#include <iostream>
#include <assert.h>
#include <string.h> // linux: memset
#include <atomic>
#include <filesystem>
#include <functional>
//#include <format> text format support need gcc-13
#include <fstream>
#include <map>
#include "common/format.hpp"
#include "common/concurrent_queue.hpp"


namespace zq{


enum class LogLevel : char
{
	Debug = 1,
	Info = 2,
	Warn = 3,
	Error = 4,
	Max = 5
};

class Log
{
	enum class Status
	{
		INIT,
		READY,
		STOP,
	};
public:
	Log(){}
	~Log()
	{ 
		finalize();
	}

	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;

	static Log& getInstance()
	{
		static Log log;
		return log;
	}

	bool init(const std::string& logName, uint64_t rollSize = 1024 * 1024 * 8)
	{
		if (m_status != Status::INIT)
		{
			return false;
		}

		if (logName.empty())
		{
			return false;
		}

		if (m_ofs)
		{
			printf("cannot call log.init() again!\n");
			return false;
		}

		std::error_code ec;
		constexpr static std::string_view s_logDir = "log/";
		if (!std::filesystem::exists(s_logDir, ec))
		{
			std::filesystem::create_directories(s_logDir, ec);
			if (ec)
			{
				printf("log create_directories failed, path:%s.\n", s_logDir.data());
				return false;
			}
		}

		m_name = std::string(s_logDir) + logName;
		m_rollSizeBytes = rollSize;
		if (!rollFile())
		{
			return false;
		}

		m_thread = std::make_unique<std::thread>(std::bind(&Log::logThread, this));
		m_status = Status::READY;
		return true;
	}

	void finalize()
	{
		if (m_status == Status::STOP)
		{
			return;
		}

		m_status = Status::STOP;
		m_logQueue.stopWait();
		if (m_thread && m_thread->joinable())
		{
			m_thread->join();
		}
	}

	template <typename... Args>
	void logfmt(std::string_view category, LogLevel level, std::string_view file, int line, std::string_view fun, std::string_view fmt, Args&&... args)
	{
		if (m_status != Status::READY || fmt.data() == nullptr || category.data() == nullptr)
		{
			return;
		}

		auto n = std::chrono::system_clock::now();
		auto m = n.time_since_epoch();
		const auto nowMicros = std::chrono::duration_cast<std::chrono::microseconds>(m).count() % 1000000;
		std::time_t t = std::chrono::system_clock::to_time_t(n);
		static char strTime[32] = { 0 };
		size_t endpos = std::strftime(strTime, sizeof(strTime) - 1, "%Y-%m-%d %H:%M:%S", std::localtime(&t));
		snprintf(strTime + endpos, sizeof(strTime) - endpos - 1, ".%06d", (int)nowMicros);

		std::string str;
		if (level <= LogLevel::Info)
		{
			//str = std::vformat("{}-[{}][{}]: ", std::make_format_args(strTime, category, toStringLvel(level)));
			str = fmt::format("{}-[{}][{}]: ", strTime, category, toStringLvel(level));
		}
		else
		{
			//str = std::vformat("{}-[{}][{}][{}:{}:{}]: ", std::make_format_args(strTime, category, toStringLvel(level), file, line, fun));
			str = fmt::format("{}-[{}][{}][{}:{}:{}]: ", strTime, category, toStringLvel(level), file, line, fun);
		}
		//str += std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
		str += fmt::format(fmt, std::forward<Args>(args)...);

		m_logQueue.push(std::move(str));

		++m_count;
		if (level == LogLevel::Error)
		{
			m_errorCount++;
		}

		memset(strTime, 0, sizeof(strTime));
	}

	void setLevel(LogLevel level)
	{
		m_level = level;
	}

	LogLevel getLevel()
	{
		return m_level;
	}

	void setEnableConsole(bool v)
	{
		m_enableStdout = v;
	}

	void setLevel(const std::string& strLevel)
	{
		if (strLevel == "DEBUG")
		{
			setLevel(LogLevel::Debug);
		}
		else if (strLevel == "INFO")
		{
			setLevel(LogLevel::Info);
		}
		else if (strLevel == "WARN")
		{
			setLevel(LogLevel::Warn);
		}
		else if (strLevel == "ERROR")
		{
			setLevel(LogLevel::Error);
		}
		else
		{
			setLevel(LogLevel::Debug);
		}
	}

private:

	bool rollFile()
	{
		if (m_ofs)
		{
			m_ofs->flush();
			m_ofs->close();
		}

		m_bytesWritten = 0;
		m_ofs.reset(new std::ofstream());

		std::string logFileName = m_name;
		auto n = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(n);
		std::ostringstream timeStr;
		timeStr << "_";
		timeStr << std::put_time(std::localtime(&t), "%Y-%m-%d-%H.%M.%S");
		logFileName.append(timeStr.str());
		logFileName.append("#" + std::to_string(++m_fileNumber));
		logFileName.append(".txt");
		m_ofs->open(logFileName, std::ofstream::out | std::ofstream::trunc);
		if (!m_ofs->is_open())
		{
			printf("rollFile: open log file faild, name:%s", logFileName.c_str());
			return false;
		}

		return true;
	}

	uint64_t count() const
	{
		return m_count;
	}

	int64_t errorCount() const
	{
		return m_errorCount;
	}

	void logThread()
	{
		std::string strLog = "";
		while (1)
		{
			if (m_status == Status::STOP && m_logQueue.empty())
			{
				break;
			}

			if (m_logQueue.waitAndPop(strLog))
			{
				writeFile(strLog);
			}
		}

		if (m_ofs && m_ofs->is_open())
		{
			m_ofs->flush();
			m_ofs->close();
			m_ofs = nullptr;
		}
	}

	void writeFile(const std::string& strLog)
	{
#if defined(_WIN32)
		if (m_enableStdout)
		{
			std::cout << strLog << std::endl;
		}
#endif

		if (m_bytesWritten > m_rollSizeBytes)
		{
			rollFile();
		}

		if (m_ofs)
		{
			m_ofs->write(strLog.data(), strLog.size());
			m_ofs->put('\n');
			m_ofs->flush();

			m_bytesWritten += strLog.size();
		}
	}

	static constexpr std::string_view toStringLvel(LogLevel lv)
	{
		switch (lv)
		{
			case LogLevel::Error:
				return "ERROR";
			case LogLevel::Warn:
				return "WARN";
			case LogLevel::Info:
				return "INFO";
			case LogLevel::Debug:
				return "DEBUG";
			default:
				return "NULL";
		}
	}

	bool m_enableStdout = true;
	LogLevel m_level = LogLevel::Debug;
	uint64_t m_count = 0;
	int64_t m_errorCount = 0;
	std::unique_ptr<std::ofstream> m_ofs;
	std::unique_ptr<std::thread> m_thread;
	ConcurrentQueue<std::string> m_logQueue;

	uint32_t m_fileNumber = 0;
	uint64_t m_bytesWritten = 0;
	uint64_t m_rollSizeBytes = 1024 * 1024 * 8;
	std::string m_name;
	std::atomic<Status> m_status = Status::INIT;
};

#define LOG_DEBUG(category, fmt, ...) Log::getInstance().logfmt(category, LogLevel::Debug, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__);
#define LOG_INFO(category, fmt, ...) Log::getInstance().logfmt(category, LogLevel::Info, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__);
#define LOG_WARN(category, fmt, ...) Log::getInstance().logfmt(category, LogLevel::Warn, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__);
#define LOG_ERROR(category, fmt, ...) Log::getInstance().logfmt(category, LogLevel::Error, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__);

}
