cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(/usr/local/lib) 

add_subdirectory(Common)
add_subdirectory(StatUtil)
add_subdirectory(PerfTestUtil)
add_subdirectory(PredictUtil)
add_subdirectory(ScreenshotUtil)
