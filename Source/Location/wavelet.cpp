//
// Created by ghiles on 4/6/17.
//

#include "wavelet.h"

void dwt2(Mat src, Mat& c, Mat& h, Mat& v, Mat& d) {
    int width = (src.cols - 1) / 2;
    int height = (src.rows - 1) / 2;

    c = Mat(height, width, CV_32FC1);
    h = Mat(height, width, CV_32FC1);
    v = Mat(height, width, CV_32FC1);
    d = Mat(height, width, CV_32FC1);

    Mat img;
    src.convertTo(img, CV_32FC1);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            c.at<float>(row, col) = 0.5f * (
                    img.at<float>(2 * row, 2 * col) +
                    img.at<float>(2 * row + 1, 2 * col) +
                    img.at<float>(2 * row, 2 * col + 1) +
                    img.at<float>(2 * row + 1, 2 * col + 1));
            v.at<float>(row, col) = 0.5f * (
                    img.at<float>(2 * row, 2 * col) +
                    img.at<float>(2 * row + 1, 2 * col) -
                    img.at<float>(2 * row, 2 * col + 1) -
                    img.at<float>(2 * row + 1, 2 * col + 1));
            h.at<float>(row, col) = 0.5f * (
                    img.at<float>(2 * row, 2 * col) -
                    img.at<float>(2 * row + 1, 2 * col) +
                    img.at<float>(2 * row, 2 * col + 1) -
                    img.at<float>(2 * row + 1, 2 * col + 1));
            d.at<float>(row, col) = 0.5f * (
                    img.at<float>(2 * row, 2 * col) -
                    img.at<float>(2 * row + 1, 2 * col) -
                    img.at<float>(2 * row, 2 * col + 1) +
                    img.at<float>(2 * row + 1, 2 * col + 1));
        }
    }
}

// we don't need c and d in this application. The below function is simpler to call
void dwt2(Mat src, Mat& h, Mat& v) {
    Mat c, d;
    dwt2(src, c, h, v, d);
}

void ewt2(Mat src, Mat& c, Mat& h, Mat& v, Mat& d) {
    int width = (src.cols - 1) / 2;
    int height = (src.rows - 1) / 2;

    c = Mat(height, width, CV_32FC1);
    h = Mat(height, width, CV_32FC1);
    v = Mat(height, width, CV_32FC1);
    d = Mat(height, width, CV_32FC1);

    Mat img;
    src.convertTo(img, CV_32FC1);

    for (int row = 1; row < height; row++) {
        for (int col = 1; col < width; col++) {
            c.at<float>(row - 1, col - 1) = 0.5f * (
                    img.at<float>(2 * row - 1, 2 * col - 1) +
                    img.at<float>(2 * row + 1, 2 * col - 1) +
                    img.at<float>(2 * row - 1, 2 * col + 1) +
                    img.at<float>(2 * row + 1, 2 * col + 1));
            v.at<float>(row - 1, col - 1) = 0.5f * (
                    img.at<float>(2 * row - 1, 2 * col - 1) +
                    img.at<float>(2 * row + 1, 2 * col - 1) -
                    img.at<float>(2 * row - 1, 2 * col + 1) -
                    img.at<float>(2 * row + 1, 2 * col + 1));
            h.at<float>(row - 1, col - 1) = 0.5f * (
                    img.at<float>(2 * row - 1, 2 * col - 1) -
                    img.at<float>(2 * row + 1, 2 * col - 1) +
                    img.at<float>(2 * row - 1, 2 * col + 1) -
                    img.at<float>(2 * row + 1, 2 * col + 1));
            d.at<float>(row - 1, col - 1) = 0.5f * (
                    img.at<float>(2 * row - 1, 2 * col - 1) -
                    img.at<float>(2 * row + 1, 2 * col - 1) -
                    img.at<float>(2 * row - 1, 2 * col + 1) +
                    img.at<float>(2 * row + 1, 2 * col + 1));
        }
    }
}

// we don't need c and d in this application. The below function is simpler to call
void ewt2(Mat src, Mat& h, Mat& v) {
    Mat c, d;
    ewt2(src, c, h, v, d);
}