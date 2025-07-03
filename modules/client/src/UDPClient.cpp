#include "UDPClient.h"

#include <nl_types.h>
#include <exception>
#include <iostream>


void UDPClient::resolveEndPoint()
{
    auto endpoints = m_resolver.resolve(boost::asio::ip::udp::v4(), m_ip, std::to_string(m_port));
    m_serverEndpoint = *endpoints;
}

void UDPClient::connect()
{
    try
    {
        m_socket.open(boost::asio::ip::udp::v4());
        m_socket.connect(m_serverEndpoint);
        logConnetionSuccesss();
    }
    catch (std::exception &ex)
    {
        logConnetionFail(ex);
    }
}

void UDPClient::sendPacket(const std::vector<std::byte>& buffer)
{
    m_socket.send(boost::asio::buffer(buffer));
}

void  UDPClient::logConnetionSuccesss()
{
        std::cout << "Connected to " << m_serverEndpoint.address() << ":"
                  << m_serverEndpoint.port() << std::endl;
}

void  UDPClient::logConnetionFail(const std::exception &ex)
{
        std::cout << "Unable to connect to:" << m_serverEndpoint.address() << ":"
                  << m_serverEndpoint.port() << "\nError: " << ex.what() << std::endl;
}
