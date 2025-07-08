#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <optional>
#include <thread>

#include "UDPClient.h"
#include "VideoFileReader.hpp"

std::optional<boost::program_options::variables_map> parseCommandline(int argc, char* argv[])
{
    namespace opt = boost::program_options;
    opt::variables_map args;
    opt::options_description desc("All arguments to pass to the Sender");
    desc.add_options()("ip", opt::value<std::string>(), "Ip-Address of the receiving server .")(
        "port", opt::value<int>(), "Port number of the server")(
        "file", opt::value<std::string>(), "Path to the file to be sent")("help", "Help message");
    opt::positional_options_description pos_desc;
    pos_desc.add("file", 1);
    pos_desc.add("ip", 1);
    pos_desc.add("port", 1);

    try
    {
        opt::store(opt::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(),
                   args);
        opt::notify(args);
        if (!args.count("ip") || !args.count("port") || !args.count("file"))
        {
            std::cout << "Missing required arguments: ip, port, or file.\n";
            std::cout << desc << "\n";
            return {};
        }
        if (args.count("help"))
        {
            std::cout << desc << "\n";
            return {};
        }
    }
    catch (std::exception& e)
    {
        std::cout << desc << "\n";
        return {};
    }

    return args;
}

void sendFragmentedFrame(std::shared_ptr<VideoFileReader> fileReader,
                         std::shared_ptr<UDPClient> client, const std::vector<std::byte>& chunk,
                         const size_t MAX_PACKET_SIZE)
{
    for (size_t offset = 0; offset < chunk.size(); offset += MAX_PACKET_SIZE)
    {
        size_t fragmentSize = std::min(MAX_PACKET_SIZE, chunk.size() - offset);
        std::vector<std::byte> fragment(chunk.begin() + offset,
                                        chunk.begin() + offset + fragmentSize);

        client->sendPacket(fragment);

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}

void sendVideoFrameEvery10ms(std::shared_ptr<VideoFileReader> fileReader,
                             std::shared_ptr<UDPClient> client)
{
    const size_t MAX_PACKET_SIZE = 1024;

    while (auto chunk = fileReader->getNextVideoFrame())
    {
        std::cout << "Read chunk of size: " << chunk->size() << std::endl;
        if (chunk->size() <= MAX_PACKET_SIZE)
        {
            client->sendPacket(*chunk);
        }
        else
        {
            sendFragmentedFrame(fileReader, client, *chunk, MAX_PACKET_SIZE);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ms between frames
    }

    std::cout << "All frames sent." << std::endl;
    std::vector<std::byte> eofPacket = {std::byte('e'), std::byte('o'), std::byte('f')};
    client->sendPacket(eofPacket);
}
