cmake_minimum_required(VERSION 3.11)
project("ImageFormatter")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(IMAGE
  src/Image.cpp
)

target_include_directories(IMAGE PRIVATE
  include/imageformatter/
  third_party/stb/
) 

add_executable(imageformatter src/main.cpp)

target_link_libraries(imageformatter PRIVATE
  IMAGE
)

target_include_directories(imageformatter PRIVATE
  third_party/cxxopts
)

