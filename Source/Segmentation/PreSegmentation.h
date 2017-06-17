//
// Created by Nabil on 24/12/2016.
//

#ifndef OPENCV_TEST_COMMON_H
#define OPENCV_TEST_COMMON_H

#include <opencv2/core/mat.hpp>

using namespace cv;

double compute_skew(Mat src);
Mat deskew(Mat img, double angle);
Mat drawContours(Mat in);
void deleteLines(Mat in);

#endif //OPENCV_TEST_COMMON_H
