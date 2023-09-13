#pragma once

#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>

void printRedText(const std::string& text);
class Logger
{
public:
    enum class LogLevel : uint8_t
    {
        debug,
        info,
        warn,
        error,
        fatal
    };
    void log(const char* str)
    {
        log(LogLevel::debug, std::string(str));
    }
    void log(std::string& str)
    {
        log(LogLevel::debug, str);
    }
    std::vector<std::string> Info;
    template<typename T, typename... TARGS>
    void log(LogLevel level, T&& v, TARGS&&... args)
    {
        Info.push_back(v);
        log(level, args...);
    }
    void log(LogLevel level)
    {
        if (Info.size() == 0)
        {
            return;
        }
        for (auto& str : Info)
        {
            printRedText(str);
        }
        std::cout << std::endl;
        Info.clear();
    }
};

