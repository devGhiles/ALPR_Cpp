//
// Created by ghiles on 4/1/17.
//

#include "location.h"

void localize_license_plate(Mat src, Mat& dst) {
    // Declare variables and initialize parameters
    Mat gray, h, v;
    float brightness_threshold = 0.0f;
    int long_line_threshold = 100;

    // convert the image to grayscale
    convert_to_grayscale(src, gray);

    // DWT (Haar Discrete Wavelet Transform)
    dwt2(gray, h, v);

    // Denoising
    remove_noise_from_v(v, brightness_threshold, long_line_threshold);

    dst = v;
}