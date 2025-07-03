# Server library
add_library(Server src/UDPServer.cpp)

target_include_directories(Server
    PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Server PRIVATE Boost::chrono  )