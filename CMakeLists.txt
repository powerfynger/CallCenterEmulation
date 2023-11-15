cmake_minimum_required(VERSION 2.8...3.5)

project(CallCenterEmulation VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_COMPILER_VERSION "9.5") # Указать версию g++

set(SOURCE_FILES
    src/main.cpp
    src/CDRJournal/CDRJournal.cpp
    src/jsonParser/jsonParser.cpp
    src/HTTPServ/HTTPServ.cpp
    src/callProcessing/callProcessing.cpp
    # src/logging/logger.cpp
    src/utils/utils.cpp
)

# Тесты
# set(TEST_FILES
#     tests/jsonParserTests.cpp
#     tests/callProcessingTests.cpp
#     tests/HTTPServTests.cpp
#     tests/loggingTests.cpp
#     tests/utilsTests.cpp
#     tests/CDRJournalTests.cpp
# )

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -DELPP_NO_DEFAULT_LOG_FILE")

add_executable(CallCenterEmulation ${SOURCE_FILES})

add_library(mongoose include/mongoose.c)
add_library(CDRJournal src/CDRJournal/CDRJournal.cpp)
add_library(jsonParser src/jsonParser/jsonParser.cpp)
add_library(HTTPServ src/HTTPServ/HTTPServ.cpp)
add_library(callProcessing src/callProcessing/callProcessing.cpp)
# add_library(logging src/logging/logger.cpp)
add_library(utils src/utils/utils.cpp)

# add_executable(tests ${TEST_FILES})

# Связываем исполняемый файл с библиотеками
target_link_libraries(CallCenterEmulation
    CDRJournal
    jsonParser
    HTTPServ
    callProcessing
    # logging
    utils
    mongoose
)

# Связываем тесты с библиотеками
# target_link_libraries(tests
#     jsonParserTests
#     callProcessingTests
#     HTTPServTests
#     loggingTests
#     utilsTests
#     CDRJournalTests
# )


