#include "Server.h"
#include "BoostUDPPolicy.hpp"
#include "ModePolicy.hpp"

#include <string>

// Template method implementations
template <typename Protocol, template <typename> class Mode>
std::error_code Server<Protocol, Mode>::start()
{   
    // Init socket
    auto ec = Protocol::openAndBindSocket(m_socket, Protocol::makeEndpoint(m_port));
    if (ec)
    {
        Log("Failed to open and bind socket", ec);
        return ec;
    }

    // Listen for packets
    m_running = true;
    Mode<Protocol>::receivePacket(m_running, m_socket, m_remoteEndpoint, m_buffer, m_packetHandler);

    return { }; // success
}

template <typename Protocol, template <typename> class Mode>
void Server<Protocol, Mode>::onPacketReceived(const typename Protocol::error_code& error, std::size_t len, std::string_view packet)
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
    if(error)
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
