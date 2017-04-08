//
// Created by ghiles on 4/8/17.
//

#include "transitions.h"

void get_candidate_points(Mat v, Mat h, int bs_prop, int transitions_threshold, int top_lines_to_check, double p,
                          vector<pair<int, int>> &candidate_points) {
    int bs = v.cols / bs_prop;
    set<pair<int, int>> tmp_candidate_points;  // we use a set temporarily in order to remove the duplicates

    for (int row = v.rows - 1; row > top_lines_to_check; row--) {
        int max_transitions = 0, col_max = 0;

        for (int col = 0; col < v.cols; col++) {
            if (v.at<float>(row, col) > 0) {
                int end_col = min(col + bs, v.cols);
                int current_transitions = transitions_count(v.row(row).colRange(col, end_col));
                if (current_transitions > max_transitions) {
                    max_transitions = current_transitions;
                    col_max = col;
                }
            }
        }

        // If there are enough transactions, check if there is a horizontal line on the LH sub-band
        if (max_transitions >= transitions_threshold) {
            for (int top_row = row - top_lines_to_check; top_row < row; top_row++) {
                int end_col_max = min(col_max + bs, v.cols);
                int line_max = max_contiguous(h.row(top_row).colRange(col_max, end_col_max));

                if (line_max >= p * bs) {
                    tmp_candidate_points.insert(pair<int, int>(top_row, col_max));
                    break;
                }
            }
        }
    }

    // Return a list of the candidate points sorted from bottom to top
    copy(tmp_candidate_points.begin(), tmp_candidate_points.end(), back_inserter(candidate_points));
    sort(candidate_points.begin(), candidate_points.end());
    reverse(candidate_points.begin(), candidate_points.end());
}

int transitions_count(Mat row) {
    int start = 0, end = row.cols, count = 0;
    float brightness_threshold = 10;

    for (int i = start; i < end - 1; i++) {
        // TODO: change this to != 0
        if ((row.at<float>(0, i) == 0 && row.at<float>(0, i + 1) > brightness_threshold) ||
            (row.at<float>(0, i) > brightness_threshold && row.at<float>(0, i + 1) == 0)) {
            count++;
        }
    }

    return count;
}

int max_contiguous(Mat row) {
    int max_max = 0, current_max = 0;
    bool currently_contiguous = false;

    for (int i = 0; i < row.cols; i++) {
        if (row.at<float>(0, i) <= 0) {
            if (currently_contiguous) {
                if (current_max > max_max) {
                    max_max = current_max;
                }
                currently_contiguous = false;
            }
        } else {
            if (currently_contiguous) {
                current_max++;
            } else {
                current_max = 1;
                currently_contiguous = true;
            }
        }
    }

    if (currently_contiguous) {
        if (current_max > max_max) {
            max_max = current_max;
        }
    }

    return max_max;
}