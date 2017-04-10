//
// Created by ghiles on 4/9/17.
//

#ifndef ALPR_CPP_TESTS_H
#define ALPR_CPP_TESTS_H

#include <opencv2/opencv.hpp>
#include "../Utils/opencv.h"

using namespace std;
using namespace cv;

void test_candidate_points(Mat src, vector<pair<int, int>> candidate_points);

void test_candidate_plates(vector<Mat> candidate_plates);

#endif //ALPR_CPP_TESTS_H
