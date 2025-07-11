#include "Server.h"
#include "BoostUDPPolicy.hpp"
#include "ModePolicy.hpp"

#include <string>

// Template method implementations
template <typename BackendPolicy, template <typename> class ModePolicy>
std::error_code Server<BackendPolicy, ModePolicy>::start()
{
    // Create local endpoint for binding
    auto localEndpoint = BackendPolicy::makeEndpoint(m_port);
    
    // init socket
    auto ec = BackendPolicy::openAndBindSocket(m_socket, localEndpoint);
    if (ec)
    {
        Log("Failed to open and bind socket", ec);
        return ec;
    }

    // listen for packets
    m_running = true;

    // receiveNextPacket();    
    ModePolicy<BackendPolicy>::run(m_running, m_socket, m_remoteEndpoint, m_buffer, m_packetHandler);

    return { }; // success
}

template <typename BackendPolicy, template <typename> class ModePolicy>
void Server<BackendPolicy, ModePolicy>::onPacketReceived(const typename BackendPolicy::error_code& error, std::size_t len, std::string_view packet)
{
    Log("Received packet of size: " + std::to_string(len));
    Log("Packet data: " + std::string(packet));
}

template <typename BackendPolicy, template <typename> class ModePolicy>
std::error_code Server<BackendPolicy, ModePolicy>::stop()
{
    if (m_running)
    {
        m_running = false;
        typename BackendPolicy::error_code error;
        auto result = m_socket.close(error);
        if(error)
        {
            Log(error);
            return error;
        }
    }
    Log("Server stopped successfully.");
    return {};  // success
}

template <typename BackendPolicy, template <typename> class ModePolicy>
std::error_code Server<BackendPolicy, ModePolicy>::shutdown()
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
