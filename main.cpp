#include <iostream>
#include <opencv2/opencv.hpp>
#include "location/location.h"

using namespace std;
using namespace cv;

int main() {
    // read and show the original image
//    char filename[] = "images/sliding.png";
    char filename[] = "images/medialab/1.jpg";
    Mat img = imread(filename, 1);
    show(img, "Image");

    // perform lp location
    Mat lp;
    localize_license_plate(img, lp);
    lp.convertTo(lp, CV_8UC1);

    // show the lp
    show(lp, "LP");
    return 0;
}