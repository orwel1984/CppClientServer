#pragma once

#include <string>
#include <boost/asio.hpp>
#include "GenericClient.h"

class TCPClient : public GenericClient
{
public:
    TCPClient(boost::asio::io_context& io_context,
              const std::string& ip,
              uint16_t port)
        : m_ip(ip),
          m_port(port),
          m_socket(io_context),
          m_resolver(io_context)
    {
        resolveEndPoint();
    }

    ~TCPClient() override { cleanup(); }

    void connect() override;
    void disconnect() override;
    void cleanup() override { disconnect(); }

    void sendPacket(const std::vector<std::byte>& buffer);

protected:
    void resolveEndPoint();
    void logConnectionSuccess();
    void logConnectionFail(const std::exception& ex);

private:
    using boost_tcp = boost::asio::ip::tcp;

    std::string m_ip;
    uint16_t m_port;

    boost_tcp::socket m_socket;
    boost_tcp::resolver m_resolver;
    boost_tcp::endpoint m_serverEndpoint;
};
