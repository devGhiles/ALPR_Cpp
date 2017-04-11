//
// Created by ghiles on 4/9/17.
//

#include "plate_chooser.h"
#include "../Utils/opencv.h"
#include "wavelet.h"

void choose_plate(vector<Mat> candidate_plates, Mat &chosen_one) {
    if (candidate_plates.empty()) {
        chosen_one = Mat::zeros(80, 20, CV_8UC3);
    } else {
        choose_lowest(candidate_plates, chosen_one);
    }
}

void choose_lowest(vector<Mat> candidate_plates, Mat &chosen_one) {
    chosen_one = candidate_plates[0].clone();
}

double average_brightness(Mat m) {
    int count = 0;
    for (int row = 0; row < m.rows; row++) {
        for (int col = 0; col < m.cols; col++) {
            if (m.at<int>(row, col) > 0) {
                count += 1;
            }
        }
    }
    return count / (m.rows * m.cols);
}

void choose_highest_average_brightness(vector<Mat> candidate_plates, Mat &chosen_one) {
    double max_average_brightness = -1.0;

    for (Mat plate : candidate_plates) {
        Mat binary;
        Mat gray;
        cvtColor(plate, gray, CV_BGR2GRAY);
        threshold(gray, binary, 100, 255, CV_THRESH_BINARY);
        double avg = average_brightness(binary);
        if (avg > max_average_brightness) {
            cout << avg << endl;
            max_average_brightness = avg;
            chosen_one = plate.clone();
        }
    }
}

void choose_highest_average_brightness_in_v(vector<Mat> candidate_plates, Mat &chosen_one) {
    Mat h, v;
    double max_avg = -1.0;
    int chosen_index = -1;

    for (uint i = 0; i < candidate_plates.size(); i++) {
        Mat plate = candidate_plates[i];
        Mat gray;
        cvtColor(plate, gray, CV_BGR2GRAY);
        dwt2(gray, h, v);
        double current_avg = average(v);
        if (current_avg > max_avg) {
            max_avg = current_avg;
            chosen_index = i;
        }
    }

    chosen_one = candidate_plates[chosen_index].clone();
}

void features_extraction(Mat plate, vector<double> &features, int n_cols, int n_rows) {
    // variables
    int W = plate.cols;
    int H = plate.rows;
    int w = (int) round(W / n_cols);
    int h = (int) round(H / n_rows);

    // convert to binary (OTSU)
    Mat gray, binary;
    cvtColor(plate, gray, CV_BGR2GRAY);
    threshold(gray, binary, 100, 255, CV_THRESH_OTSU);

    // the real thing
    for (int p_row = 0; p_row < n_rows; p_row++) {
        int start_row = p_row * h;
        int end_row = min(start_row + w - 1, plate.rows - 1);
        for (int p_col = 0; p_col < n_cols; p_col++) {
            int start_col = p_col * w;
            int end_col = min(start_col + h - 1, plate.cols - 1);

            Mat cell;
            subimg(binary, cell, start_row, end_row, start_col, end_col);
            features.push_back(black_density(cell));
        }
    }
}

void features_extraction(Mat plate, vector<double> &features) {
    int n_cols = 10;
    int n_rows = 5;
    features_extraction(plate, features, n_cols, n_rows);
}

double black_density(Mat binary_image) {
    int count = 0;
    for (int row = 0; row < binary_image.rows; row++) {
        for (int col = 0; col < binary_image.cols; col++) {
            if (binary_image.at<int>(row, col) == 0) {
                count++;
            }
        }
    }
    return ((double) count) / (binary_image.rows * binary_image.cols);
}