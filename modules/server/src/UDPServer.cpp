#include "UDPServer.hpp"

#include <boost/bind/bind.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

void UDPServer::start()
{
    ip::udp::endpoint endpoint(ip::udp::v4(), m_port);

    m_socket.open(endpoint.protocol());
    m_socket.bind(endpoint);
    m_running = true;

    std::cout << "\nUDPServer started on port " << m_port << std::endl;
    receive_packet();
}

void UDPServer::receive_packet()
{
    m_socket.async_receive_from(boost::asio::buffer(m_recv_buffer), m_remote_endpoint,
                                [this](boost::system::error_code ec, std::size_t bytes_transferred)
                                { on_packet_received(ec, bytes_transferred); });
}

void UDPServer::on_packet_received(const boost::system::error_code& error, std::size_t bytes_recvd)
{
    if (!error)
    {
        m_packetHandler(error, bytes_recvd, std::string_view(m_recv_buffer.data(), bytes_recvd));

        if (m_running)
        {        
            receive_packet(); //recursive async chain
        }
    }
    else
    {
        std::cout << "Error code: " << error << std::endl;
        std::cout << "Error message: " << error.message() << std::endl;
    }
}

void UDPServer::process_packet(const std::error_code& ec, std::size_t len, std::string_view packet)
{
    std::cout << "\n\nReceived packet of size: " << len << std::endl;
    std::cout << "\nPacket data: \n" << packet << std::endl;
}

void UDPServer::stop()
{
    if (m_running)
    {
        m_running = false;
        boost::system::error_code ec;
        auto result = m_socket.close(ec);
        if (ec)
        {
            std::cerr << "Error closing UDP socket: " << ec.message() << std::endl;
        }
        std::cout << "\nUDPServer stopped." << std::endl;
    }
}

void UDPServer::shutdown()
{
    stop();

    // Add any additional cleanup here if needed

    std::cout << "\nUDPServer shutdown complete." << std::endl;
}
