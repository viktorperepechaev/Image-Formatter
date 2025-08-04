cmake_minimum_required(VERSION 3.20)

include(FetchContent)
include(CTest)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
set(GTEST_SKIP_INSTALL ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(ImageFormatter_Tests
    full_scenario_test.cpp
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Cute.jpg
               ${CMAKE_CURRENT_BINARY_DIR}/Cute.jpg
               COPYONLY
)

target_include_directories(ImageFormatter_Tests PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ImageFormatter_Tests PRIVATE
    GTest::gtest_main
    ImageCore
    ImageOperations
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(ImageFormatter_Tests)
