#pragma once

// client side protocol helpers for TCP

#include <vector>
#include <string>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/error_code.hpp>

namespace impl
{
    namespace protocol
    {
        struct TCP
        {
            using Context   = boost::asio::io_context;
            using Protocol  = boost::asio::ip::tcp;
            using Endpoint  = Protocol::endpoint;
            using Socket    = Protocol::socket;
            using Resolver  = Protocol::resolver;
            using error_code = boost::system::error_code;

            // resolver needs an address family object
            // some asio resolvers take a 'protocol' object; the easiest
            // thing is to return an IPv4 protocol instance directly.
            static Protocol version() { return Protocol::v4(); }

            // connect/close helpers that mirror the old concrete TCPClient
            static void connectSocket(Socket &socket, const Endpoint &endpoint)
            {
                socket.connect(endpoint);
            }

            static void disconnectSocket(Socket &socket)
            {
                if (socket.is_open()) {
                    error_code ec;
                    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                    socket.close(ec);
                }
            }

            static void send(Socket &socket, const std::vector<std::byte> &buffer)
            {
                boost::asio::write(socket, boost::asio::buffer(buffer));
            }
        };
    } // namespace protocol
} // namespace impl
