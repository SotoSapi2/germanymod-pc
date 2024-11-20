#pragma once
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <filesystem>
#include <stacktrace>

namespace Logger
{
    std::wstring GetLoggerFilepath();

    void SetLogfilePath(std::wstring logPath, std::wstring filename);

    void ClearLogfile();

    void Print(const char* format, ...);
}

#define LOG_TEST()          Logger::Print("[TEST PASSED] %s:%i", __FUNCTION__, __LINE__)
#define LOG_NOTAG(str, ...) Logger::Print(str, __VA_ARGS__)
#define LOG_INFO(str, ...)  Logger::Print("[INFO] " str, __VA_ARGS__)
#define LOG_WARN(str, ...)  Logger::Print((std::string("[WARN] " str "\n") + std::to_string(std::stacktrace::current()) + "\n\n").c_str(), __VA_ARGS__)
#define LOG_ERROR(str, ...) Logger::Print((std::string("[PROPLAM] " str "\n") + std::to_string(std::stacktrace::current()) + "\n\n").c_str(), __VA_ARGS__)
#define LOG_NULLCHECK(ptr)  Logger::Print("%s is null? %s", #ptr, ptr == nullptr ? "true" : "false")