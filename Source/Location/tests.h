//
// Created by ghiles on 4/9/17.
//

#ifndef ALPR_CPP_TESTS_H
#define ALPR_CPP_TESTS_H

#include <opencv2/opencv.hpp>
#include <time.h>
#include "../Utils/opencv.h"
#include "svm.h"
#include "projections.h"
#include "../OCR/OCR.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

void test_candidate_points(Mat src, vector<pair<int, int>> candidate_points);

void test_candidate_plates(vector<Mat> candidate_plates);

void svm_generate_plates_database();

void main_location();

void test_black_density();

void test_folder(string test_folder, string target_folder, int num_images);

float test_location();

float test_location(int bs_prop, float brightness_threshold, int long_line_threshold, int transitions_threshold,
                    int top_lines_to_check, double p, int rough_width, int rough_height, int resize_coeff);

void test_recognition();

void read_plate_numbers(string filename, vector<string> &numbers);

void test_system();

void find_localization_parameters();

#endif //ALPR_CPP_TESTS_H
