# Receiver  Application
add_executable(ReceiverApp src/main.cpp)

target_include_directories(ReceiverApp
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ReceiverApp PRIVATE Server Boost::program_options)
