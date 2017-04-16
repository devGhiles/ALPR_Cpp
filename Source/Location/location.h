//
// Created by ghiles on 4/1/17.
//

#ifndef ALPR_CPP_LOCATION_H
#define ALPR_CPP_LOCATION_H

#include <opencv2/opencv.hpp>
#include "wavelet.h"
#include "noise.h"
#include "transitions.h"
#include "exact_location.h"
#include "plate_chooser.h"
#include "tests.h"
#include "correction.h"
#include "../Utils/opencv.h"

using namespace cv;

void localize_license_plate(Mat src, Mat& dst);

void localize_license_plate(Mat src, Mat &dst, vector<Mat> &candidate_plates);

#endif //ALPR_CPP_LOCATION_H
