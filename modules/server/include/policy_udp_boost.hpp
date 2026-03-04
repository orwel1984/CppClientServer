#pragma once

// Project
#include "server_error.h"
#include "logging.hpp"

// Standard library
#include <cstddef>
#include <string>
#include <vector>
#include <optional>
#include <expected>
#include <system_error>

// Boost
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/system/error_code.hpp>
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
            using Acceptor = std::nullptr_t; 
            using error_code = boost::system::error_code;

            static auto makeBuffer(std::vector<char>& buffer) { return boost::asio::buffer(buffer); }
            static Endpoint makeEndpoint(unsigned short  port) { return Endpoint(Protocol::v4(), port); }

            static std::expected<void, std::error_code>
            openSocket(Socket& socket, std::optional<std::nullptr_t>&, const Endpoint& endpoint, Context& io)
            {
                boost::system::error_code ec;                
                socket.open(endpoint.protocol(), ec); if (ec) return unexpected_error(ServerError::socket_open_failed);
                socket.bind(endpoint, ec); if (ec) return unexpected_error(ServerError::bind_failed);

                logging::Log("Server started on port:" + std::to_string(endpoint.port()));
                return {};
            }
        };

    } //namespace protocol
} //namespace impl