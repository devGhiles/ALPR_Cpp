//
// Created by ghiles on 4/9/17.
//

#include "plate_chooser.h"
#include "../Utils/opencv.h"
#include "tests.h"
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