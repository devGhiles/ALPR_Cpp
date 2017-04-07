cmake_minimum_required(VERSION 3.6)
project(ALPR_Cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES main.cpp location/location.cpp location/location.h utils/opencv.cpp utils/opencv.h location/wavelet.cpp location/wavelet.h location/noise.cpp location/noise.h)
add_executable(ALPR_Cpp ${SOURCE_FILES})

target_link_libraries(ALPR_Cpp ${OpenCV_LIBS})