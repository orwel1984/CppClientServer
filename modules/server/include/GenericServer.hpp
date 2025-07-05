#pragma once
#include <boost/asio.hpp>
#include <system_error>

class ServerImpl;
class GenericServer
{
public:
    virtual std::error_code start() = 0;
    virtual std::error_code stop() = 0;
    virtual std::error_code shutdown() = 0;
    virtual ~GenericServer() = default;
};
