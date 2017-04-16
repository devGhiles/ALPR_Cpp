//
// Created by ghiles on 4/9/17.
//

#include "tests.h"
#include "location.h"
#include "wavelet.h"

void test_candidate_points(Mat src, vector<pair<int, int>> candidate_points) {
    for (pair<int, int> candidate : candidate_points) {
        int start_row = candidate.first * 2;
        int start_col = candidate.second * 2;
        int height = 40;
        int width = 5 * height;
        int end_row = start_row + height - 1;
        int end_col = start_col + width - 1;
        Mat src_copy = src.clone();
        rectangle(src_copy, Point(start_col, start_row), Point(end_col, end_row), Scalar(0, 255, 0));
        show(src_copy, "Candidate Points Test");
    }
}

void test_candidate_plates(vector<Mat> candidate_plates) {
    for (Mat plate : candidate_plates) {
        show(plate, "Candidate Plates Test");
    }
}

void svm_generate_plates_database() {
    int candidates_count = 0;
    for (int i = 1; i <= 810; i++) {
        Mat img = imread("images/G1/G1 (" + to_string(i) + ").jpg");
        Mat plate;
        vector<Mat> candidates;
        localize_license_plate(img, plate, candidates);
        for (Mat lp : candidates) {
            candidates_count++;
            imwrite("images/candidates/" + to_string(candidates_count) + ".jpg", lp);
        }
        if (candidates_count == 810) {
            break;
        }
    }
}

void main_location() {
    Mat img = imread("images/G1/G1 (1).jpg");
    Mat plate;
    localize_license_plate(img, plate);
    show(plate);
}

void test_black_density() {
    Mat plate = imread("images/plates/G1/1.jpg");
    int n_cols = 16;
    int n_rows = 4;
    vector<float> features;
    features_extraction(plate, features, n_cols, n_rows);
    for (float f : features) {
        cout << f << endl;
    }
}

void test_folder(string test_folder, string target_folder, int num_images) {
    VideoCapture images;
    if (!images.open(test_folder)) {
        cout << "Can not open the folder images" << endl;
        return;
    }

    Mat frame;
    int img_index = 0;
    while (images.read(frame) && (img_index < num_images)) {
        img_index++;
        Mat plate;
        localize_license_plate(frame, plate);
        imwrite(target_folder + to_string(img_index) + ".jpg", plate);
    }
}