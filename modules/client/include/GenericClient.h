#pragma once
#include <boost/asio.hpp>

class GenericClient
{
public:
    virtual ~GenericClient() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;     
    virtual void cleanup() = 0; 
};
