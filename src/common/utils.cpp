#include "common/utils.h"
#include "common/file.hpp"

#if defined(PLATFORM_WIN)
#include <windows.h>
#elif defined(PLATFORM_LINUX)
#include <unistd.h>
#endif

namespace zq {


int Utils::createPIDFile(std::string const& filename)
{
	int pid = getPid();
	if (!File::write(filename, std::to_string(pid)))
	{
		return -1;
	}

	return pid;
}

int Utils::getPid()
{
#if defined(PLATFORM_WIN)
	int pid = (int)GetCurrentProcessId();
#elif defined(PLATFORM_LINUX)
	int pid = getpid();
#else
	int pid = 0;
#endif

	return pid;
}


}
