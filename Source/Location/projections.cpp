//
// Created by ghiles on 4/14/17.
//

#include "projections.h"

void row_projections(Mat img, vector<int> &projections) {
    for (int row = 0; row < img.rows; row++) {
        int sum = 0;
        for (int col = 0; col < img.cols; col++) {
            sum += img.at<uchar>(row, col);
        }
        projections.push_back(sum);
    }
}

void row_projections(Mat img, vector<float> &projections) {
    for (int row = 0; row < img.rows; row++) {
        float sum = 0.0f;
        for (int col = 0; col < img.cols; col++) {
            sum += img.at<float>(row, col);
        }
        projections.push_back(sum);
    }
}

void col_projections(Mat img, vector<int> &projections) {
    for (int col = 0; col < img.cols; col++) {
        int sum = 0;
        for (int row = 0; row < img.rows; row++) {
            sum += img.at<int>(row, col);
        }
        projections.push_back(sum);
    }
}

void col_projections(Mat img, vector<float> &projections) {
    for (int col = 0; col < img.cols; col++) {
        float sum = 0.0f;
        for (int row = 0; row < img.rows; row++) {
            sum += img.at<float>(row, col);
        }
        projections.push_back(sum);
    }
}

void show_projections(vector<int> projections) {
    if (projections.empty()) {
        return;
    }

    int M = projections[0];
}