//
// Created by ghiles on 4/8/17.
//

#ifndef ALPR_CPP_TRANSITIONS_H
#define ALPR_CPP_TRANSITIONS_H

#include <opencv2/opencv.hpp>
#include <set>

using namespace std;
using namespace cv;

void get_candidate_points(Mat v, Mat h, int bs_prop, int transitions_threshold, int top_lines_to_check, double p,
                          vector<pair<int, int>> &candidate_points);

int transitions_count(Mat row);

int max_contiguous(Mat row);

#endif //ALPR_CPP_TRANSITIONS_H
