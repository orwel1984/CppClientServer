#include "UDPServer.h"

#include <boost/bind/bind.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace
{
    void Log(const std::string& message) { std::cout << message << std::endl; }

    void Log(const std::error_code& error)
    {
        std::cout << "Error code: " << error.value() << std::endl;
        std::cout << "Error message: " << error.message() << std::endl;
    }

    void Log(const std::string& message, const std::error_code& error)
    {
        Log(message);
        Log(error);
    }
}

std::error_code UDPServer::start()
{
    ip::udp::endpoint endpoint(ip::udp::v4(), m_port);

    boost::system::error_code ec;
    m_socket.open(endpoint.protocol(), ec);
    if (ec) { return ec; }
        
    m_socket.bind(endpoint, ec);
    if (ec) { return ec; }

    m_running = true;
    receivePacket();

    Log("UDPServer started successfully on port " + std::to_string(m_port));
    return {};  // success
}

void UDPServer::receivePacket()
{
    m_socket.async_receive_from(boost::asio::buffer(m_recv_buffer), m_remote_endpoint,
                                [this](boost::system::error_code ec, std::size_t bytes_transferred)
                                { onPacketReceived(ec, bytes_transferred); });
}

void UDPServer::onPacketReceived(const boost::system::error_code& error, std::size_t size)
{
    if(error)
    {
        Log(error);
        return;
    }

    // Handle Packet
    m_packetHandler(error, size, std::string_view(m_recv_buffer.data(), size));

    if (m_running)
    {
        receivePacket();  // recursive async chain
    }
}

void UDPServer::processPacket(const std::error_code& ec, std::size_t len, std::string_view packet)
{
    Log("Received packet of size: " + std::to_string(len));
    Log("Packet data: " + std::string(packet));
}

std::error_code UDPServer::stop()
{
    if (m_running)
    {
        m_running = false;
        boost::system::error_code error;
        auto result = m_socket.close(error);
        if(error)
        {
            Log(error);
            return error;
        }
    }
    Log("UDPServer stopped successfully.");
    return {};  // success
}

std::error_code UDPServer::shutdown()
{
    auto ec = stop();
    if (ec)
    {
        Log("Error closing UDP socket", ec);
        return ec;
    }

    Log("UDPServer shutdown complete.");
    return {};  // success
}
