
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <chrono>
#include <iostream>
#include <ostream>

#include "UDPClient.h"
#include "Utils.hpp"
#include "VideoFileReader.hpp"

int main(int argc, char* argv[])
{
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
        boost::asio::io_context::strand strand(io);
        boost::asio::steady_timer timer(io, std::chrono::seconds(1));

        auto videoFramesReader = std::make_shared<VideoFileReader>(args["file"].as<std::string>());

        timer.async_wait(
            [&](auto... vn)
            {
                auto client = std::make_shared<UDPClient>(io, args["ip"].as<std::string>(), args["port"].as<int>());
                client->connect();

                sendVideoFrameEvery10ms(videoFramesReader, client);
            });

        io.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
