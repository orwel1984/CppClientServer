#pragma once

#include <iostream>
#include <optional>

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


