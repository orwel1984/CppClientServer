
#include "policy_udp_boost.hpp"
#include "policy_mode.hpp"
#include "server.h"

#include <string>

// Constructor
template <typename Protocol, template <typename> class Mode>
Server<Protocol, Mode>::Server(Context &io, uint16_t port, PacketHandler handler)
    : m_socket(io), m_port(port), m_running(false)
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
    // 1.   Init socket
    auto r = Protocol::openSocket(m_socket, Protocol::makeEndpoint(m_port));
    if (!r)
    {
        logging::Log("Failed to open and bind socket", r.error());
        return std::error_code(r.error().value(), r.error().category());
    }

    // 2.   Listen for packets
    m_running = true;
    Mode<Protocol>::receivePacket(m_running, m_socket, m_remoteEndpoint, m_buffer, m_packetHandler);

    return {}; // success
}

template <typename Protocol, template <typename> class Mode>
std::error_code
Server<Protocol, Mode>::stop()
{
    // Atomically set m_running to false and check if it was previously true
    if (!m_running.exchange(false))
    {
        return {};
    }
    
    typename Protocol::error_code error;
    m_socket.close(error);
    if (error)
    {
        logging::Log(error);
        return error;
    }

    logging::Log("Server stopped successfully.");
    return {}; // success
}

template <typename Protocol, template <typename> class Mode>
std::error_code
Server<Protocol, Mode>::shutdown()
{
    auto ec = stop();
    if (ec)
    {
        logging::Log("Error during shutdown", ec);
        return ec;
    }

    logging::Log("Server shutdown complete.");
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
