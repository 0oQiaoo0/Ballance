#pragma once
#include "Logger.h"
#include "GlobalContext.h"

#define LOG_HELPER(LOG_LEVEL, ...) \
globalContext.mLogger->log(LOG_LEVEL, "[" + std::string(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_DEBUG(...) LOG_HELPER(Logger::LogLevel::debug, __VA_ARGS__);

#define LOG_INFO(...) LOG_HELPER(Logger::LogLevel::info, __VA_ARGS__);

#define LOG_WARN(...) LOG_HELPER(Logger::LogLevel::warn, __VA_ARGS__);

#define LOG_ERROR(...) LOG_HELPER(Logger::LogLevel::error, __VA_ARGS__);

#define LOG_FATAL(...) LOG_HELPER(Logger::LogLevel::fatal, __VA_ARGS__);


