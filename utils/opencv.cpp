//
// Created by ghiles on 4/6/17.
//

#include "opencv.h"

void convert_to_grayscale(Mat src, Mat& dst) {
    if (src.type() != CV_8UC1) {
        cvtColor(src, dst, CV_BGR2GRAY);
    } else {
        dst = src;
    }
}

/**
 *
 * @param img a Mat object whose elements are float
 * @return the average (arithmetic mean) of the strictly positive elements of img
 */
float average_of_positives(Mat& img) {
    float sum = 0.0f;
    int elements_count = 0;

    for (int row = 0; row < img.rows; row++) {
        for (int col = 0; col < img.cols; col++) {
            if (img.at<float>(row, col) > 0) {
                sum += img.at<float>(row, col);
                elements_count++;
            }
        }
    }

    return sum / elements_count;
}