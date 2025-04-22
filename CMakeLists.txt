cmake_minimum_required(VERSION 3.10)

# Make sure this matches ./NAM/version.h!
project(NAM VERSION 0.3.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include_directories(SYSTEM /usr/local/include)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    link_libraries(stdc++fs)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
else()
    message(FATAL_ERROR "Unrecognized Platform!")
endif()

set(NAM_DEPS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies")

# Explicitly tell CMake dependencies are
include_directories(${NAM_DEPS_PATH}/eigen)
include_directories(${NAM_DEPS_PATH}/nlohmann)

# Add a shared library target for libNAM.a
add_library(NAM STATIC
    NAM/activations.cpp
    NAM/convnet.cpp
    NAM/dsp.cpp
    NAM/get_dsp.cpp
    NAM/lstm.cpp
    NAM/util.cpp
    NAM/wavenet.cpp
)

target_include_directories(NAM PUBLIC ${CMAKE_SOURCE_DIR}/NAM)
target_compile_options(NAM PRIVATE -fPIC)
set_target_properties(NAM PROPERTIES OUTPUT_NAME "NAM")
