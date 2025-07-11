#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <system_error>
namespace
{
    void Log(const std::string& message) { std::cout << message << std::endl; }

    void Log(const std::error_code& error)
    {
        std::cout << "Error code: " << error.value() << std::endl;
        std::cout << "Error message: " << error.message() << std::endl;
    }

    void Log(const std::string& message, const std::error_code& error)
    {
        Log(message);
        Log(error);
    }
}  // namespace

struct BoostUDPPolicy
{
    using Context = boost::asio::io_context;
    using Protocol = boost::asio::ip::udp;
    using Endpoint = Protocol::endpoint;
    using Socket = boost::asio::ip::udp::socket;
    using error_code = boost::system::error_code;

    static auto makeBuffer(std::vector<char>& buffer) { return boost::asio::buffer(buffer); }

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
        Log("Server started on port:" + std::to_string(endpoint.port()));

        return {};  // success
    }
};
