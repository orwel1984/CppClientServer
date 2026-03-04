#pragma once

class IClient
{
public:
    virtual ~IClient() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;     
    virtual void cleanup() = 0; 
};
