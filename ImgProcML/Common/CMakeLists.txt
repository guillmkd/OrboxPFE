cmake_minimum_required(VERSION 3.2)

set(SHARED_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SHARED_LIB_INCLUDE_DIR ${SHARED_LIB_INCLUDE_DIR} PARENT_SCOPE)

if(TARGET shared_lib)
    message("shared_lib is already defined")
else()
    include_directories(${SHARED_LIB_INCLUDE_DIR})
    set(LIB_SRCS InputParser.cpp Rois.cpp Screenshots.cpp IdProvider.hpp Stat.hpp)
    add_library(shared_lib STATIC ${LIB_SRCS})
endif()
