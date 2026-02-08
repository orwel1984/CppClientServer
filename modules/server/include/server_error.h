#pragma once

#include <system_error>

enum class ServerError {
    success = 0,
    socket_open_failed,
    bind_failed,
    send_failed,
    receive_failed,
    timeout,
    connection_reset,
    unknown
};

struct ServerErrorCategory : public std::error_category 
{    
    const char* name() const noexcept override { return "server"; }

    std::string message(int ev) const override
    {
        switch (static_cast<ServerError>(ev)) {
            case ServerError::success: return "Success";
            case ServerError::socket_open_failed: return "Failed to open socket";
            case ServerError::bind_failed: return "Failed to bind socket";
            case ServerError::send_failed: return "Failed to send data";
            case ServerError::receive_failed: return "Failed to receive data";
            case ServerError::timeout: return "Operation timed out";
            case ServerError::connection_reset: return "Connection reset by peer";
            default: return "Unknown server error";
        }
    }
};

// static instance of this error_category
inline const std::error_category& server_category() noexcept 
{
    static ServerErrorCategory instance;
    return instance;
}

namespace std 
{
    template <>
    struct is_error_code_enum<ServerError> : true_type {};
}

std::error_code make_error_code(ServerError e);
