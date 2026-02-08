#pragma once

#include <string>
#include <system_error>

enum class HttpError
{
    invalid_request = 1,  // 400
    unauthorized,         // 401
    forbidden,            // 403
    not_found,            // 404
    rate_limited,         // 429
    server_error          // 500
};

struct HttpErrorCategory : public std::error_category
{
    // Unique name for the category (e.g., "http")
    const char* name() const noexcept override { return "http"; }

    // Convert error codes to human-readable messages
    std::string message(int ev) const override
    {
        switch (static_cast<HttpError>(ev))
        {
        case HttpError::invalid_request:    return "400 Bad Request";
        case HttpError::unauthorized:   return "401 Unauthorized";
        case HttpError::forbidden:  return "403 Forbidden";
        case HttpError::not_found:  return "404 Not Found";
        case HttpError::rate_limited:   return "429 Too Many Requests";
        case HttpError::server_error:   return "500 Internal Server Error";
        default: return "Unknown HTTP Error";
        }
    }
};

// Singleton accessor for the category
const std::error_category& http_category() noexcept
{
    static HttpErrorCategory instance;
    return instance;
}
