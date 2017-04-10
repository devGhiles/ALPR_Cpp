//
// Created by ghiles on 4/7/17.
//

#ifndef ALPR_CPP_NOISE_H
#define ALPR_CPP_NOISE_H

#include <opencv2/opencv.hpp>
#include "../Utils/opencv.h"

using namespace std;
using namespace cv;

void remove_noise_from_v(Mat& v, float brightness_threshold, int long_line_threshold);
void remove_noise_from_h(Mat& h);
void get_lines_and_heights(Mat v, float brightness_threshold, vector<pair<int, int>>& lines, vector<int>& heights);
void remove_long_lines(Mat& v, vector<pair<int, int>>& lines, vector<int>& heights, int long_line_threshold);
void remove_vertical_line(Mat& img, int row, int col, int height);
void delete_lines(vector<pair<int, int>>& lines, vector<int>& heights, vector<int> lines_to_delete);
void remove_short_lines(Mat& v, vector<pair<int, int>>& lines, vector<int>& heights, double avg_height);

#endif //ALPR_CPP_NOISE_H
