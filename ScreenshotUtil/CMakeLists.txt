cmake_minimum_required(VERSION 3.5)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project(ScreenshotUtil)

find_package(OpenCV REQUIRED)
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED)

include_directories(${WIRINGPI_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp App.cpp App.h ../Common/InputParser.h ../Common/Timer.h ../Common/Logger.h)

add_executable(screenshotutil ${SOURCE_FILES})

target_link_libraries(screenshotutil ${OpenCV_LIBS})
target_link_libraries(screenshotutil ${WIRINGPI_LIBRARIES})
target_link_libraries(screenshotutil ${CMAKE_THREAD_LIBS_INIT})
