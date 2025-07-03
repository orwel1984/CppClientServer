#pragma once
#include <boost/asio.hpp>

class GenericClient
{
public:
    GenericClient(boost::asio::io_context& io_context, const std::string& ip, uint16_t port) {}
    virtual ~GenericClient() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;     
    virtual void cleanup() = 0; 
};
