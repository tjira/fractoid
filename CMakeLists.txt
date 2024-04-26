# minimum cmake requirement
cmake_minimum_required(VERSION 3.18)

# project name
project(Fractoid)

# include the FetchContent module
include(FetchContent)

# set some smake variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# set compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-mavx -s -O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg -Og -Wall -Wextra")

# static link if on Windows
if(WIN32)
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -static")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " -static")
endif()

# git clone libraries
FetchContent_Declare(argparse SYSTEM GIT_REPOSITORY https://github.com/p-ranav/argparse.git GIT_TAG ac4c578f6020d5164f66ccb26b4727dea657a12b)
FetchContent_Declare(json SYSTEM GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG a259ecc51e1951e12f757ce17db958e9881e9c6c)
FetchContent_Declare(stb SYSTEM GIT_REPOSITORY https://github.com/nothings/stb.git GIT_TAG ae721c50eaf761660b4f90cc590453cdb0c2acd0)

# fetch the dependencies
FetchContent_MakeAvailable(argparse json stb)

# include directories
include_directories(include ${argparse_SOURCE_DIR}/include ${json_SOURCE_DIR}/include ${stb_SOURCE_DIR})

# find OpenMP
find_package(OpenMP REQUIRED)

# add fractoid executable
add_executable(fractoid src/image.cpp src/main.cpp)

# link OpenMP on release build
if(${CMAKE_BUILD_TYPE} STREQUAL Release)
    target_link_libraries(fractoid OpenMP::OpenMP_CXX)
endif()
