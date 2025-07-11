#include <boost/asio.hpp>
#include <iostream>

#include "FileWriter.hpp"

#include "UDPServer.h"

#include "BoostUDPPolicy.hpp"
#include "Server.h"

#include "Utils.hpp"
#include "TimestampsLogger.hpp"

int main(int argc, char* argv[])
{
    // read command-line arguments
    auto commandlineOpt = parseCommandline(argc, argv);
    if (!commandlineOpt)
    {
        std::cout << "Failed to parse command line arguments." << std::endl;
        exit(1);
    }
    const auto& args = *commandlineOpt;

    try
    {
        boost::asio::io_context io;

        FileWriter fileWriter(args["file"].as<std::string>());
        TimestampLogger logger("timestamps.txt");

        // Setup a UDP based receiver server
        // UDPServer receiver(io, args["port"].as<int>());
        Server<BoostUDPPolicy, AsyncMode> receiver(io, args["port"].as<int>());

        // define a custom Packet-Handler to receive video datagrams
        auto filePacketHandler =
            [&](const std::error_code& ec, std::size_t len, std::string_view packet)
        {
            logger.timestamp();
            if (packet != "eof")
            {
                auto error = fileWriter.writeChunk(packet);
            }
            else
            {
                std::cout << "End of file received!" << std::endl;
                receiver.shutdown();
            }
            std::cout << "Received packet of size: " << len << std::endl;
        };
        
        receiver.setPacketHandler(filePacketHandler);

        // now start listening
        receiver.start();

        io.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}