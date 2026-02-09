#pragma once

#include <system_error>
#include <expected>
#include <string>

#include <boost/asio/error.hpp>
#include <boost/system/error_code.hpp>

enum class ServerError {
    success = 0,
    socket_open_failed,
    bind_failed,
    send_failed,
    receive_failed,
    timeout,
    connection_reset,
    close_failed,
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
            case ServerError::close_failed: return "Failed to close socket";
            case ServerError::unknown: return "Unknown server error";
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

inline std::error_code make_error_code(ServerError e)
{
    return {static_cast<int>(e), server_category()};
}

using EV = std::expected<void, std::error_code>;

inline EV unexpected_error(ServerError e)
{
    return std::unexpected(make_error_code(e));
}

inline std::string server_error_message(ServerError e)
{
    return server_category().message(static_cast<int>(e));
}

// Helper: translate std::error_code to message (only correct when ec is from server category)
inline std::string server_error_message(const std::error_code& ec)
{
    if (ec.category() != server_category())
        return ec.message();
    return server_category().message(ec.value());
}

// Map Boost.Asio / boost::system error to ServerError (temporary; protocol-agnostic mapping)
inline std::error_code toServerError(const boost::system::error_code& ec)
{
    if (!ec) return {};
    namespace asio_err = boost::asio::error;
    if (ec == asio_err::connection_reset || ec == asio_err::connection_refused)
        return make_error_code(ServerError::connection_reset);
    if (ec == asio_err::timed_out)
        return make_error_code(ServerError::timeout);
    return make_error_code(ServerError::receive_failed);
}

