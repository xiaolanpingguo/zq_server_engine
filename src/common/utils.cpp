#include "common/utils.h"
#include "common/file.hpp"

#ifdef PLATFORM_WIN
#include <windows.h>
#elif PLATFORM_LINUX
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
#ifdef PLATFORM_WIN
	int pid = (int)GetCurrentProcessId();
#else
	int pid = _getpid();
#endif

	return pid;
}


}
