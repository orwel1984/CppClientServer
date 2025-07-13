#include "Server.h"

#include <string>

#include "BoostUDPPolicy.hpp"
#include "ModePolicy.hpp"

// Constructor
template <typename Protocol, template <typename> class Mode>
Server<Protocol, Mode>::Server(Context& io, uint16_t port, PacketHandler handler)
    : m_socket(io), m_port(port), m_running(false)
{
    m_buffer.resize(UDP_PACKET_SIZE);
    m_packetHandler = (handler) ? std::move(handler)
                                : std::bind(&Server::onPacketReceived, this, std::placeholders::_1,
                                            std::placeholders::_2, std::placeholders::_3);
}

template <typename Protocol, template <typename> class Mode>
std::error_code Server<Protocol, Mode>::start()
{
    // 1.   Init socket
    auto ec = Protocol::openAndBindSocket(m_socket, Protocol::makeEndpoint(m_port));
    if (ec)
    {
        Log("Failed to open and bind socket", ec);
        return ec;
    }

    // 2.   Listen for packets
    m_running = true;
    Mode<Protocol>::receivePacket(m_running, m_socket, m_remoteEndpoint, m_buffer, m_packetHandler);

    return {};  // success
}

template <typename Protocol, template <typename> class Mode>
void Server<Protocol, Mode>::onPacketReceived(const typename Protocol::error_code& error,
                                              std::size_t len, std::string_view packet)
{
    Log("Received packet of size: " + std::to_string(len));
    Log("Packet data: " + std::string(packet));
}

template <typename Protocol, template <typename> class Mode>
std::error_code Server<Protocol, Mode>::stop()
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
        Log(error);
        return error;
    }

    Log("Server stopped successfully.");
    return {};  // success
}

template <typename Protocol, template <typename> class Mode>
std::error_code Server<Protocol, Mode>::shutdown()
{
    auto ec = stop();
    if (ec)
    {
        Log("Error during shutdown", ec);
        return ec;
    }

    Log("Server shutdown complete.");
    return {};  // success
}

// Explicit template instantiations for the types we actually use
template class Server<BoostUDPPolicy, AsyncMode>;
template class Server<BoostUDPPolicy, SyncMode>;
