# Google Test Helper for CMake
# This file provides functions to download and configure Google Test

function(configure_googletest)
    message(STATUS "Configuring Google Test...")
    
    # Fetch Google Test from GitHub
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip 
    )    
    FetchContent_MakeAvailable(googletest)
    
    message(STATUS "Google Test configured successfully")
endfunction()

# Function to add a test executable
function(add_googletest TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    target_link_libraries(${TEST_NAME} gtest gtest_main gmock gmock_main)
    
    # Add the test to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    # Set working directory to project root for tests
    set_tests_properties(${TEST_NAME} PROPERTIES
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endfunction() 