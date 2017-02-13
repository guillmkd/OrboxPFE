cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project(PerfTestUtil)

find_package(OpenCV REQUIRED)
add_subdirectory(../Common shared_lib)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${SHARED_LIB_INCLUDE_DIR})

set(SOURCE_FILES main.cpp ClassificationTest.cpp ClassificationTest.h)

add_executable(perfTestUtil ${SOURCE_FILES})

target_link_libraries(perfTestUtil shared_lib)
target_link_libraries(perfTestUtil ${OpenCV_LIBS})
