#include <iostream>
#include <functional>
#include "test.h"
#include "common/format.hpp"
using namespace std;
using namespace zq;


int main(int argc, char* argv[])
{
	Log::getInstance().init("test");

	std::vector<std::function<bool()>> vec
	{
		//std::bind(testFormat),
		//std::bind(testRedis),
		std::bind(testMongo),
		//std::bind(testLog),
		//std::bind(testLogin),
	};

	for (const auto& fun : vec)
	{
		if (!fun())
		{
			std::cout << "test function:" << fun.target_type().name() << "failed !" << std::endl; 
		}
	}

	Log::getInstance().finalize();
	return 0;
}




