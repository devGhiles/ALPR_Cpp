//
// Created by ghiles on 4/6/17.
//

#include "opencv.h"

void show(Mat img) {
    show(img, "OpenCV");
}

void show(Mat img, string window_name) {
    imshow(window_name, img);
    waitKey(0);
}

void show(Plaque plaque) {
    show(plaque, "OpenCV");
}

void show(Plaque plaque, string window_name) {
    show(plaque.plateImg, window_name);
}

void show_dwt(Mat dwt_component) {
    show_dwt(dwt_component, "OpenCV");
}

void show_dwt(Mat dwt_component, string window_name) {
    Mat img;
    normalize(dwt_component, img, 0, 255, NORM_MINMAX, CV_8UC1);
    imshow(window_name, img);
    waitKey(0);
}

void show_binary(Mat img, string window_name) {
    Mat gray, binary;
    cvtColor(img, gray, CV_BGR2GRAY);
    threshold(gray, binary, 100, 255, THRESH_OTSU);
    show(binary, window_name);
}

void show_binary(Mat img) {
    show_binary(img, "OpenCV");
}

void show_v(Mat img) {
    show_v(img, "OpenCV");
}

void show_v(Mat img, string window_name) {
    Mat gray, h, v;
    convert_to_grayscale(img, gray);
    dwt2(gray, h, v);
    show_dwt(v, window_name);
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
    dst = src(Rect(start_col, start_row, width, height)).clone();
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

double average_submat(Mat m, int start_row, int end_row, int start_col, int end_col) {
    Mat submat;
    subimg(m, submat, start_row, end_row, start_col, end_col);
    return average(submat);
}

double average(Mat m) {
    return mean(m).val[0];
}

float black_density(Mat binary_image) {
    int count = 0;
    for (int row = 0; row < binary_image.rows; row++) {
        for (int col = 0; col < binary_image.cols; col++) {
            if (binary_image.at<int>(row, col) == 0) {
                count++;
            }
        }
    }
    return ((float) count) / (binary_image.rows * binary_image.cols);
}

void convert_vector_to_mat(vector<vector<float>> v, Mat &m) {
    if (v.empty()) {
        m = Mat();
        return;
    }

    int rows = (int) v.size();
    int cols = (int) v[0].size();
    m = Mat(rows, cols, CV_32FC1);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            m.at<float>(row, col) = v[row][col];
        }
    }
}