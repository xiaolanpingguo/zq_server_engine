#include "test.h"
#include "common/format.hpp"
using namespace zq;

bool formatTest()
{
	int a = 100;
	std::string s = "dwa";
	float b = 9.8f;
	std::string str = fmt::format("[{dwa}}}][{0}][{2}]: ", a, s, b);
	std::cout << str << std::endl;

	return true;
}