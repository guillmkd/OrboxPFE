cmake_minimum_required(VERSION 3.2)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project(ScreenshotUtil)

find_package(OpenCV REQUIRED)
#find_package(WiringPi REQUIRED)
#find_package(Threads REQUIRED)
add_subdirectory(../Common shared_lib)

#include_directories(${WIRINGPI_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${SHARED_LIB_INCLUDE_DIR})


set(SOURCE_FILES main.cpp App.cpp App.h)
# set(SOURCE_FILES main.cpp)

add_executable(screenshotUtil ${SOURCE_FILES})

target_link_libraries(screenshotUtil shared_lib)
target_link_libraries(screenshotUtil ${OpenCV_LIBS})
#target_link_libraries(screenshotUtil ${WIRINGPI_LIBRARIES})
#target_link_libraries(screenshotUtil ${CMAKE_THREAD_LIBS_INIT})
