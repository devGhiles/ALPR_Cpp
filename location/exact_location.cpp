//
// Created by ghiles on 4/8/17.
//

#include "exact_location.h"
#include "tests.h"

void get_candidate_plates(Mat src, Mat v, vector<pair<int, int>> candidate_points, vector<Mat> &candidate_plates) {
    int height = 10, ratio = 10, window_width_prop = 3;

    for (pair<int, int> point : candidate_points) {
        int row = point.first;
        int col = point.second;
        int start_row, end_row, start_col, end_col;

        rough_location(src, row, col, height, ratio, start_row, end_row, start_col, end_col);
//        col_location(v, start_row, end_row, start_col, end_col, window_width_prop);
        row_location(v, start_row, end_row, start_col, end_col);

        // save the candidate plate
        save_candidate_plate(src, candidate_plates, start_row, end_row, start_col, end_col);
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
    subimg(src, lp, 2 * start_row, 2 * end_row, 2 * start_col, 2 * end_col);
    candidate_plates.push_back(lp);
}

void col_location(Mat v, int start_row, int end_row, int &start_col, int &end_col, int window_width_prop) {
    double window_brightness_threshold = get_window_brightness_threshold(v, start_row, end_row, start_col, end_col);
    int window_width = (end_col - start_col) / window_width_prop;
    int window_start_row = start_row, window_end_row = end_row;

    // Left hand side location
    double max_avg = -1.0;
    int final_start_col = -1;
    for (int window_start_col = start_col; window_start_col < start_col + window_width; window_start_col++) {
        int window_end_col = window_start_col + window_width;
        double current_avg = average_submat_float(v, window_start_row, window_end_row, window_start_col,
                                                  window_end_col);
        if (current_avg > window_brightness_threshold) {
            final_start_col = window_start_col;
            max_avg = current_avg;
            break;
        }
    }

    // Right hand side location
    max_avg = -1.0;
    int final_end_col = -1;
    for (int window_end_col = end_col; window_end_col > end_col - window_width; window_end_col--) {
        int window_start_col = window_end_col - window_width;
        double current_avg = average_submat_float(v, window_start_row, window_end_row, window_start_row,
                                                  window_end_col);
        if (current_avg > window_brightness_threshold) {
            final_end_col = window_end_col;
            max_avg = current_avg;
            break;
        }
    }

    // get results
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