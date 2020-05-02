project(tests)

find_library(gtest REQUIRED)
find_library(gtest_main REQUIRED)

add_executable(tests test.cpp)

target_link_libraries(tests gtest gtest_main pthread)
