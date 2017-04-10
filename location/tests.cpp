//
// Created by ghiles on 4/9/17.
//

#include "tests.h"

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