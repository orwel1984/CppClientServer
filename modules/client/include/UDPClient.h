#pragma once

#include <string>
#include "GenericClient.h"

class UDPClient : public GenericClient
{
public:
    UDPClient(boost::asio::io_context& io_context, const std::string& ip, uint16_t port)
        : m_ip(ip),
          m_port(port),
          m_socket(io_context),
          m_serverEndpoint(boost::asio::ip::udp::v4(), port),
          m_resolver(io_context)
    {
        resolveEndPoint();
    }

    ~UDPClient() override { cleanup(); }

    void connect() override;
    void disconnect() override { m_socket.close(); }
    void cleanup() override {  disconnect(); }
    void sendPacket(const std::vector<std::byte>& buffer);

protected:
    void  resolveEndPoint();
    void  logConnetionSuccesss();    
    void  logConnetionFail(const std::exception &ex);    

private:
    using boost_udp = boost::asio::ip::udp;
    
    std::string m_ip;
    uint16_t m_port;
    boost_udp::socket m_socket;
    boost_udp::resolver m_resolver;
    boost_udp::endpoint m_serverEndpoint;
};
