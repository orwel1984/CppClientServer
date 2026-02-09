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
            using Acceptor = Protocol::acceptor;
            using Socket = Protocol::socket;
            using error_code = boost::system::error_code;

            static auto makeBuffer(std::vector<char>& buffer) { return boost::asio::buffer(buffer); }
            static Endpoint makeEndpoint(unsigned short port) { return Endpoint(Protocol::v4(), port); }

            // Same signature as UDP: openSocket(Socket& socket, endpoint).
            // Inside: create acceptor, open/bind/listen, then acceptor.accept(socket).
            static std::expected<void, std::error_code>
            openSocket(Socket& socket, const Endpoint& endpoint)
            {
                boost::system::error_code ec;
                auto& io = static_cast<Context&>(socket.get_executor().context());

                Acceptor acceptor(io);
                acceptor.open(endpoint.protocol(), ec);
                if (ec) return unexpected_error(ServerError::socket_open_failed);

                acceptor.bind(endpoint, ec);
                if (ec) return unexpected_error(ServerError::bind_failed);

                acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
                if (ec) return unexpected_error(ServerError::socket_open_failed);

                acceptor.accept(socket, ec);
                if (ec) return unexpected_error(ServerError::receive_failed);

                logging::Log("Server started on port:" + std::to_string(endpoint.port()));
                return {};
            }
        };

    } // namespace protocol
} // namespace impl

// TCP uses stream read (read_some), not receive_from. Specialize Sync for TCP.
#include "policy_mode.hpp"

namespace impl { namespace mode {

template <>
struct Sync<impl::protocol::TCP>
{
    using Socket = impl::protocol::TCP::Socket;
    using Endpoint = impl::protocol::TCP::Endpoint;
    using Error_Code = impl::protocol::TCP::error_code;

    template <typename Handler>
    static std::expected<void, std::error_code>
    receivePacket(
        const std::atomic<bool>& isRunning,
        Socket& socket,
        Endpoint& /*remote*/,
        std::vector<char>& buffer,
        Handler&& handler)
    {
        while (isRunning)
        {
            Error_Code ec;
            std::size_t size = socket.read_some(impl::protocol::TCP::makeBuffer(buffer), ec);
            handler(toServerError(ec), size, std::string_view(buffer.data(), size));
            if (ec) return std::unexpected(toServerError(ec));
        }
        return {};
    }
};

}} // namespace impl::mode
