#pragma once
#include <boost/asio.hpp>

class GenericServer 
{
public:
    GenericServer(boost::asio::io_context& io_context){};
    virtual ~GenericServer() = default;

    virtual void start() =0;
    virtual void stop() =0;
    virtual void shutdown() =0;
};
