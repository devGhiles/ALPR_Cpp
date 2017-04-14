cmake_minimum_required(VERSION 3.6)
project(ALPR_Cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES main.cpp Source/Location/location.cpp Source/Location/location.h Source/Utils/opencv.cpp Source/Utils/opencv.h Source/Location/wavelet.cpp Source/Location/wavelet.h Source/Location/noise.cpp Source/Location/noise.h Source/Location/transitions.cpp Source/Location/transitions.h Source/Location/exact_location.cpp Source/Location/exact_location.h Source/Location/plate_chooser.cpp Source/Location/plate_chooser.h Source/Location/tests.cpp Source/Location/tests.h Source/Segmentation/PreSegmentation.cpp Source/Segmentation/PreSegmentation.h Source/Segmentation/SegmentationByContours.cpp Source/Segmentation/SegmentationByContours.h Source/OCR/OCR.cpp Source/OCR/OCR.h Source/Models/Plaque.h Source/Models/Plaque.cpp Source/Models/CharSegment.h Source/Models/CharSegment.cpp Source/ML/HOT.cpp Source/ML/HOT.h Source/ML/OCRTarbia.cpp Source/ML/OCRTarbia.h Source/Location/svm.cpp Source/Location/svm.h Source/Location/projections.cpp Source/Location/projections.h)
add_executable(ALPR_Cpp ${SOURCE_FILES})

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_objdetect opencv_ml)

target_link_libraries(ALPR_Cpp ${OpenCV_LIBS})