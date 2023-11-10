cmake_minimum_required(VERSION 2.8...3.5)

project(CallCenterEmulation VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_COMPILER_VERSION "9.5") # Указать версию g++

set(SOURCE_FILES src/main.cpp)

add_executable(CallCenterEmulation ${SOURCE_FILES})

include(CPack)
