cmake_minimum_required(VERSION 3.6)
project(ALPR_Cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES main.cpp Source/Location/Location.cpp Source/Location/Location.h Source/Segmentation/PreSegmentation.cpp Source/Segmentation/PreSegmentation.h Source/Segmentation/SegmentationByContours.cpp Source/Segmentation/SegmentationByContours.h Source/OCR/OCR.cpp Source/OCR/OCR.h Source/Models/Plaque.h Source/Models/Plaque.cpp Source/Models/CharSegment.h Source/Models/CharSegment.cpp Source/ML/HOT.cpp Source/ML/HOT.h Source/ML/OCRTarbia.cpp Source/ML/OCRTarbia.h)
add_executable(ALPR_Cpp ${SOURCE_FILES})

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_objdetect opencv_ml)

target_link_libraries(ALPR_Cpp ${OpenCV_LIBS})