
#pragma once
#include <atomic>
#include <vector>
#include "logging.hpp"

namespace impl
{
    namespace mode
    {
        template <typename Protocol>
        struct Async
        {
            using Socket = typename Protocol::Socket;
            using Endpoint = typename Protocol::Endpoint;

            template <typename Handler>
            static void
            receivePacket(const std::atomic<bool> &isRunning, Socket &socket, Endpoint &remote,
                std::vector<char> &buffer, Handler &&handler)
            {
                socket.async_receive_from(Protocol::makeBuffer(buffer), remote,
                    [&](auto ec, auto size) // completion handler
                    {
                        if (ec)
                        {
                            logging::Log("Receive error", ec);
                            return;
                        }
                            logging::Log("Received packet of size: " + std::to_string(size) + " from " +
                                remote.address().to_string() + ":" + std::to_string(remote.port()));

                        // process packet
                        handler(ec, size, std::string_view(buffer.data(), size));

                        // recursive
                        if (isRunning)
                        {
                            receivePacket(isRunning, socket, remote, buffer, std::forward<decltype(handler)>(handler));
                        }
                    });
            }
        };

        template <typename Protocol>
        struct Sync
        {
            using Socket = typename Protocol::Socket;
            using Endpoint = typename Protocol::Endpoint;
            using Error_Code = typename Protocol::error_code;

            template <typename Handler>
            static void
            receivePacket(const std::atomic<bool> &isRunning, Socket &socket, Endpoint &remote,
                std::vector<char> &buffer, Handler &&handler)
            {
                while (isRunning)
                {
                    // Receive packet
                    Error_Code ec;
                    std::size_t size = socket.receive_from(Protocol::makeBuffer(buffer), remote, 0, ec);

                    // Process packet
                    handler(ec, size, std::string_view(buffer.data(), size));
                }
            }
        };

    } // namespace mode
} // namespace impl
