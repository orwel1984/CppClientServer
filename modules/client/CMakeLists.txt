# Client library
set(CLIENT_SOURCES
    src/UDPClient.cpp
    src/TCPClient.cpp
    src/Client.cpp          # generic policy-based client
)

add_library(Client ${CLIENT_SOURCES})

target_include_directories(Client
    PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Client PRIVATE Boost::chrono  )

# Client tests
add_executable(ClientTests test/ClientTests.cpp)

target_include_directories(ClientTests
    PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ClientTests gtest_main Client)
add_test(NAME ClientTests COMMAND ClientTests)