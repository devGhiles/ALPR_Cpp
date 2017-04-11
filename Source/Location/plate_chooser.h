//
// Created by ghiles on 4/9/17.
//

#ifndef ALPR_CPP_PLATE_CHOOSER_H
#define ALPR_CPP_PLATE_CHOOSER_H

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void choose_plate(vector<Mat> candidate_plates, Mat &chosen_one);

void choose_lowest(vector<Mat> candidate_plates, Mat &chosen_one);

void choose_highest_average_brightness(vector<Mat> candidate_plates, Mat &chosen_one);

void choose_highest_average_brightness_in_v(vector<Mat> candidate_plates, Mat &chosen_one);

void features_extraction(Mat plate, vector<double> &features, int n_cols, int n_rows);

void features_extraction(Mat plate, vector<double> &features);

double black_density(Mat binary_image);

#endif //ALPR_CPP_PLATE_CHOOSER_H
