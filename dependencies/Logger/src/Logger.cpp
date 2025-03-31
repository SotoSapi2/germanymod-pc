#include "Logger.hpp"
#include <windows.h>

namespace Logger
{
    std::wstring logPath = L"";
    std::ofstream logfileStream;
    DebugOutputType debugOutType = DebugOutputType::None;

    std::wstring GetLoggerFilepath()
    {
        return logPath;
    }

    void SetLogfilePath(DebugOutputType debugOutType, const std::wstring& logPath, std::ios::openmode openmode)
    {
        Logger::debugOutType = debugOutType;
        Logger::logPath = logPath;
        Logger::logfileStream = std::ofstream(logPath, openmode);
    }

    void Log(LogTag tag, const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        int size = std::vsnprintf(nullptr, 0, format, args) + 1;
        va_end(args);

        std::string message;
        switch (tag)
        {
            case Logger::LogTag::Info:
                message = "[INFO] ";
                break;
            case Logger::LogTag::Warning:
                message = "[WARN] ";
                break;
            case Logger::LogTag::Error:
                message = "[ERROR] ";
                break;
            default:
                break;
        }

        int intitialMessageSize = message.size();
        message.resize(intitialMessageSize + size);

        va_start(args, format);
        std::vsnprintf(&message[intitialMessageSize], message.size(), format, args);
        va_end(args);

        #ifdef LOGGER_STACKTRACE_IS_SUPPORTED
        #ifdef LOGGER_TRACE_WARNING
        if (tag == LogTag::Warning)
        {
            message.append("\n");
            message.append(std::to_string(std::stacktrace()));
        }
        #endif // LOGGER_TRACE_WARNING

        #ifdef LOGGER_TRACE_ERROR
        if (tag == LogTag::Error)
        {
            message.append("\n");
            message.append(std::to_string(std::stacktrace()));
        }
        #endif // LOGGER_TRACE_ERROR
        #endif // LOGGER_TRACE_WARNING
        message.append("\0");

        switch (debugOutType)
        {
            case Logger::DebugOutputType::Stdout:
                std::cout << message << std::endl;
                break;
            case Logger::DebugOutputType::Debugger:
                OutputDebugStringA(message.c_str());
                break;
            case Logger::DebugOutputType::None:
            default:
                break;
        }

        if (logfileStream.is_open())
        {
            logfileStream << message << std::endl;
            logfileStream.flush();
        }
    }
}