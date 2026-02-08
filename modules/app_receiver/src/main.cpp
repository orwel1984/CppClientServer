#include "policy_udp_boost.hpp"
#include "policy_mode.hpp"
#include "server.h"

#include "Utils.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>

using namespace impl;

int
main(int argc, char *argv[])
{
    auto [path, port] = parsePathAndPort(argc, argv);

    try
    {
        boost::asio::io_context io;
        
        Server<protocol::UDP, mode::Async> server(io, port);
        server.setPacketHandler(packetHandler(path, server) );
        server.start();

        io.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}