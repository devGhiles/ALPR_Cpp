//
// Created by ghiles on 4/9/17.
//

#include "plate_chooser.h"

void choose_plate(vector<Mat> candidate_plates, Mat &chosen_one) {
    if (candidate_plates.empty()) {
        chosen_one = Mat::zeros(80, 20, CV_8UC3);
    } else {
        chosen_one = candidate_plates[0].clone();
    }
}