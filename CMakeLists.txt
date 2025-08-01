# ===== ORIGINAL CMakeLists.txt (kept for reference) =====
# cmake_minimum_required(VERSION 3.11)
# project("ImageFormatter")
# 
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
# 
# add_library(IMAGE
#   src/Image.cpp
# )
# 
# target_include_directories(IMAGE PRIVATE
#   include/imageformatter/
#   third_party/stb/
# ) 
# 
# add_executable(imageformatter src/main.cpp)
# 
# target_link_libraries(imageformatter PRIVATE
#   IMAGE
# )
# 
# target_include_directories(imageformatter PRIVATE
#   third_party/cxxopts
# )
# ===== END ORIGINAL =====

cmake_minimum_required(VERSION 3.20)
project(ImageFormatter VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Debug/Release configurations
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Third-party include directories
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(STB_INCLUDE_DIR "${THIRD_PARTY_DIR}/stb")
set(CXXOPTS_INCLUDE_DIR "${THIRD_PARTY_DIR}/cxxopts")

# Project include directories
set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Image Core Library
add_library(ImageCore STATIC
    src/Image.cpp
)

target_include_directories(ImageCore PUBLIC
    ${PROJECT_INCLUDE_DIR}
    ${STB_INCLUDE_DIR}
)

target_compile_features(ImageCore PUBLIC cxx_std_20)

# Operations Library
add_library(ImageOperations STATIC
    src/operations/DarkenOperation.cpp
    src/operations/InvertOperation.cpp
    src/operations/ReflectHorizontallyOperation.cpp
    src/operations/ReflectVerticallyOperation.cpp
    src/operations/Rotate90DegreesClockwiseOperation.cpp
    src/operations/Rotate90DegreesCounterclockwiseOperation.cpp
    src/operations/RotateOperation.cpp
    src/operations/SobelOperatorOperation.cpp
    src/operations/OperationPipeline.cpp
)

target_include_directories(ImageOperations PUBLIC
    ${PROJECT_INCLUDE_DIR}
)

target_link_libraries(ImageOperations PUBLIC
    ImageCore
)

target_compile_features(ImageOperations PUBLIC cxx_std_20)

# Main executable
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

# Platform-specific linking
if(UNIX AND NOT APPLE)
    target_link_libraries(imageformatter PRIVATE m) # Math library on Linux
endif()

# Install targets
install(TARGETS imageformatter
    RUNTIME DESTINATION bin
)

# Enable testing if this is the main project
#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
#    include(CTest)
#    if(BUILD_TESTING)
#        add_subdirectory(tests)
#    endif()
#endif()

# Print configuration info
message(STATUS "ImageFormatter Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")

