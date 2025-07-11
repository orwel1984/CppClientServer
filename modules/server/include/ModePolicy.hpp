
#pragma once
#include <vector>
#include <atomic>

template <typename BackendPolicy>
struct AsyncMode
{
    using Socket = typename BackendPolicy::Socket;
    using Endpoint = typename BackendPolicy::Endpoint;

    template<typename Handler>
    static void run(const std::atomic<bool>& isRunning,Socket& socket, Endpoint& remote, std::vector<char>& buffer, Handler&& handler)
    {
        socket.async_receive_from( BackendPolicy::makeBuffer(buffer), remote, 
            [&](auto ec, auto size)
            {
                if (ec){ Log("Receive error", ec); return;}                
                Log("Received packet of size: " + std::to_string(size) + " from " + remote.address().to_string() + ":" + std::to_string(remote.port()));

                // process packet
                handler(ec, size, std::string_view(buffer.data(), size));
                
                // recursive call
                if(isRunning){
                    run(isRunning, socket, remote, buffer, std::forward<decltype(handler)>(handler));
                }
            }
        );
    }
};

template<typename BackendPolicy>
struct SyncMode {

    using Socket = typename BackendPolicy::Socket;
    using Endpoint = typename BackendPolicy::Endpoint;
    using Error_Code = typename BackendPolicy::error_code;

    template<typename Handler>
    static void run(const std::atomic<bool>& isRunning, Socket& socket, Endpoint& remote, std::vector<char>& buffer, Handler&& handler)
    {
        while(isRunning)
        {
            // Receive packet
            Error_Code ec;
            std::size_t size = socket.receive_from(BackendPolicy::makeBuffer(buffer), remote, 0, ec);
    
            // Process packet
            handler(ec, size, std::string_view(buffer.data(), size));
        }
    }
};
