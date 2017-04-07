#include <iostream>
#include <opencv2/opencv.hpp>
#include "location/location.h"

using namespace std;
using namespace cv;

int main() {
    // read and show the original image
//    char filename[] = "images/medialab/1.jpg";
    char filename[] = "images/sliding.png";
    Mat img = imread(filename, 1);
    imshow("Image", img);
    waitKey(0);

    // perform lp location
    Mat lp;
    localize_license_plate(img, lp);
    lp.convertTo(lp, CV_8UC1);

    // show the lp
    imshow("LP", lp);
    waitKey(0);
    return 0;
}