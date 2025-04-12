cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(JPLog2)

cmake_policy(SET CMP0074 NEW)

# choose "Delete Cache and Reconfigure"
set(Nuke_ROOT "C:/Program Files/Nuke12.1v2") # manual

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include_directories(${CMAKE_SOURCE_DIR})
find_package(Nuke REQUIRED)

message("##################################")
message("Using Nuke ${NUKE_VERSION_MAJOR}.${NUKE_VERSION_MINOR}v${NUKE_VERSION_RELEASE}")
message("##################################")

if (UNIX)
    add_compile_options(
        -DUSE_GLEW -fPIC -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx
    )
endif()

find_package(OpenGL)
if (NOT OpenGL_FOUND)
    message(WARNING "Couldn't find OpenGL")   
endif()

if (NUKE_VERSION_MAJOR VERSION_GREATER_EQUAL 15.0)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1")
    set(CMAKE_CXX_STANDARD 17)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")
    set(CMAKE_CXX_STANDARD 14)
endif()

# add sub directory
add_subdirectory(src)