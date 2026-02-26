#include "TCPClient.h"

#include <iostream>
#include <exception>

void TCPClient::resolveEndPoint()
{
    auto endpoints = m_resolver.resolve( boost::asio::ip::tcp::v4(),  m_ip, std::to_string(m_port));
    m_serverEndpoint = *endpoints.begin();
}

void TCPClient::connect()
{
    try
    {
        m_socket.connect(m_serverEndpoint);
        logConnectionSuccess();
    }
    catch (const std::exception& ex)
    {
        logConnectionFail(ex);
    }
}

void TCPClient::disconnect()
{
    if (m_socket.is_open())
    {
        boost::system::error_code ec;
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        m_socket.close(ec);
    }
}

void TCPClient::sendPacket(const std::vector<std::byte>& buffer)
{
    boost::asio::write(m_socket, boost::asio::buffer(buffer));
}

void TCPClient::logConnectionSuccess()
{
    std::cout << "Connected to "
              << m_socket.remote_endpoint().address()
              << ":"
              << m_socket.remote_endpoint().port()
              << std::endl;
}

void TCPClient::logConnectionFail(const std::exception& ex)
{
    std::cout << "Unable to connect to "
              << m_ip << ":" << m_port
              << "\nError: " << ex.what()
              << std::endl;
}
