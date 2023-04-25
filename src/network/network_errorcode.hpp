#pragma once

#include <limits>
#include <string>

namespace zq
{
enum class NetworkCommonError : int
{
	READ_MSG_TOO_BIG = 100,
	WRITE_MSG_TOO_BIG = 101,
};

class ErrorCode
{
public:
	ErrorCode(int errorCode = 0, const std::string& msg = "") :
			m_errorCode(errorCode),
			m_msg(msg)
	{
	}

	virtual ~ErrorCode() = default;

	virtual operator bool()
	{
		return m_errorCode != 0;
	}

	bool operator==(const ErrorCode& right) const noexcept
	{
		return this->m_errorCode == right.m_errorCode;
	}

	bool operator!=(const ErrorCode& right) const noexcept
	{
		return !(*this == right);
	}

	virtual int code() const noexcept
	{
		return m_errorCode;
	}

	virtual std::string message() const noexcept
	{
		switch (static_cast<NetworkCommonError>(m_errorCode))
		{
			default:
			case NetworkCommonError::READ_MSG_TOO_BIG:
				return "The socket read message exceeded the locally configured limit";
			case NetworkCommonError::WRITE_MSG_TOO_BIG:
				return "The socket write message exceeded the locally configured limit";
		}
		return "error code: " + std::to_string(m_errorCode) + "msg: " + m_msg + "\n";
	}

private:
	int m_errorCode;
	std::string m_msg;
};

inline ErrorCode makeErrorCode(int ec, const std::string& msg = "unkonw")
{
	return ErrorCode(ec, msg);
}

}