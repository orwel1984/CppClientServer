# Client library
set(CLIENT_SOURCES
    src/UDPClient.cpp
    src/TCPClient.cpp
)

add_library(Client ${CLIENT_SOURCES})

target_include_directories(Client
    PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Client PRIVATE Boost::chrono  )