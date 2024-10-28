# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Project Name and Version
project(VectorPong VERSION 1.0)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Packages using pkg-config
find_package(PkgConfig REQUIRED)

# Find SDL2
pkg_search_module(SDL2 REQUIRED sdl2)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found!")
endif()

# Find SDL2_ttf
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
if(NOT SDL2_TTF_FOUND)
    message(FATAL_ERROR "SDL2_ttf not found!")
endif()

# Include Directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Link Directories
link_directories(
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_TTF_LIBRARY_DIRS}
)

# Compiler Flags
add_definitions(
    ${SDL2_CFLAGS_OTHER}
    ${SDL2_TTF_CFLAGS_OTHER}
)

# Source Files
set(SOURCES
    pong.cpp
)

# Add Executable
add_executable(VectorPong ${SOURCES})

# Link Libraries
target_link_libraries(VectorPong
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)
