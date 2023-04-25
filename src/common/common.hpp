#pragma once


#include "common/platform.hpp"
#include "common/typedefs.hpp"
#include "common/log.hpp"

#include <iostream>
#include <iosfwd>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <type_traits>
#include <numeric>
#include <array>
#include <assert.h>
#include <ctime>
#include <chrono>
#include <random>
#include <algorithm>
#include <future>
#include <utility>
#include <memory>
#include <filesystem>
#include <coroutine>
#include <ctype.h>
#include <charconv>
#include <stdarg.h>


namespace zq{
	


// Windows badly defines a lot of stuff we'll never use. Undefine it.
#ifdef _WIN32
#undef min // override standard definition
#undef max // override standard definition
#undef ERROR // override (really stupid) wingdi.h standard definition
#undef DELETE // override (another really stupid) winnt.h standard definition
#undef MessageBox // override winuser.h standard definition
#undef MIN // override standard definition
#undef MAX // override standard definition
#undef CLAMP // override standard definition
#undef CONNECT_DEFERRED // override from Windows SDK, clashes with Object enum
#endif

// Generic ABS function, for math uses please use Math::abs.
#ifndef ABS
#define ABS(mV) (((mV) < 0) ? (-(mV)) : (mV))
#endif

#ifndef SGN
#define SGN(mV) (((mV) == 0) ? (0.0) : (((mV) < 0) ? (-1.0) : (+1.0)))
#endif

#ifndef MIN
#define MIN(mA, mB) (((mA) < (mB)) ? (mA) : (mB))
#endif

#ifndef MAX
#define MAX(mA, mB) (((mA) > (mB)) ? (mA) : (mB))
#endif

#ifndef CLAMP
#define CLAMP(mA, mMin, mMax) (((mA) < (mMin)) ? (mMin) : (((mA) > (mMax)) ? mMax : mA))
#endif


#ifdef COMPILER_MSC
#define GENERATE_TRAP() __debugbreak()
#else
#define GENERATE_TRAP() __builtin_trap()
#endif
	


template <typename... Args>
inline void Assert(bool condition, std::string_view file, int line, std::string_view fun, std::string_view fmt, Args&&... args)
{
	if (condition)
	{
		return;
	}

	std::string str = "";
	if (file.data() && fun.data())
	{
		str += std::vformat("file:{}, line:{}, function:{}, msg:", std::make_format_args(file, line, fun));
	}
	if (fmt.data())
	{
		str += std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
	}

	fprintf(stderr, "\nASSERTION FAILED:%s\n", str.c_str());
	GENERATE_TRAP();
}

#ifdef SERVER_RELEASE
#define ASSERT(cnd, msg)
#else
#define ASSERT(condition, fmt, ...) Assert(condition, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#endif
}