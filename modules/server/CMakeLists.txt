# Server library
set(SERVER_SOURCES
    src/UDPServer.cpp
    src/Server.cpp
)
add_library(Server ${SERVER_SOURCES})

target_include_directories(Server
    PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(Server PRIVATE Boost::chrono  )


# Server Tests 
add_executable(ServerTests test/UDPServerTests.cpp)

target_include_directories(ServerTests
    PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(ServerTests gtest_main gmock_main Server)
add_test(NAME ServerTests COMMAND ServerTests)
