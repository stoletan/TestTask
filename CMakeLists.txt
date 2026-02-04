cmake_minimum_required(VERSION 4.1)

project(MyTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
task
src/main.cpp
src/InputProcessing.cpp
src/TimeClub.cpp
src/Table.cpp
src/ComputerClub.cpp)

# Tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(src)

enable_testing()

add_executable(test_app
test/unitTest.cpp
src/InputProcessing.cpp
src/TimeClub.cpp
src/Table.cpp
src/ComputerClub.cpp)

target_link_libraries(test_app gtest_main gmock_main)