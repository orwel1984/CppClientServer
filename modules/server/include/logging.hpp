#pragma once

#include <iostream>
#include <string>
#include <system_error>

namespace logging {

inline void Log(const std::string &message)
{
    std::cout << message << std::endl;
}

inline void Log(const std::error_code &error)
{
    std::cout << "Error code: " << error.value() << std::endl;
    std::cout << "Error message: " << error.message() << std::endl;
}

inline void Log(const std::string &message, const std::error_code &error)
{
    Log(message);
    Log(error);
}

} // namespace logging
