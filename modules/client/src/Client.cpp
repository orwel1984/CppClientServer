#include "Client.h"

template <typename Protocol>
void Client<Protocol>::resolveEndPoint()
{
    auto endpoints = m_resolver.resolve( Protocol::version,  m_ip, std::to_string(m_port));
    m_serverEndpoint = *endpoints.begin();
}

template <typename Protocol>
void Client<Protocol>::connect()
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

template <typename Protocol>
void Client<Protocol>::disconnect()
{
    if (m_socket.is_open())
    {
        boost::system::error_code ec;
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        m_socket.close(ec);
    }
}

template <typename Protocol>
void Client<Protocol>::sendPacket(const std::vector<std::byte>& buffer)
{
    boost::asio::write(m_socket, boost::asio::buffer(buffer));
}

template <typename Protocol>
void Client<Protocol>::logConnectionSuccess()
{
    std::cout << "Connected to "
              << m_socket.remote_endpoint().address()
              << ":"
              << m_socket.remote_endpoint().port()
              << std::endl;
}

template <typename Protocol>
void Client<Protocol>::logConnectionFail(const std::exception& ex)
{
    std::cout << "Unable to connect to "
              << m_ip << ":" << m_port
              << "\nError: " << ex.what()
              << std::endl;
}
