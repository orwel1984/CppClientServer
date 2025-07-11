#pragma once

#include <boost/asio.hpp>
#include <system_error>

struct BoostUDPPolicy
{
    using Context = boost::asio::io_context;
    using Protocol = boost::asio::ip::udp;
    using Endpoint = Protocol::endpoint;
    using Socket = boost::asio::ip::udp::socket;
    using error_code = boost::system::error_code;

    static Endpoint makeEndpoint(unsigned short port) { return Endpoint(Protocol::v4(), port); }

    static std::error_code openAndBindSocket(Socket& socket, const Endpoint& endpoint)
    {
        boost::system::error_code ec;

        socket.open(endpoint.protocol(), ec);
        if (ec)
        {
            return ec;
        }

        socket.bind(endpoint, ec);
        if (ec)
        {
            return ec;
        }

        return {};  // success
    }
};

template <typename BackendPolicy>
struct AsyncMode
{
    using Socket = typename BackendPolicy::Socket;
    using Endpoint = typename BackendPolicy::Endpoint;

    template<typename Handler>
    static void run(Socket& socket, Endpoint& remote, std::vector<char>& buffer, Handler&& handler)
    {
        socket.async_receive_from(boost::asio::buffer(buffer), remote, std::forward<decltype(handler)>(handler));
    }
};
