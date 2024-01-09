cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
project(tuple-views)

find_package(range-v3 CONFIG REQUIRED)

include_directories(include)
add_subdirectory(test)
