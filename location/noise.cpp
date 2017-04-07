//
// Created by ghiles on 4/7/17.
//

#include "noise.h"

void remove_noise_from_v(Mat& v, float brightness_threshold, int long_line_threshold) {
    vector<pair<int, int>> lines;  // starting pixels, (row, col)
    vector<int> heights;  // heights[i] corresponds to lines[i]
    get_lines_and_heights(v, brightness_threshold, lines, heights);

    remove_long_lines(v, lines, heights, long_line_threshold);

    double avg_height = accumulate(heights.begin(), heights.end(), 0.0) / heights.size();
    remove_short_lines(v, lines, heights, avg_height);
}

void remove_noise_from_h(Mat& h) {

}

void get_lines_and_heights(Mat v, float brightness_threshold, vector<pair<int, int>> lines, vector<int> heights) {
    int current_line = -1;

    for (int col = 0; col < v.cols; col++) {
        bool in_line = false;
        for (int row = 0; row < v.rows; row++) {
            if (v.at<float>(row, col) > brightness_threshold) {
                if (in_line) {
                    heights[current_line]++;
                } else {
                    in_line = true;
                    current_line++;
                    lines.push_back(pair<int, int>(row, col));
                    heights.push_back(1);
                }
            } else {
                v.at<float>(row, col) = min(0.0f, brightness_threshold);
                if (in_line) {
                    in_line = false;
                }
            }
        }
    }
}

void remove_long_lines(Mat& v, vector<pair<int, int>> lines, vector<int> heights, int long_line_threshold) {
    vector<int> lines_to_delete;

    for (uint i = 0; i < heights.size(); i++) {
        if (heights[i] >= long_line_threshold) {
            remove_vertical_line(v, lines[i].first, lines[i].second, heights[i]);
            lines_to_delete.push_back(i);
        }
    }

    delete_lines(lines, heights, lines_to_delete);
}

void remove_vertical_line(Mat& v, int row, int col, int height) {
    int start_row = row;
    int end_row = min(start_row + height, v.rows);
    for (int r = start_row; r < end_row; r++) {
        v.at<float>(r, col) = 0.0f;
    }
}

void delete_lines(vector<pair<int, int>> lines, vector<int> heights, vector<int> lines_to_delete) {
    sort(lines_to_delete.begin(), lines_to_delete.end());
    reverse(lines_to_delete.begin(), lines_to_delete.end());
    for (int line : lines_to_delete) {
        lines.erase(lines.begin() + line);
        heights.erase(heights.begin() + line);
    }
}

void remove_short_lines(Mat& v, vector<pair<int, int>> lines, vector<int> heights, double avg_height) {
    vector<int> lines_to_delete;
    for (uint i = 0; i < heights.size(); i++) {
        if (heights[i] < avg_height) {
            remove_vertical_line(v, lines[i].first, lines[i].second, heights[i]);
            lines_to_delete.push_back(i);
        }
    }
    delete_lines(lines, heights, lines_to_delete);
}