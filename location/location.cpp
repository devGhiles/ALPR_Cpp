//
// Created by ghiles on 4/1/17.
//

#include "location.h"

void localize_license_plate(Mat src, Mat &dst) {
    // Declare variables and initialize parameters
    Mat gray, h, v;
    float brightness_threshold = 0.0f;
    int long_line_threshold = 100;
    int bs_prop = 5;
    int transitions_threshold = 8;
    int top_lines_to_check = 5;
    double p = 0.5;

    // convert the image to grayscale
    convert_to_grayscale(src, gray);

    // DWT (Haar Discrete Wavelet Transform)
    dwt2(gray, h, v);

    // Denoising
    remove_noise_from_v(v, brightness_threshold, long_line_threshold);
    remove_noise_from_h(h);

    // Candidate points (from where we have enough transitions)
    vector<pair<int, int>> candidate_points;
    get_candidate_points(v, h, 5, 8, 5, 0.5, candidate_points);

    // Candidate plates (from candidate points)
    vector<Mat> candidate_plates;
    get_candidate_plates(src, v, candidate_points, candidate_plates);

    if (!candidate_plates.empty()) {
        dst = candidate_plates[0].clone();
    }
}