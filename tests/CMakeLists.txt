# Tests CMakeLists.txt
# This file configures test executables

# Example test - you can add more tests here
add_executable(example_test example_test.cpp)
target_link_libraries(example_test gtest gtest_main gmock gmock_main)
add_test(NAME example_test COMMAND example_test)

# Add more tests as needed:
# add_executable(client_test client_test.cpp)
# target_link_libraries(client_test gtest gtest_main gmock gmock_main)
# add_test(NAME client_test COMMAND client_test) 