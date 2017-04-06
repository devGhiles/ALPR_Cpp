//
// Created by ghiles on 4/1/17.
//

#include "location.h"

void localize_license_plate(Mat src, Mat& dst) {
    // convert the image to grayscale
    Mat gray;
    convert_to_grayscale(src, gray);

    dst = gray;
}