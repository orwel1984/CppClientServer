#pragma once

#include <iostream>
#include <optional>
#include <string_view>
#include <utility>
#include <string>
#include <cstdlib>

#include "FileWriter.hpp"
#include "TimestampsLogger.hpp"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

std::optional<boost::program_options::variables_map> 
parseCommandline(int argc, char* argv[])
{
    namespace opt = boost::program_options;
    opt::variables_map args;
    opt::options_description desc("All arguments to pass to the Receiver");
    desc.add_options()
        ("file", opt::value<std::string>(), "Path where to save the file")
        ("port", opt::value<int>(), "Port number of the server")
        ("help", "Help message");
    opt::positional_options_description pos_desc;
    pos_desc.add("file", 1);
    pos_desc.add("port", 1);

    try
    {
        opt::store(opt::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(),
                   args);
        opt::notify(args);
        if (!args.count("port") || !args.count("file"))
        {
            std::cout << "Missing required arguments: port, or file.\n";
            std::cout << desc << "\n";
            return {};
        }
        if(args.count("help"))
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


// Helper: parse path and port from commandline and exit(1) on failure
inline std::pair<std::string, int>
parsePathAndPort(int argc, char* argv[])
{
    auto commandlineOpt = parseCommandline(argc, argv);
    if (!commandlineOpt)
    {
        std::cout << "Failed to parse command line arguments." << std::endl;
        std::exit(1);
    }

    const auto &args = *commandlineOpt;
    const int port = args["port"].as<int>();
    const std::string path = args["file"].as<std::string>();
    return {path, port};
}


template <typename ReceiverT>
static auto
packetHandler(const std::string_view path, ReceiverT &receiver)
{
    return [path, &receiver](const std::error_code &ec, std::size_t len, std::string_view packet)
    {
        FileWriter fileWriter(path);
        TimestampLogger logger("timestamps.txt");

       logger.timestamp();
        if (packet == "eof")
        {
            logging::Log("EOF received — closing file and shutting down receiver");
            receiver.shutdown();
            return;
        }
        fileWriter.writeChunk(packet);
        logging::Log("Received packet — size=" + std::to_string(len) + " bytes");
    };
}


