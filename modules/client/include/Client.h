#pragma once

#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>

#include "IClient.h"

template <typename Protocol>
class Client : public IClient
{
    using Context = typename Protocol::Context;
    using Resolver = typename Protocol::Resolver;
    using Socket = typename Protocol::Socket;
    using Endpoint = typename Protocol::Endpoint;
    using Error_Code = typename Protocol::error_code;

public:

    Client(Context& io_context, const std::string& ip, uint16_t port)
        : m_ip(ip),
          m_port(port),
          m_socket(io_context),
          m_resolver(io_context)
    {
        resolveEndPoint();
    }

    ~Client() override { cleanup(); }

    void connect() override;
    void disconnect() override;
    void cleanup() override { disconnect(); }

    void sendPacket(const std::vector<std::byte>& buffer);

protected:
    void resolveEndPoint();

    void logConnectionSuccess();
    void logConnectionFail(const std::exception& ex);

private:
    Socket m_socket;
    Resolver m_resolver;
    Endpoint m_serverEndpoint;

    std::string m_ip;  // ip address
    uint16_t m_port;  // port
};
