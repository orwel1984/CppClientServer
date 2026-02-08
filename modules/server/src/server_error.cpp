#include "server_error.h"

std::error_code make_error_code(ServerError e) { return {static_cast<int>(e), server_category()}; }