//
// Created by Nabil on 19/02/2017.
//

#ifndef OPENCV_TEST_SEGMENT_H
#define OPENCV_TEST_SEGMENT_H

#include <opencv/cv.hpp>

using namespace std;
using namespace cv;

class CharSegment{
public:
    CharSegment();
    CharSegment(Mat i, Rect p);
    Mat img;
    Rect pos;
};

#endif //OPENCV_TEST_SEGMENT_H