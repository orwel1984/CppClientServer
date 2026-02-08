#pragma once

#include "server_error.h"
#include "logging.hpp"

#include <boost/asio.hpp>
#include <string>

namespace impl
{
    namespace protocol
    {
        struct UDP
        {
            using Context = boost::asio::io_context;
            using Protocol = boost::asio::ip::udp;
            using Endpoint = Protocol::endpoint;
            using Socket = boost::asio::ip::udp::socket;
            using error_code = boost::system::error_code;

            static auto makeBuffer(std::vector<char>& buffer) { return boost::asio::buffer(buffer); }
            static Endpoint makeEndpoint(unsigned short port) { return Endpoint(Protocol::v4(), port); }

            static std::error_code 
            openSocket(Socket& socket, const Endpoint& endpoint)
            {
                boost::system::error_code ec;
                
                socket.open(endpoint.protocol(), ec);

                if (ec)
                {
                    logging::Log("Socket open failed", ec); // Log Boost error
                    return std::error_code(static_cast<int>(ServerError::socket_open_failed), server_category());
                }

                socket.bind(endpoint, ec);

                if (ec)
                {
                    logging::Log("Socket bind failed", ec); // Log Boost error
                    return std::error_code(static_cast<int>(ServerError::bind_failed), server_category());
                }
                logging::Log("Server started on port:" + std::to_string(endpoint.port()));

                return {};  // success
            }
        };

    } //namespace protocol
} //namespace impl