#pragma once

#include "common.hpp"
#include "common/string.hpp"
#include <filesystem>


namespace fs = std::filesystem;

namespace zq
{
    class Directory
    {
    public:
        inline static fs::path working_directory;

        static std::string currentDirectory()
        {
            std::error_code ec;
            auto p = fs::current_path(ec);
            return p.string();
        }

        static bool exists(const std::string &path)
        {
            std::error_code ec;
            auto b = fs::exists(path, ec);
            return (!ec) && b;
        }

        //THandler bool(const fs::path& path,bool dir)
        template<typename Handler>
        static void scanDir(const std::string_view& dir, int max_depth, const Handler& handler)
        {
			struct DoScan
            {
                int depth = 0;
                void operator()(const fs::path& path, const Handler& handler)
                {
                    if (depth < 0)
                    {
                        return;
                    }

                    std::error_code ec;
                    if (!fs::exists(path, ec))
                    {
                        return;
                    }

                    depth--;

                    for (auto& p : fs::directory_iterator(path))
                    {
                        if (!handler(p.path(), fs::is_directory(p, ec)))
                        {
                            break;
                        }
                        if (fs::is_directory(p, ec))
                        {
                             operator()(p.path(), handler);
                        }
                    }
                }
            };

            DoScan d{ max_depth };
            d(dir, handler);
        }

        static bool createDirectory(const std::string& dir)
        {
            std::error_code ec;
            fs::create_directories(dir, ec);
            return !ec;
        }

        static bool remove(const std::string& dir)
        {
            std::error_code ec;
            fs::remove(dir, ec);
            return !ec;
        }

        static bool removeAll(const std::string& dir)
        {
            std::error_code ec;
            fs::remove_all(dir, ec);
            return !ec;
        }

        static std::string find(const std::string& path, const std::string& filename, int depth = 10)
        {
            std::string result;
            std::vector<std::string> searchdir = split<std::string>(path, ";");
            for (const auto& v : searchdir)
            {
                scanDir(v, depth, [&result, &filename](const fs::path& p, bool)
                    {
                        if (p.filename().string() == filename)
                        {
                            result = fs::absolute(p).string();
                            return false;
                        }
                        return true;
                    });

                if (!result.empty())
                {
                    return result;
                }
            }
            return result;
        }
    };
}