//
// Created by ghiles on 4/6/17.
//

#ifndef ALPR_CPP_WAVELET_H
#define ALPR_CPP_WAVELET_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

// Filter type
#define NONE 0  // no filter
#define HARD 1  // hard shrinkage
#define SOFT 2  // soft shrinkage
#define GARROT 3  // garrot filter

float sgn(float x);
float soft_shrink(float d, float T);
float hard_shrink(float d, float T);
float Garrot_shrink(float d, float T);
static void cvHaarWavelet(Mat &src, Mat &dst, int NIter);
static void cvInvHaarWavelet(Mat &src, Mat &dst, int NIter, int SHRINKAGE_TYPE = 0, float SHRINKAGE_T = 50);

#endif //ALPR_CPP_WAVELET_H
