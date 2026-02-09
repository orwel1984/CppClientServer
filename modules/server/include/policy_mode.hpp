
#pragma once
#include "server_error.h"
#include <atomic>
#include <vector>
#include <expected>

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
            static std::expected<void, std::error_code>
            receivePacket(
                const std::atomic<bool>& isRunning,
                Socket& socket,
                Endpoint& remote,
                std::vector<char>& buffer,
                Handler&& handler)
            {
                socket.async_receive_from(  Protocol::makeBuffer(buffer), remote,
                    [&, handler = std::forward<Handler>(handler)]
                    (auto ec, auto size) mutable
                    {
                        if (ec) { handler(toServerError(ec), 0, {}); return; }
                        handler(toServerError(ec), size, std::string_view(buffer.data(), size));

                        if (isRunning) {
                            receivePacket(isRunning, socket, remote, buffer, std::move(handler));
                        }
                    });
                return {}; // async started successfully
            }
        };

        template <typename Protocol>
        struct Sync
        {
            using Socket = typename Protocol::Socket;
            using Endpoint = typename Protocol::Endpoint;
            using Error_Code = typename Protocol::error_code;

            template <typename Handler>
            static std::expected<void, std::error_code>
            receivePacket(
                const std::atomic<bool>& isRunning,
                Socket& socket,
                Endpoint& remote,
                std::vector<char>& buffer,
                Handler&& handler)
            {
                while (isRunning)
                {
                    Error_Code ec;
                    std::size_t size = socket.receive_from( Protocol::makeBuffer(buffer),remote, 0, ec);

                    if (ec) return std::unexpected(toServerError(ec));

                    handler(toServerError(ec), size, std::string_view(buffer.data(), size));
                }
                return {};
            }
        };

    } // namespace mode
} // namespace impl
