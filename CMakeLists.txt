cmake_minimum_required(VERSION 3.10)
project(MemoryRiver)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# The main executable that OJ will use
add_executable(code main.cpp)
