//
// Created by ghiles on 4/6/17.
//

#ifndef ALPR_CPP_OPENCV_H
#define ALPR_CPP_OPENCV_H

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void show(Mat img, string window_name);

void show_dwt(Mat dwt_component, string window_name);

void convert_to_grayscale(Mat src, Mat &dst);

float average_of_positives(Mat &img);

void subimg(Mat src, Mat &dst, int start_row, int end_row, int start_col, int end_col);

double average_mat_float(Mat m);

double average_submat_float(Mat m, int start_row, int end_row, int start_col, int end_col);

#endif //ALPR_CPP_OPENCV_H
