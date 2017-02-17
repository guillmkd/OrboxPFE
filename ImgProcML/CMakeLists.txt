cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_subdirectory(ImgProcML/Common)
add_subdirectory(ImgProcML/StatUtil)
add_subdirectory(ImgProcML/PerfTestUtil)
add_subdirectory(ImgProcML/PredictUtil)
# add_subdirectory(ImgProcML/ScreenshotUtil)