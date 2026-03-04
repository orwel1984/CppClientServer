#include "Client.h"

#include "policy_tcp_boost.hpp"  // protocols provide client helpers
#include "policy_udp_boost.hpp"

#include <iostream>
#include <exception>
#include <boost/asio/write.hpp>

// resolve endpoint using the protocol-specific address version helper
template <typename Protocol>
void Client<Protocol>::resolveEndPoint()
{
    auto endpoints = m_resolver.resolve(Protocol::version(), m_ip, std::to_string(m_port));
    m_serverEndpoint = *endpoints.begin();
}

template <typename Protocol>
void Client<Protocol>::connect()
{
    try
    {
        Protocol::connectSocket(m_socket, m_serverEndpoint);
        logConnectionSuccess();
    }
    catch (const std::exception &ex)
    {
        logConnectionFail(ex);
    }
}

template <typename Protocol>
void Client<Protocol>::disconnect()
{
    Protocol::disconnectSocket(m_socket);
}

// send the packet through the policy send helper
template <typename Protocol>
void Client<Protocol>::sendPacket(const std::vector<std::byte> &buffer)
{
    Protocol::send(m_socket, buffer);
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
void Client<Protocol>::logConnectionFail(const std::exception &ex)
{
    std::cout << "Unable to connect to "
              << m_ip << ":" << m_port
              << "\nError: " << ex.what()
              << std::endl;
}

// explicit template instantiations for the protocols we currently use
template class Client<impl::protocol::UDP>;
template class Client<impl::protocol::TCP>;
