//
// Created by ghiles on 4/14/17.
//

#ifndef ALPR_CPP_PROJECTIONS_H
#define ALPR_CPP_PROJECTIONS_H

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void row_projections(Mat binary_image, vector<int> &projections);

void col_projections(Mat binary_image, vector<int> &projections);

#endif //ALPR_CPP_PROJECTIONS_H
