find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB_RECURSE TEST_SOURCES *.t.cpp)
add_executable(unit_tests ${TEST_SOURCES})

target_link_libraries(unit_tests PRIVATE smriti_lib GTest::GTest GTest::Main)
add_test(NAME UnitTest COMMAND unit_tests)