//
// Created by Nabil on 28/02/2017.
//

#ifndef OPENCV_TEST_HOT_H
#define OPENCV_TEST_HOT_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class HOT {
    public :
        HOT();

        int xWins, yWins;
        int nbrBins;
        int xWinOverlap, yWinOverlap;
        vector<float> calculer(Mat in);

    private :
        int testMag = 0;
        bool matchTemplate(uchar in[3][3], int bin);
        float matchTemplateByArgMax(uchar in[3][3], int bin);
};


#endif //OPENCV_TEST_HOT_H
