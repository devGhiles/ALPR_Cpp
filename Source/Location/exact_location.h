//
// Created by ghiles on 4/8/17.
//

#ifndef ALPR_CPP_EXACT_LOCATION_H
#define ALPR_CPP_EXACT_LOCATION_H

#include <opencv2/opencv.hpp>
#include "../Utils/opencv.h"
#include "projections.h"
#include "wavelet.h"
#include "noise.h"

using namespace std;
using namespace cv;

void get_candidate_plates(Mat src, Mat v, vector<pair<int, int>> candidate_points, vector<Mat> &candidate_plates);

void rough_location(Mat img, int row, int col, int height, int ratio, int &start_row, int &end_row, int &start_col,
                    int &end_col);

void
save_candidate_plate(Mat src, vector<Mat> &candidate_plates, int start_row, int end_row, int start_col, int end_col);

void col_location(Mat v, int start_row, int end_row, int &start_col, int &end_col, int window_width_prop);

void row_location(Mat v, int &start_row, int &end_row, int start_col, int end_col);

double get_window_brightness_threshold(Mat v, int start_row, int end_row, int start_col, int end_col);

void col_location_by_projections(Mat src, int start_row, int end_row, int &start_col, int &end_col);

void col_correction_by_projections(Mat &plate);

void row_correction_by_projections(Mat &plate);

#endif //ALPR_CPP_EXACT_LOCATION_H
