//
// Created by ghiles on 4/6/17.
//

#ifndef ALPR_CPP_WAVELET_H
#define ALPR_CPP_WAVELET_H

#include "opencv2/opencv.hpp"

using namespace cv;

void dwt2(Mat src, Mat& c, Mat& h, Mat& v, Mat& d);
void dwt2(Mat src, Mat& h, Mat& v);

#endif //ALPR_CPP_WAVELET_H
