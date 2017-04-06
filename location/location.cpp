//
// Created by ghiles on 4/1/17.
//

#include "location.h"

void localize_license_plate(Mat src, Mat& dst) {
    // Declare variables
    Mat gray, h, v;

    // convert the image to grayscale
    convert_to_grayscale(src, gray);

    // DWT (Haar Discrete Wavelet Transform)
    dwt2(gray, h, v);

    dst = v;
}