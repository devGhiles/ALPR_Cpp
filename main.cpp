#include <iostream>
#include <opencv2/opencv.hpp>
#include "location/location.h"

using namespace std;
using namespace cv;

int main() {
    // read and show the original image
    char filename[] = "images/medialab/1.jpg";
    Mat img = imread(filename, CV_8UC3);
    namedWindow("Location", CV_WINDOW_AUTOSIZE);
    imshow("Test", img);
    waitKey(0);

    // perform lp location
    Mat lp = Mat(img.rows, img.cols, CV_8UC3);
    localize_license_plate(img, lp);

    // show the lp
    imshow("Test", lp);
    waitKey(0);
    return 0;
}