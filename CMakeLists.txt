# Build with -Dcppcoro_DIR=<cppcoro-build-dir> -Dcppcoro_LIB=<cpp-coro-lib.lib>
# Example: -Dcppcoro_DIR=D:\\Programs\\cppcoro\\build\\cppcoro -Dcppcoro_LIB=D:\\Programs\\cppcoro\\cmake-build-release\\lib\\cppcoro.lib
cmake_minimum_required(VERSION 3.16)
project(coroutine_examples)

set(CMAKE_CXX_STANDARD 20)

find_package(cppcoro REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE cppcoro::cppcoro)
# For some reason cppcoro doesn't link, so I have to set up path to static library manually
target_link_libraries(main PRIVATE ${cppcoro_LIB})
