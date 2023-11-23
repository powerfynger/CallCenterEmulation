cmake_minimum_required(VERSION 2.8...3.5)

project(CallCenterEmulation VERSION 0.1.0 LANGUAGES C CXX)
# set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_COMPILER_VERSION "9.5")
set (CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)

FetchContent_MakeAvailable(googletest)


set(SOURCE_FILES
    src/main.cpp
    src/CDRJournal/CDRJournal.cpp
    src/jsonParser/jsonParser.cpp
    src/HTTPServ/HTTPServ.cpp
    src/callProcessing/callProcessing.cpp
    src/utils/utils.cpp
)
set(TEST_FILES
    tests/operatorCallTest.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(PRIVATE ${googletest_SOURCE_DIR}/include)
# include_directories(PRIVATE ${gmock_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -DELPP_NO_DEFAULT_LOG_FILE")


add_executable(CallCenterEmulation ${SOURCE_FILES})
add_executable(tests ${TEST_FILES})

add_library(mongoose include/mongoose.c)
add_library(CDRJournal src/CDRJournal/CDRJournal.cpp)
add_library(jsonParser src/jsonParser/jsonParser.cpp)
add_library(HTTPServ src/HTTPServ/HTTPServ.cpp)
add_library(callProcessing src/callProcessing/callProcessing.cpp)
add_library(utils src/utils/utils.cpp)

target_link_libraries(callProcessing CDRJournal)


# Связываем исполняемый файл с библиотеками
target_link_libraries(CallCenterEmulation
    # CDRJournal
    jsonParser
    HTTPServ
    callProcessing
    utils
    mongoose
)

target_link_libraries(tests
    gtest
    gtest_main
    # CDRJournal
    HTTPServ
    callProcessing
)