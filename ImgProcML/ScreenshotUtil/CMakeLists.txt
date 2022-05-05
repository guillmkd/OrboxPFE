cmake_minimum_required(VERSION 3.2)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project(ScreenshotUtil)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
add_subdirectory(../Common shared_lib)

include_directories(${SHARED_LIB_INCLUDE_DIR})
include_directories(${pigpio_INCLUDE_DIR})

set(SOURCE_FILES main.cpp App.cpp App.h)

add_executable(screenshotUtil ${SOURCE_FILES})

target_link_libraries(screenshotUtil shared_lib)
target_link_libraries(screenshotUtil ${OpenCV_LIBS})
target_link_libraries(screenshotUtil ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(screenshotUtil ${pigpio_LIBRARY})
