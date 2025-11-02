cmake_minimum_required(VERSION 4.0)
project(LFSBuilder)

set(CMAKE_CXX_STANDARD 20)

add_executable(LFSBuilder
        src/app.cpp
        src/ToolsCheck.cpp
        src/Partitioning.cpp
        src/RunCmd.cpp
        include/SetupSources.h
        src/SetupSources.cpp
        include/SetupLFSUser.h
        src/SetupLFSUser.cpp
        include/BuildSources.h
        src/sources/BinUtilsPass1.cpp
)

target_include_directories(LFSBuilder PRIVATE include external)
