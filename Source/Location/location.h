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
#include "../Models/Plaque.h"

using namespace cv;

void localize_license_plate(Mat src, Mat &dst);

void localize_license_plate(Mat src, Mat &dst, vector<Mat> &candidate_plates);

void localize_license_plate(Mat src, Mat &dst, int bs_prop, float brightness_threshold, int long_line_threshold,
                            int transitions_threshold, int top_lines_to_check, double p, int rough_width,
                            int rough_height);

void localize_license_plate(Mat src, Mat &dst, vector<Mat> &candidate_plates, int bs_prop, float brightness_threshold,
                            int long_line_threshold, int transitions_threshold, int top_lines_to_check, double p,
                            int rough_width, int rough_height);

void localize_license_plate(Mat src, Plaque &plaque);

void localize_license_plate(Mat src, Plaque &plaque, int bs_prop, float brightness_threshold, int long_line_threshold,
                            int transitions_threshold, int top_lines_to_check, double p, int rough_width,
                            int rough_height);

#endif //ALPR_CPP_LOCATION_H
