#pragma once


#include <fstream>
#include <vector>
#include <string>


namespace zq
{
    class File
    {
    public:
        static std::string readAll(const std::string& path, std::ios::openmode Mode = std::ios::binary | std::ios::in)
        {
            std::fstream is(path, Mode);
            if (is.is_open())
            {
                // get length of file:
                is.seekg(0, is.end);
                size_t length = static_cast<size_t>(is.tellg());
                is.seekg(0, is.beg);

                std::string tmp;
                tmp.resize(length);
                is.read(&tmp.front(), length);
                is.close();
                return tmp;
            }
            return std::string();
        }

        static bool write(const std::string& path, const std::string& content, std::ios::openmode Mode = std::ios::out)
        {
            std::fstream os(path, Mode);
            if (os.is_open())
            {
                os.write(content.data(), content.size());
                os.close();
                return true;
            }

            return false;
        }

        static size_t getFileSize(const std::string& path)
        {
            std::ifstream is(path, std::ifstream::binary);
            if (is) 
            {
                // get length of file:
                is.seekg(0, is.end);
                auto length = static_cast<size_t>(is.tellg());
                is.close();
                return length;
            }

            return 0;
        }
    };
}

