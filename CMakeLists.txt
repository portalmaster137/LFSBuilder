cmake_minimum_required(VERSION 4.0)
project(LFSBuilder)

set(CMAKE_CXX_STANDARD 20)

add_executable(LFSBuilder
        src/app.cpp
        src/ToolsCheck.cpp
        src/Partitioning.cpp
)

target_include_directories(LFSBuilder PRIVATE include external)
