#include "policy_udp_boost.hpp"
#include "policy_tcp_boost.hpp"

#include "policy_mode.hpp"
#include "server.h"
#include "server_error.h"
#include "logging.hpp"

#include <string>

// Constructor
template <typename Protocol, template <typename> class Mode>
Server<Protocol, Mode>::Server(Context &io, uint16_t port, PacketHandler handler)
    : m_socket(io), m_port(port), m_running(false), m_io(io)
{
    m_buffer.resize(UDP_PACKET_SIZE);
    m_packetHandler = (handler) ? std::move(handler)
                                : std::bind(&Server::onPacketReceived, this, std::placeholders::_1,
                                      std::placeholders::_2, std::placeholders::_3);
}

template <typename Protocol, template <typename> class Mode>
std::error_code
Server<Protocol, Mode>::start()
{
    if( auto open = Protocol::openSocket(m_socket, m_acceptor, Protocol::makeEndpoint(m_port), m_io); !open)
    {
        return open.error();
    }

    m_running = true;

    // TCP 
    if constexpr (std::is_same_v<Protocol, impl::protocol::TCP>)
    {
        logging::Log("Waiting for TCP client to connect...");
        typename Protocol::error_code ec;
        m_acceptor->accept(m_socket, ec);
        if (ec){ return make_error_code(ServerError::socket_open_failed);}
        logging::Log("TCP client connected.");        
    }

    if( auto recv = Mode<Protocol>::receivePacket(m_running, m_socket, m_remoteEndpoint, m_buffer, m_packetHandler); !recv)
    {        
        return recv.error();
    }
    return {};
}

template <typename Protocol, template <typename> class Mode>
std::error_code
Server<Protocol, Mode>::stop()
{
    if (!m_running.exchange(false))
        return {};

    typename Protocol::error_code ec;
    m_socket.close(ec);
    if (ec)
    {
        return make_error_code(ServerError::close_failed);
    }

    logging::Log("Server stopped successfully.");
    return {};
}

template <typename Protocol, template <typename> class Mode>
std::error_code
Server<Protocol, Mode>::shutdown()
{
    auto ec = stop();
    if (ec)
    {
        return ec;
    }

    return {}; // success
}

template <typename Protocol, template <typename> class Mode>
void
Server<Protocol, Mode>::onPacketReceived(
    const typename Protocol::error_code &error, std::size_t len, std::string_view packet)
{
    logging::Log("Received packet of size: " + std::to_string(len));
    logging::Log("Packet data: " + std::string(packet));
}

// Explicit template instantiations for the types we actually use
template class Server<impl::protocol::UDP, impl::mode::Async>;
template class Server<impl::protocol::UDP, impl::mode::Sync>;
template class Server<impl::protocol::TCP, impl::mode::Sync>;
