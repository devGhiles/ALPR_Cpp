#include "location.h"

void localize_license_plate(Mat src, Mat &dst) {
    vector<Mat> candidate_plates;
    localize_license_plate(src, dst, candidate_plates);
}

void localize_license_plate(Mat src, Mat &dst, vector<Mat> &candidate_plates) {
    // Declare variables and initialize parameters
    int bs_prop = 10;
    int long_line_threshold = 20;
    int transitions_threshold = 7;
    int top_lines_to_check = 9;
    float brightness_threshold = 0.0f;
    double p = 0.3;
    int rough_width = 100;
    int rough_height = 9;

    localize_license_plate(src, dst, candidate_plates, bs_prop, brightness_threshold, long_line_threshold,
                           transitions_threshold, top_lines_to_check, p, rough_width, rough_height);
}

void localize_license_plate(Mat src, Mat &dst, int bs_prop, float brightness_threshold, int long_line_threshold,
                            int transitions_threshold, int top_lines_to_check, double p, int rough_width,
                            int rough_height) {
    vector<Mat> candidate_plates;
    localize_license_plate(src, dst, candidate_plates, bs_prop, brightness_threshold, long_line_threshold,
                           transitions_threshold, top_lines_to_check, p, rough_width, rough_height);
}

void localize_license_plate(Mat src, Mat &dst, vector<Mat> &candidate_plates, int bs_prop, float brightness_threshold,
                            int long_line_threshold, int transitions_threshold, int top_lines_to_check, double p,
                            int rough_width, int rough_height) {
    // Declare variables and initialize parameters
    Mat gray, h, v;

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
    get_candidate_plates(src, v, candidate_points, candidate_plates, rough_width, rough_height);

    // Correct candidate plates
    correct_plates(candidate_plates);

    // Choose a plate between candidates
    choose_plate(candidate_plates, dst);
}

void localize_license_plate(Mat src, Plaque &plaque) {
    // Declare variables and initialize parameters
    int bs_prop = 9;
    int long_line_threshold = 20;
    int transitions_threshold = 7;
    int top_lines_to_check = 9;
    float brightness_threshold = 0.0f;
    double p = 0.3;
    int rough_width = 90;
    int rough_height = 9;

    localize_license_plate(src, plaque, bs_prop, brightness_threshold, long_line_threshold, transitions_threshold,
                           top_lines_to_check, p, rough_width, rough_height);
}

void localize_license_plate(Mat src, Plaque &plaque, int bs_prop, float brightness_threshold, int long_line_threshold,
                            int transitions_threshold, int top_lines_to_check, double p, int rough_width,
                            int rough_height) {
    Mat gray, h, v;

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
    vector<Plaque> candidate_plates;
    get_candidate_plates(src, v, candidate_points, candidate_plates, rough_width, rough_height);

    // Correct candidate plates
    correct_plates(candidate_plates);

    // Choose a plate between candidates
    choose_plate(candidate_plates, plaque);
}