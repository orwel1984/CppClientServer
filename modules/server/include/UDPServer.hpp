#pragma once

#include <atomic>
#include <boost/asio.hpp>
#include <functional>

#include "GenericServer.hpp"

#define UDP_PACKET_SIZE 1024
using PacketHandler = std::function<void(const std::error_code &, std::size_t, std::string_view)>;


class UDPServer : public GenericServer
{
public:
    UDPServer(boost::asio::io_context &io_context, uint16_t port, PacketHandler handler = nullptr)
    : m_port(port), m_socket(io_context), m_running(false)
    {
        m_packetHandler = (handler)
                              ? std::move(handler)
                              : std::bind(&UDPServer::processPacket, this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3);
    }

    ~UDPServer() override { shutdown(); }
    std::error_code start() override;
    std::error_code stop() override;
    std::error_code shutdown() override;

    // Packet Handling
protected:
    void receivePacket();
    void onPacketReceived(const boost::system::error_code &ec, std::size_t size);
    void processPacket(const std::error_code &ec, std::size_t len, std::string_view buffer);
public: 
    void setPacketHandler(PacketHandler handler){ m_packetHandler = handler; }
    int getPacketSize() {return UDP_PACKET_SIZE;}

private:
    uint16_t m_port;
    boost::asio::ip::udp::socket m_socket;
    boost::asio::ip::udp::endpoint m_remote_endpoint;
    std::array<char, UDP_PACKET_SIZE> m_recv_buffer;
    std::atomic<bool> m_running;
    PacketHandler m_packetHandler;
};