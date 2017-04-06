//
// Created by ghiles on 4/6/17.
//

#include "opencv.h"

void convert_to_grayscale(Mat src, Mat& dst) {
    if (src.type() != CV_8UC1) {
        cvtColor(src, dst, CV_BGR2GRAY);
    } else {
        dst = src;
    }
}