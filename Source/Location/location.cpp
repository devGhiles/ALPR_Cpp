//
// Created by ghiles on 4/1/17.
//

#include "location.h"

void localize_license_plate(Mat src, Mat &dst) {
    vector<Mat> candidate_plates;
    localize_license_plate(src, dst, candidate_plates);
}

void localize_license_plate(Mat src, Mat &dst, vector<Mat> &candidate_plates) {
    // Declare variables and initialize parameters
    Mat gray, h, v;
    float brightness_threshold = 0.0f;
    int long_line_threshold = 100;
    int bs_prop = 5;
    int transitions_threshold = 8;
    int top_lines_to_check = 10;
    double p = 0.2;

    // convert the image to grayscale
    convert_to_grayscale(src, gray);

    // DWT (Haar Discrete Wavelet Transform)
    dwt2(gray, h, v);

    // Denoising
    remove_noise_from_v(v, brightness_threshold, long_line_threshold);
    remove_noise_from_h(h);

    // Candidate points (from where we have enough transitions)
    vector<pair<int, int>> candidate_points;
    get_candidate_points(v, h, bs_prop, transitions_threshold, top_lines_to_check, p, candidate_points);

    // Candidate plates (from candidate points)
    get_candidate_plates(src, v, candidate_points, candidate_plates);

    // Correct candidate plates
    correct_plates(candidate_plates);

    // Choose a plate between candidates
    choose_plate(candidate_plates, dst);
}

void localize_license_plate(Mat src, Plaque &plaque) {
    // Declare variables and initialize parameters
    Mat gray, h, v;
    float brightness_threshold = 0.0f;
    int long_line_threshold = 100;
    int bs_prop = 5;
    int transitions_threshold = 8;
    int top_lines_to_check = 10;
    double p = 0.2;

    // convert the image to grayscale
    convert_to_grayscale(src, gray);

    // DWT (Haar Discrete Wavelet Transform)
    dwt2(gray, h, v);

    // Denoising
    remove_noise_from_v(v, brightness_threshold, long_line_threshold);
    remove_noise_from_h(h);

    // Candidate points (from where we have enough transitions)
    vector<pair<int, int>> candidate_points;
    get_candidate_points(v, h, bs_prop, transitions_threshold, top_lines_to_check, p, candidate_points);

    // Candidate plates (from candidate points)
    vector<Mat> candidate_plates;
    get_candidate_plates(src, v, candidate_points, candidate_plates);

    // Correct candidate plates
    correct_plates(candidate_plates);

    // Choose a plate between candidates
    Mat chosen_plate;
    int chosen_plate_index = choose_plate(candidate_plates, chosen_plate);

    // return the plate
    pair<int, int> chosen_point = candidate_points[chosen_plate_index];
    plaque = Plaque(chosen_plate,
                    Rect(2 * chosen_point.second, 2 * chosen_point.first, chosen_plate.cols, chosen_plate.rows));
}