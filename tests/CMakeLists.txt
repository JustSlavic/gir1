project(tests)

find_library(gtest REQUIRED)
find_library(gtest_main REQUIRED)

include_directories(include)

set(SOURCE_FILES
        ../include/logging/logging.h ../src/logging/logging.cpp
        ../include/logging/handler.h ../src/logging/handler.cpp)

set(TEST_FILES
        logging.cpp)

add_executable(tests
        ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(tests gtest gtest_main pthread)
