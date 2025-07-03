# Receiver  Application
add_executable(SenderApp src/main.cpp)

target_include_directories(SenderApp
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SenderApp PRIVATE Client pthread Boost::program_options)
