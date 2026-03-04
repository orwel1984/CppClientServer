#pragma once

// client side protocol helpers for UDP

#include <vector>
#include <string>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/system/error_code.hpp>

namespace impl
{
    namespace protocol
    {
        struct UDP
        {
            using Context   = boost::asio::io_context;
            using Protocol  = boost::asio::ip::udp;
            using Endpoint  = Protocol::endpoint;
            using Socket    = Protocol::socket;
            using Resolver  = Protocol::resolver;
            using error_code = boost::system::error_code;

            static Protocol version() { return Protocol::v4(); }

            static void connectSocket(Socket &socket, const Endpoint &endpoint)
            {
                // UDP "connect" simply sets the default remote endpoint
                socket.open(Protocol::v4());
                socket.connect(endpoint);
            }

            static void disconnectSocket(Socket &socket)
            {
                error_code ec;
                socket.close(ec);
            }

            static void send(Socket &socket, const std::vector<std::byte> &buffer)
            {
                socket.send(boost::asio::buffer(buffer));
            }
        };
    } // namespace protocol
} // namespace impl
