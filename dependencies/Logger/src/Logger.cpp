#include "Logger.hpp"

namespace Logger
{
    std::wstring logPath = L"";
    std::ofstream logfileStream;

    std::wstring GetLoggerFilepath()
    {
        return logPath;
    }

    void SetLogfilePath(std::wstring logPath, std::wstring filename)
    {
        Logger::logPath = logPath + filename + L"\\.txt";
        Logger::logfileStream = std::ofstream(GetLoggerFilepath(), std::ios::app);
    }

    void ClearLogfile()
    {
        std::filesystem::remove(GetLoggerFilepath());
    }

    void Print(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        int size = std::vsnprintf(nullptr, 0, format, args) + 1;
        va_end(args);

        std::string message;
        message.resize(size);

        va_start(args, format);
        std::vsnprintf(&message[0], size, format, args);
        va_end(args);

        std::cout << message << std::endl;

        if (logfileStream.is_open())
        {
            logfileStream << message << std::endl;
            logfileStream.flush();
        }
    }
}