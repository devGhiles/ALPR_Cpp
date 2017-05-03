//
// Created by ghiles on 4/16/17.
//

#include "correction.h"

void correct_plates(vector<Mat> &plates) {
    vector<Mat> corrected_plates;
    for (Mat plate : plates) {
        col_correction_by_projections_in_v(plate);
        row_correction_by_projections_in_v(plate);
        corrected_plates.push_back(plate.clone());
    }
    plates = corrected_plates;
}

void correct_plates(vector<Plaque> &plates) {
    vector<Plaque> corrected_plates;
    for (Plaque plate : plates) {
        col_correction_by_projections_in_v(plate);
        row_correction_by_projections_in_v(plate);
        corrected_plates.push_back(plate);
    }
    plates = corrected_plates;
}

void col_correction_by_projections_in_v(Mat &plate) {
    Mat h, v, gray;
    cvtColor(plate, gray, CV_BGR2GRAY);
    dwt2(gray, h, v);
    if ((v.rows == 0) || (v.cols == 0)) {
        return;
    }
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
    for (start_col = 0; start_col < v.cols - 1; start_col++) {
        if (projections[start_col] > stats::mean(projections, v.cols) * 1.1f) {
            break;
        }
    }
    start_col = min(start_col, v.cols - 1);

    // end_col
    int end_col;
    for (end_col = v.cols - 1; end_col > start_col; end_col--) {
        if (projections[end_col] > stats::mean(projections, v.cols) * 1.6f) {
            break;
        }
    }
    end_col = max(end_col, start_col + 1);

    // get the sub-image from the plate
    start_col *= 2;
    end_col = min(2 * end_col, plate.cols - 1);
    subimg(plate, plate, 0, plate.rows - 1, start_col, end_col);
}

void col_correction_by_projections_in_v(Plaque &plate) {
    Mat h, v, gray;
    cvtColor(plate.plateImg, gray, CV_BGR2GRAY);
    dwt2(gray, h, v);
    if ((v.rows == 0) || (v.cols == 0)) {
        return;
    }
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
    for (start_col = 0; start_col < v.cols - 1; start_col++) {
        if (projections[start_col] > stats::mean(projections, v.cols) * 1.1f) {
            break;
        }
    }
    start_col = min(start_col, v.cols - 1);

    // end_col
    int end_col;
    for (end_col = v.cols - 1; end_col > start_col; end_col--) {
        if (projections[end_col] > stats::mean(projections, v.cols) * 1.6f) {
            break;
        }
    }
    end_col = max(end_col, start_col + 1);

    // get the sub-image from the plate
    start_col *= 2;
    end_col = min(2 * end_col, plate.plateImg.cols - 1);
    subimg(plate.plateImg, plate.plateImg, 0, plate.plateImg.rows - 1, start_col, end_col);
    plate.position.x += start_col;
    plate.position.width = plate.plateImg.cols;
}

void row_correction_by_projections_in_v(Mat &plate) {
    Mat h, v, gray;
    cvtColor(plate, gray, CV_BGR2GRAY);
    dwt2(gray, h, v);
    if ((v.rows == 0) || (v.cols == 0)) {
        return;
    }
    remove_noise_from_v(v, 0, 100);

    // rows projections
    float projections[v.rows];
    for (int row = 0; row < v.rows; row++) {
        float sum = 0.0f;
        for (int col = 0; col < v.cols; col++) {
            sum += v.at<float>(row, col);
        }
        projections[row] = sum;
    }

    // start_row
    int start_row;
    for (start_row = 0; start_row < v.rows; start_row++) {
        if (projections[start_row] > stats::mean(projections, v.rows) * 0.3f) {
            break;
        }
    }
    start_row = min(start_row, v.rows - 1);

    // end_row
    int end_row;
    for (end_row = v.rows - 1; end_row > start_row; end_row--) {
        if (projections[end_row] > stats::mean(projections, v.rows) * 0.3f) {
            break;
        }
    }
    end_row = max(end_row, start_row + 1);

    // get the sub-image from the plate
    start_row *= 2;
    end_row = min(2 * end_row, plate.rows - 1);
    subimg(plate, plate, start_row, end_row, 0, plate.cols - 1);
}

void row_correction_by_projections_in_v(Plaque &plaque) {
    Mat h, v, gray;
    cvtColor(plaque.plateImg, gray, CV_BGR2GRAY);
    dwt2(gray, h, v);
    if ((v.rows == 0) || (v.cols == 0)) {
        return;
    }
    remove_noise_from_v(v, 0, 100);

    // rows projections
    float projections[v.rows];
    for (int row = 0; row < v.rows; row++) {
        float sum = 0.0f;
        for (int col = 0; col < v.cols; col++) {
            sum += v.at<float>(row, col);
        }
        projections[row] = sum;
    }

    // start_row
    int start_row;
    for (start_row = 0; start_row < v.rows; start_row++) {
        if (projections[start_row] > stats::mean(projections, v.rows) * 0.3f) {
            break;
        }
    }
    start_row = min(start_row, v.rows - 1);

    // end_row
    int end_row;
    for (end_row = v.rows - 1; end_row > start_row; end_row--) {
        if (projections[end_row] > stats::mean(projections, v.rows) * 0.3f) {
            break;
        }
    }
    end_row = max(end_row, start_row + 1);

    // get the sub-image from the plate
    start_row *= 2;
    end_row = min(2 * end_row, plaque.plateImg.rows - 1);
    subimg(plaque.plateImg, plaque.plateImg, start_row, end_row, 0, plaque.plateImg.cols - 1);
    plaque.position.y += start_row;
    plaque.position.height = plaque.plateImg.rows;
}
