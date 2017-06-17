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
        bool DEBUG = true;
        float minH=24;
        vector<CharSegment> segmenter(Plaque plaque, int thresh);
        Mat segmenterTrainData(Mat in);
    private:
        bool verifierRatio(Mat charSegment);
        Mat Dilation(Mat in, int, void* );
};

#endif //OPENCV_TEST_SEGMENTATIONBYCONTOURS_H
