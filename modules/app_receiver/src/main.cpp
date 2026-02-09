#include "policy_tcp_boost.hpp"
#include "policy_udp_boost.hpp"
#include "policy_mode.hpp"
#include "server.h"

#include "Utils.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>

#include "logging.hpp"

using namespace impl;

int
main(int argc, char *argv[])
{
    auto [path, port] = parsePathAndPort(argc, argv);

    try
    {
        boost::asio::io_context io;
        
        Server<protocol::TCP, mode::Sync> server(io, port);
        server.setPacketHandler(packetHandler(path, server) );

        if (auto ec = server.start(); ec)
        {
            logging::Log("Server failed:" + server_error_message(ec));
        }

        io.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}