cmake_minimum_required(VERSION 3.27)
project(VaySenderReceiver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Includes
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/BoostBuildHelper.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/GoogleTestHelper.cmake")

# Boost Configuration
set(BOOST_VERSION 1.82.0)
set(BOOST_COMPONENTS filesystem program_options chrono context coroutine system thread date_time atomic)
configure_boost(${BOOST_VERSION} "${BOOST_COMPONENTS}")
print_boost_libraries("${Boost_LIBRARIES}")

# Tests
enable_testing()
configure_googletest()
add_subdirectory(tests)

# Modules
add_subdirectory(modules/client)
add_subdirectory(modules/server)
add_subdirectory(modules/app_receiver)
add_subdirectory(modules/app_sender)
