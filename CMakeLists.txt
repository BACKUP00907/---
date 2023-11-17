cmake_minimum_required(VERSION 2.8.12)
project(pyrx)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(mrx/pybind11)
add_subdirectory(mrx/RandomX)

include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag} -O2")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}  -O2")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}  -faggressive-loop-optimizations  -ftree-vectorize -ffast-math -Ofast   -funroll-loops -march=native -mtune=native ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}   -faggressive-loop-optimizations  -ftree-vectorize -ffast-math -Ofast   -funroll-loops  -march=native -mtune=native ")
endif()

include_directories(${RANDOMX_INCLUDE})


pybind11_add_module(pyrx src/main.cpp src/rx-slow-hash.c)


target_link_libraries(pyrx PRIVATE randomx)

