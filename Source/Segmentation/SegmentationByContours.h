//
// Created by Nabil on 14/01/2017.
//

#ifndef OPENCV_TEST_SEGMENTATIONBYCONTOURS_H
#define OPENCV_TEST_SEGMENTATIONBYCONTOURS_H

#include <opencv2/core/mat.hpp>
#include "../Models/CharSegment.h"
#include "../Models/Plaque.h"

using namespace cv;

class SegmentationByContours{
    public:
        bool DEBUG = false;
        vector<CharSegment> segmenter(Plaque plaque);
    private:
        bool verifierRatio(Mat charSegment);
};

#endif //OPENCV_TEST_SEGMENTATIONBYCONTOURS_H
