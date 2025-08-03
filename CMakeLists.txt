cmake_minimum_required(VERSION 3.20)
project(ImageFormatter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(STB_INCLUDE_DIR "${THIRD_PARTY_DIR}/stb")
set(CXXOPTS_INCLUDE_DIR "${THIRD_PARTY_DIR}/cxxopts")

set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

add_library(ImageCore STATIC
    src/Image.cpp
)

target_include_directories(ImageCore PUBLIC
    ${PROJECT_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
)

target_compile_features(ImageCore PUBLIC cxx_std_20)

add_library(ImageOperations STATIC
    src/operations/DarkenOperation.cpp
    src/operations/InvertOperation.cpp
    src/operations/ReflectHorizontallyOperation.cpp
    src/operations/ReflectVerticallyOperation.cpp
    src/operations/Rotate90DegreesClockwiseOperation.cpp
    src/operations/Rotate90DegreesCounterclockwiseOperation.cpp
    src/operations/RotateOperation.cpp
    src/operations/SobelOperatorOperation.cpp
    src/operations/SteganographyEncodeOperation.cpp
    src/operations/SteganographyDecodeOperation.cpp
    src/operations/OperationPipeline.cpp
)

target_include_directories(ImageOperations PUBLIC
    ${PROJECT_INCLUDE_DIR}
)

target_link_libraries(ImageOperations PUBLIC
    ImageCore
)

target_compile_features(ImageOperations PUBLIC cxx_std_20)

add_executable(imageformatter 
    src/main.cpp
)

target_include_directories(imageformatter PRIVATE
    ${PROJECT_INCLUDE_DIR}
    ${CXXOPTS_INCLUDE_DIR}
)

target_link_libraries(imageformatter PRIVATE
    ImageCore
    ImageOperations
)

target_compile_features(imageformatter PRIVATE cxx_std_20)

install(TARGETS imageformatter
    RUNTIME DESTINATION bin
)

message(STATUS "ImageFormatter Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")

