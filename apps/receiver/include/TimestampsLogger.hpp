#pragma once

#include <chrono>
#include <fstream>
#include <string>

class TimestampLogger
{
public:
    TimestampLogger(const std::string& filePath) : m_logFile(filePath, std::ios::out | std::ios::app)
    {
    }

    ~TimestampLogger()
    {
        if (m_logFile.is_open())
            m_logFile.close();
    }

    void timestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        m_logFile << ms << std::endl;
    }

private:
    std::ofstream m_logFile;
};