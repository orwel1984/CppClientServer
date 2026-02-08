#pragma once

#include "server_error.h"
#include "logging.hpp"

#include <boost/asio.hpp>
#include <string>
#include <expected>

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

            static std::expected<void, std::error_code>
            openSocket(Socket& socket, const Endpoint& endpoint)
            {
                boost::system::error_code ec;
                
                socket.open(endpoint.protocol(), ec);
                    if (ec) return unexpected_error(ServerError::socket_open_failed);

                socket.bind(endpoint, ec);
                    if (ec) return unexpected_error(ServerError::bind_failed);

                logging::Log("Server started on port:" + std::to_string(endpoint.port()));
                return std::expected<void, error_code>{};  // success
            }
        };

    } //namespace protocol
} //namespace impl