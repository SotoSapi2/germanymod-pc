#pragma once
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <filesystem>

#if __cplusplus >= 202101L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202004L)
    #define LOGGER_STACKTRACE_IS_SUPPORTED
#endif

#ifdef LOGGER_STACKTRACE_IS_SUPPORTED
    #include <stacktrace>
#endif

namespace Logger
{
    enum class LogTag
    {
        Info,
        Warning,
        Error,
        None
    };

    enum class DebugOutputType
    {
        Stdout,
        Debugger,
        None
    };

    std::wstring GetLoggerFilepath();

    void SetLogfilePath(DebugOutputType debugOutType, const std::wstring& logPath, 
                        std::ios::openmode openmode = std::ios_base::out | std::ios_base::trunc);

    void Log(LogTag tag, const char* format, ...);
}

#ifdef LOGGER_DEFINE_MACRO
#define LOG_TEST()          Logger::Log(Logger::LogTag::None, "[TEST PASSED] %s:%i", __FUNCTION__, __LINE__)
#define LOG_NOTAG(str, ...) Logger::Log(Logger::LogTag::None, str, __VA_ARGS__)
#define LOG_INFO(str, ...)  Logger::Log(Logger::LogTag::Info, str, __VA_ARGS__)
#define LOG_WARN(str, ...)  Logger::Log(Logger::LogTag::Warn, str, __VA_ARGS__)
#define LOG_ERROR(str, ...) Logger::Log(Logger::LogTag::Error, str, __VA_ARGS__)

#define LOG_INFO_IF(ifTrue, str, ...)   if(ifTrue) LOG_INFO(str, __VA_ARGS__)
#define LOG_ERROR_IF(ifTrue, str, ...)  if(ifTrue) LOG_ERROR(str, __VA_ARGS__)
#endif