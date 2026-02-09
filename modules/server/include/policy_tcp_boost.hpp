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
        struct TCP
        {
            using Context = boost::asio::io_context;            
            using Protocol = boost::asio::ip::tcp;
            using Endpoint = Protocol::endpoint;
            using Socket = Protocol::socket;
            using Acceptor = Protocol::acceptor;
            using error_code = boost::system::error_code;

            static Endpoint makeEndpoint(unsigned short port) { return Endpoint(Protocol::v4(), port); }
            static auto makeBuffer(std::vector<char>& buffer) { return boost::asio::buffer(buffer); }

            static std::expected<void, std::error_code>
            openSocket(Socket& socket, std::optional<Acceptor>& acceptor, const Endpoint& endpoint, Context& io)
            {
                boost::system::error_code ec;
                acceptor.emplace(io, endpoint); // bind + listen

                logging::Log("TCP server listening on port:" + std::to_string(endpoint.port()));
                return {};
            }
        };


    } // namespace protocol
} // namespace impl
