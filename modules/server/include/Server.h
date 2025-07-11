#pragma once

#include <atomic>
#include <functional>

#include "GenericServer.h"

#define UDP_PACKET_SIZE 1024
using PacketHandler = std::function<void(const std::error_code&, std::size_t, std::string_view)>;

/**
    A Server class that implements the GenericServer interface based on implmentation policy and a
   mode policy. The Backend Policy will decide whether this is a TCP or UDP based server and provide
   a socket of that particular type. The ModePolicy will determine if the server should run in an
   asynchronous or synhcronous way.
 */
template <typename BackendPolicy, template <typename> class ModePolicy>
class Server : public GenericServer
{
public:
    using Socket = typename BackendPolicy::Socket;
    using Endpoint = typename BackendPolicy::Endpoint;
    using Context = typename BackendPolicy::Context;
    using Error_Code = typename BackendPolicy::error_code;

    //constructor
    Server(Context& io, uint16_t port, PacketHandler handler = nullptr)
        : m_socket(io), m_port(port), m_running(false)
    {
        m_buffer.resize(UDP_PACKET_SIZE);
        m_packetHandler = (handler)
                              ? std::move(handler)
                              : std::bind(&Server::onPacketReceived, this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3);
    }

    // generic-server
    std::error_code start() override;
    std::error_code stop() override;
    std::error_code shutdown() override;
    
    // packet-handling
    using PacketHandler = std::function<void(const std::error_code &, std::size_t, std::string_view)>;    
    void setPacketHandler(PacketHandler handler) { m_packetHandler = std::move(handler); }

protected:
    void receiveNextPacket();
    void onPacketReceived(const Error_Code& error, std::size_t len, std::string_view packet);

private:
    Socket m_socket;
    Endpoint m_remoteEndpoint;    
    uint16_t m_port;
    PacketHandler m_packetHandler;
    std::vector<char> m_buffer;
    std::atomic<bool> m_running;
};
