//
// Created by ghiles on 4/16/17.
//

#include "correction.h"

void correct_plates(vector<Mat> &plates) {
    vector<Mat> corrected_plates;
    for (Mat plate : plates) {
        col_correction_by_projections_in_v(plate);
        corrected_plates.push_back(plate.clone());
    }
    plates = corrected_plates;
}

void col_correction_by_projections_in_v(Mat &plate) {
    Mat h, v, gray;
    cvtColor(plate, gray, CV_BGR2GRAY);
    dwt2(gray, h, v);
    remove_noise_from_v(v, 0, 100);

    // col projections
    float projections[v.cols];
    for (int col = 0; col < v.cols; col++) {
        float sum = 0.0f;
        for (int row = 0; row < v.rows; row++) {
            sum += v.at<float>(row, col);
        }
        projections[col] = sum;
    }

    // start_col
    int start_col;
    for (start_col = 0; start_col < v.cols; start_col++) {
        if (projections[start_col] > 40) {
            break;
        }
    }
    start_col = min(start_col, v.cols - 1);

    // end_col
    int end_col;
    for (end_col = v.cols - 1; end_col > start_col; end_col--) {
        if (projections[end_col] > 40) {
            break;
        }
    }
    end_col = max(end_col, start_col + 1);

    // get the sub-image from the plate
    start_col *= 2;
    end_col = min(2 * end_col, plate.cols - 1);
    subimg(plate, plate, 0, plate.rows - 1, start_col, end_col);
}