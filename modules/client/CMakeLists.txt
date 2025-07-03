# Client library
add_library(Client src/UDPClient.cpp)

target_include_directories(Client
    PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Client PRIVATE Boost::chrono  )