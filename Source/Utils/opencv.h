//
// Created by ghiles on 4/6/17.
//

#ifndef ALPR_CPP_OPENCV_H
#define ALPR_CPP_OPENCV_H

#include <opencv2/opencv.hpp>
#include "../Location/wavelet.h"

using namespace std;
using namespace cv;

void show(Mat img);

void show(Mat img, string window_name);

void show_dwt(Mat dwt_component, string window_name);

void show_binary(Mat img, string window_name);

void show_binary(Mat img);

void show_v(Mat img);

void show_v(Mat img, string window_name);

void convert_to_grayscale(Mat src, Mat &dst);

float average_of_positives(Mat &img);

void subimg(Mat src, Mat &dst, int start_row, int end_row, int start_col, int end_col);

double average_mat_float(Mat m);

double average_submat(Mat m, int start_row, int end_row, int start_col, int end_col);

double average(Mat m);

float black_density(Mat binary_image);

void convert_vector_to_mat(vector<vector<float>> v, Mat &m);

#endif //ALPR_CPP_OPENCV_H
