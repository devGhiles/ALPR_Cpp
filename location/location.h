//
// Created by ghiles on 4/1/17.
//

#ifndef ALPR_CPP_LOCATION_H
#define ALPR_CPP_LOCATION_H

#include <opencv2/opencv.hpp>

using namespace cv;

void localize_license_plate(Mat src, Mat& dst);

#endif //ALPR_CPP_LOCATION_H
