#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat img = imread("images/lena.tif", CV_8UC3);
    namedWindow("Test", CV_WINDOW_AUTOSIZE);
    imshow("Test", img);
    waitKey(0);

    return 0;
}