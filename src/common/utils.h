#pragma once


#include "common/common.hpp"


namespace zq {


class Utils
{
public:
	static int createPIDFile(const std::string& filename);
	static int getPid();
};

}



