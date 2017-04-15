//
// Created by ghiles on 4/8/17.
//

#include "exact_location.h"

void get_candidate_plates(Mat src, Mat v, vector<pair<int, int>> candidate_points, vector<Mat> &candidate_plates) {
    int height = 10, ratio = 10, window_width_prop = 3;

    for (pair<int, int> point : candidate_points) {
        int row = point.first;
        int col = point.second;
        int start_row, end_row, start_col, end_col;

        rough_location(src, row, col, height, ratio, start_row, end_row, start_col, end_col);
//        col_location(v, start_row, end_row, start_col, end_col, window_width_prop);
        row_location(v, start_row, end_row, start_col, end_col);
//        col_location_by_projections(src, start_row, end_row, start_col, end_col);

        // save the candidate plate
        save_candidate_plate(src, candidate_plates, 2 * start_row, 2 * end_row, 2 * start_col, 2 * end_col);
    }
}

void rough_location(Mat img, int row, int col, int height, int ratio, int &start_row, int &end_row, int &start_col,
                    int &end_col) {
    start_row = row;
    end_row = row + height - 1;
    if (end_row >= (img.rows - 1) / 2) {
        end_row = ((img.rows - 1) / 2) - 1;
    }

    start_col = col;
    end_col = start_col + ((end_row - start_row) * ratio);
    if (end_col >= (img.cols - 1) / 2) {
        end_col = ((img.cols - 1) / 2) - 1;
    }
}

void
save_candidate_plate(Mat src, vector<Mat> &candidate_plates, int start_row, int end_row, int start_col, int end_col) {
    Mat lp;
    subimg(src, lp, start_row, end_row, start_col, end_col);
    candidate_plates.push_back(lp);
}

void col_location(Mat v, int start_row, int end_row, int &start_col, int &end_col, int window_width_prop) {
    double window_brightness_threshold = get_window_brightness_threshold(v, start_row, end_row, start_col, end_col);
    int window_width = (end_col - start_col + 1) / window_width_prop;
    int window_start_row = start_row, window_end_row = end_row;

    // Left hand side location
    double max_avg = -1.0;
    int final_start_col = start_col;
    for (int window_start_col = start_col; window_start_col < start_col + window_width; window_start_col++) {
        int window_end_col = window_start_col + window_width;
        double current_avg = average_submat(v, window_start_row, window_end_row, window_start_col,
                                            window_end_col);
        if (current_avg > window_brightness_threshold) {
            final_start_col = window_start_col;
            max_avg = current_avg;
            break;
        }
    }

    // Right hand side location
    max_avg = -1.0;
    int final_end_col = end_col;
    for (int window_end_col = end_col; window_end_col > end_col - window_width; window_end_col--) {
        int window_start_col = window_end_col - window_width;
        double current_avg = average_submat(v, window_start_row, window_end_row, window_start_col,
                                            window_end_col);
        if (current_avg > window_brightness_threshold) {
            final_end_col = window_end_col;
            max_avg = current_avg;
            break;
        }
    }

    // save the results
    start_col = max(0, final_start_col);
    end_col = min(v.cols - 1, final_end_col);
}

void row_location(Mat v, int &start_row, int &end_row, int start_col, int end_col) {
    int row_top, row_bottom;

    for (row_top = start_row; row_top >= 0; row_top--) {
        Mat line;
        subimg(v, line, row_top, row_top, start_col, end_col);
        if (average(line) <= 2) {  // TODO: something else than <= 2
            break;
        }
    }
    row_top = max(row_top, 0);

    for (row_bottom = end_row; row_bottom < v.rows; row_bottom++) {
        Mat line;
        subimg(v, line, row_bottom, row_bottom, start_col, end_col);
        if (average(line) <= 2) {  // TODO: something else than <= 2
            break;
        }
    }
    row_bottom = min(row_bottom, v.rows - 1);

    start_row = row_top;
    end_row = row_bottom;
}

double get_window_brightness_threshold(Mat v, int start_row, int end_row, int start_col, int end_col) {
    Mat line;
    subimg(v, line, start_row, end_row, start_col, end_col);
    return average_mat_float(line) * 0.5;
}

void col_location_by_projections(Mat src, int start_row, int end_row, int &start_col, int &end_col) {
    Mat gray, binary;  // TODO: We assume white chars on black background, change this if needed
    Mat plate;
    subimg(src, plate, 2 * start_row, 2 * end_row, 2 * start_col, 2 * end_col);
    cvtColor(plate, gray, CV_BGR2GRAY);
    threshold(gray, binary, 100, 255, CV_THRESH_OTSU);
    vector<int> projections;
    col_projections(binary, projections);

    int col;

    for (col = 0; col < projections.size(); col++) {
        if (projections[col] > 0) {
            break;
        }
    }
    start_col += (col - 1) / 2;

    int length = (int) projections.size();
    for (col = 0; col < length; col++) {
        if (projections[length - 1 - col] > 0) {
            break;
        }
    }
    end_col -= (col - 1) / 2;
}

void col_correction_by_projections(Mat &plate) {
    Mat gray, binary;  // TODO: We assume white chars on black background, change this if needed
    cvtColor(plate, gray, CV_BGR2GRAY);
    threshold(gray, binary, 100, 255, CV_THRESH_OTSU);
    vector<int> projections;
    col_projections(binary, projections);

    int col;

    for (col = 0; col < projections.size(); col++) {
        if (projections[col] > 0) {
            break;
        }
    }
    int start_col = col;

    for (col = plate.cols - 1; col >= start_col; col--) {
        if (projections[col] > 0) {
            break;
        }
    }
    int end_col = col;

    Mat tmp_plate;
    subimg(plate, tmp_plate, 0, plate.rows - 1, start_col, end_col);
    plate = tmp_plate.clone();
}