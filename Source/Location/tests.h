//
// Created by ghiles on 4/9/17.
//

#ifndef ALPR_CPP_TESTS_H
#define ALPR_CPP_TESTS_H

#include <opencv2/opencv.hpp>
#include "../Utils/opencv.h"
#include "svm.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

void test_candidate_points(Mat src, vector<pair<int, int>> candidate_points);

void test_candidate_plates(vector<Mat> candidate_plates);

void svm_generate_plates_database();

void main_location();

void test_black_density();

void test_folder(string test_folder, string target_folder, int num_images);

void test_location();

#endif //ALPR_CPP_TESTS_H
