#include <iostream>
#include "common/format.hpp"
using namespace std;
using namespace zq;

int main(int argc, char* argv[])
{
	int a = 100;
	std::string s = "dwa";
	float b = 9.8f;
	std::string str = fmt::format("[{dwa}][{0}][{2}]: ", a, s, b);
	std::cout << str << std::endl;
	return 0;
}




