cmake_minimum_required(VERSION 3.13)

project(smriti-cpp)
set(CMAKE_BUILD_TYPE Debug) # Set default build type to Debug


enable_testing()
add_subdirectory(tests)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
find_package(asio CONFIG REQUIRED)

add_library(
    smriti_lib
    src/server.cpp
    src/session.cpp
    src/resp_value.cpp
    src/parser.cpp
    src/processor.cpp
    src/utils_string.cpp
)
target_include_directories(smriti_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(smriti_lib PRIVATE
    asio
    asio::asio
    Boost::system
    Threads::Threads)

add_executable(server src/smriti.m.cpp)
target_compile_options(server PRIVATE -g) # Add debug symbols
target_link_libraries(server PRIVATE smriti_lib
    asio
    asio::asio
    Boost::system
    Threads::Threads)
