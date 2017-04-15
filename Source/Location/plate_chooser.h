//
// Created by ghiles on 4/9/17.
//

#ifndef ALPR_CPP_PLATE_CHOOSER_H
#define ALPR_CPP_PLATE_CHOOSER_H

#include <opencv2/opencv.hpp>
#include "svm.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

void choose_plate(vector<Mat> candidate_plates, Mat &chosen_one);

void choose_lowest(vector<Mat> candidate_plates, Mat &chosen_one);

void choose_highest_average_brightness(vector<Mat> candidate_plates, Mat &chosen_one);

void choose_highest_average_brightness_in_v(vector<Mat> candidate_plates, Mat &chosen_one);

void choose_using_svm(vector<Mat> candidate_plates, Mat &chosen_one);

#endif //ALPR_CPP_PLATE_CHOOSER_H
