#pragma once


#include "common/common.hpp"

namespace zq{


namespace strutils{


template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
inline T stringConvert(const std::string_view& s, std::errc& ec)
{
	T result;
	auto res = std::from_chars(s.data(), s.data() + s.size(), result);
	ec = res.ec;
	return result;
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
inline T stringConvert(const std::string_view& s)
{
	T result;
	if (auto [p, ec] = std::from_chars(s.data(), s.data() + s.size(), result); ec != std::errc())
	{
		throw std::runtime_error(std::to_string(static_cast<int>(ec)));
	}
	return result;
}

template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
inline T stringConvert(const std::string_view& s, std::errc& ec, int base = 10)
{
	T result{};
	auto res = std::from_chars(s.data(), s.data() + s.size(), result, base);
	ec = res.ec;
	return result;
}

template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
inline T stringConvert(const std::string_view& s, int base = 10)
{
	T result;
	if (auto [p, ec] = std::from_chars(s.data(), s.data() + s.size(), result, base); ec != std::errc())
	{
		throw std::runtime_error(std::to_string(static_cast<int>(ec)));
	}
	return result;
}

template <typename T, std::enable_if_t<std::is_same_v<T, std::string>, int> = 0>
inline std::string stringConvert(const std::string_view& s)
{
	return std::string(s);
}

template <typename T, std::enable_if_t<std::is_same_v<T, std::string_view>, int> = 0>
inline std::string_view stringConvert(const std::string_view& s)
{
	return s;
}

/*
e. split("aa/bb/cc","/")
*/
template <class T>
std::vector<T> split(const std::string_view& src, const std::string_view& sep)
{
	std::vector<T> r;
	std::string_view::const_iterator b = src.begin();
	std::string_view::const_iterator e = src.end();
	for (auto i = src.begin(); i != src.end(); ++i)
	{
		if (sep.find(*i) != std::string_view::npos)
		{
			if (b != e && b != i)
			{
				r.push_back(stringConvert<T>(std::string_view(std::addressof(*b), size_t(i - b))));
			}
			b = e;
		}
		else
		{
			if (b == e)
			{
				b = i;
			}
		}
	}

	if (b != e)
	{
		r.push_back(stringConvert<T>(std::string_view(std::addressof(*b), size_t(e - b))));
	}

	return r;
}

//return left n char
inline std::string left(const std::string& str, size_t n)
{
	return std::string(str, 0, n);
}

//return right n char
inline std::string right(const std::string& str, size_t n)
{
	size_t s = (str.size() >= n) ? (str.size() - n) : 0;
	return std::string(str, s);
}

//" /t/n/r"
inline std::string_view trimRight(std::string_view v)
{
	const auto words_end(v.find_last_not_of(" \t\n\r"));
	if (words_end != std::string_view::npos)
	{
		v.remove_suffix(v.size() - words_end - 1);
	}
	return v;
}

inline std::string_view trimLeft(std::string_view v)
{
	const auto words_begin(v.find_first_not_of(" \t\n\r"));
	v.remove_prefix(std::min(words_begin, v.size()));
	return v;
}

inline std::string_view trim(std::string_view v)
{
	const auto words_end(v.find_last_not_of(" \t\n\r"));
	if (words_end != std::string_view::npos)
	{
		v.remove_suffix(v.size() - words_end - 1);
	}
	const auto words_begin(v.find_first_not_of(" \t\n\r"));
	v.remove_prefix(std::min(words_begin, v.size()));
	return v;
}

inline void replace(std::string& src, std::string_view old, std::string_view strnew)
{
	for (std::string::size_type pos(0); pos != std::string::npos; pos += strnew.size())
	{
		if ((pos = src.find(old, pos)) != std::string::npos)
			src.replace(pos, old.size(), strnew);
		else
			break;
	}
}

//https://en.cppreference.com/w/cpp/string/byte/tolower
//the behavior of std::tolower is undefined if the argument's value is neither representable
//as unsigned char nor equal to EOF.
//To use these functions safely with plain chars (or signed chars),
//	the argument should first be converted to unsigned char
inline char toupper(unsigned char c)
{
	return static_cast<char>(std::toupper(c));
}

inline char tolower(unsigned char c)
{
	return static_cast<char>(std::tolower(c));
}

inline void upper(std::string& src)
{
	std::transform(src.begin(), src.end(), src.begin(), toupper);
}

inline void lower(std::string& src)
{
	std::transform(src.begin(), src.end(), src.begin(), tolower);
}

template <typename TString>
inline bool equalStringNoCase(const TString& str1, const TString& str2)
{
	if (str1.size() != str2.size())
		return false;

	auto iter1begin = str1.begin();
	auto iter2begin = str2.begin();

	auto iter1end = str1.end();
	auto iter2end = str2.end();

	for (; iter1begin != iter1end && iter2begin != iter2end; ++iter1begin, ++iter2begin)
	{
		if (!(toupper(*iter1begin) == toupper(*iter2begin)))
		{
			return false;
		}
	}
	return true;
}

inline std::string hexString(std::string_view text)
{
	static constexpr std::string_view hex = "0123456789abcdef";
	std::string res(text.size() * 2, 0);
	size_t i = 0;
	for (uint8_t c : text)
	{
		res[i * 2] = hex[c >> 4];
		res[i * 2 + 1] = hex[c & 0xf];
		++i;
	}
	return res;
}
} //namespace strutils
}
