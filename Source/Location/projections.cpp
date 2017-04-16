//
// Created by ghiles on 4/14/17.
//

#include "projections.h"

void row_projections(Mat binary_image, vector<int> &projections) {
    for (int row = 0; row < binary_image.rows; row++) {
        int count = 0;
        for (int col = 0; col < binary_image.cols; col++) {
            if (binary_image.at<int>(row, col) > 0) {
                count++;
            }
        }
        projections.push_back(count);
    }
}

void col_projections(Mat binary_image, vector<int> &projections) {
    for (int col = 0; col < binary_image.cols; col++) {
        int count = 0;
        for (int row = 0; row < binary_image.rows; row++) {
            if (binary_image.at<int>(row, col) > 0) {
                count++;
            }
        }
        projections.push_back(count);
    }
}