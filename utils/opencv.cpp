//
// Created by ghiles on 4/6/17.
//

#include "opencv.h"

void show(Mat img, string window_name) {
    imshow(window_name, img);
    waitKey(0);
}

void show_dwt(Mat dwt_component, string window_name) {
    Mat img;
    dwt_component.convertTo(img, CV_8UC1);
    imshow(window_name, img);
    waitKey(0);
}

void convert_to_grayscale(Mat src, Mat &dst) {
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
float average_of_positives(Mat &img) {
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

void subimg(Mat src, Mat &dst, int start_row, int end_row, int start_col, int end_col) {
    int width = end_col - start_col + 1;
    int height = end_row - start_row + 1;
    dst = src(Rect(start_row, start_col, width, height)).clone();
}

double average_mat_float(Mat m) {
    double sum = 0.0;
    for (int row = 0; row < m.rows; row++) {
        for (int col = 0; col < m.cols; col++) {
            sum += m.at<float>(row, col);
        }
    }

    return sum / (m.rows * m.cols);
}

double average_submat_float(Mat m, int start_row, int end_row, int start_col, int end_col) {
    Mat submat;
    subimg(m, submat, start_row, end_row, start_col, end_col);
    return average_mat_float(submat);
}