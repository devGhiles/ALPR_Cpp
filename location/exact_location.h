//
// Created by ghiles on 4/8/17.
//

#ifndef ALPR_CPP_EXACT_LOCATION_H
#define ALPR_CPP_EXACT_LOCATION_H

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void get_candidate_plates(Mat src, Mat v, vector<pair<int, int>> candidate_points, vector<Mat> &candidate_plates);

#endif //ALPR_CPP_EXACT_LOCATION_H
