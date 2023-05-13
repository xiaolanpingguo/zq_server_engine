#pragma once


#include <sstream>
#include <tuple>


namespace zq{

struct fmt
{
	template <typename... Args>
	static std::string format(std::string_view format, Args&&... args)
	{
		size_t tp_size = sizeof...(args);
		if (tp_size <= 0)
		{
			return std::string(format);
		}

		auto tp = std::forward_as_tuple(std::forward<Args>(args)...);
		size_t start = 0;
		size_t pos = 0;
		std::ostringstream ss;
		int i = 0;
		while (1)
		{
			pos = format.find('{', start);
			if (pos == std::string::npos)
			{
				ss << format.substr(start);
				break;
			}

			ss << format.substr(start, pos - start);
			if (format[pos + 1] == '{')
			{
				ss << '{';
				start = pos + 2;
				continue;
			}

			start = pos + 1;
			pos = format.find('}', start);
			if (pos == std::string::npos)
			{
				ss << format.substr(start - 1);
				break;
			}

			// match the {}
			int index = 0;
			std::string_view sub_str = format.substr(start, pos - start);
			if (sub_str.empty())
			{
				index = i;
				i++;
			}
			// match like {0}, sub_str: 0
			else
			{
				char* endptr = nullptr;
				index = strtol(sub_str.data(), &endptr, 10);
				if (index < 0 || index >= (int)tp_size)
				{
					index = 0;
				}
			}

			visit(index, tp, [&](auto& item)
			{
				ss << item;
			}, std::make_index_sequence<sizeof...(args)>{});

			start = pos + 1;
		}

		return ss.str();
	}
private:

	template <class Tuple, class F, std::size_t...Idx>
	static void visit(std::size_t i, Tuple& t, F&& f, std::index_sequence<Idx...>)
	{
		((i == Idx && ((std::forward<F>(f)(std::get<Idx>(t))), false)), ...);
	}
};
}
