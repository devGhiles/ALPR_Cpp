//
// Created by ghiles on 4/6/17.
//

#include "wavelet.h"

//--------------------------------
// signum
//--------------------------------
float sgn(float x) {
    float res = 0;
    if (x == 0) {
        res = 0;
    }
    if (x > 0) {
        res = 1;
    }
    if (x < 0) {
        res = -1;
    }
    return res;
}

//--------------------------------
// Soft shrinkage
//--------------------------------
float soft_shrink(float d, float T) {
    float res;
    if (fabs(d) > T) {
        res = sgn(d) * (fabs(d) - T);
    } else {
        res = 0;
    }

    return res;
}

//--------------------------------
// Hard shrinkage
//--------------------------------
float hard_shrink(float d, float T) {
    float res;
    if (fabs(d) > T) {
        res = d;
    } else {
        res = 0;
    }

    return res;
}

//--------------------------------
// Garrot shrinkage
//--------------------------------
float Garrot_shrink(float d, float T) {
    float res;
    if (fabs(d) > T) {
        res = d - ((T * T) / d);
    } else {
        res = 0;
    }

    return res;
}

//--------------------------------
// Wavelet transform
//--------------------------------
static void cvHaarWavelet(Mat &src, Mat &dst, int NIter) {
    float c, dh, dv, dd;
    assert(src.type() == CV_32FC1);
    assert(dst.type() == CV_32FC1);
    int width = src.cols;
    int height = src.rows;
    for (int k = 0; k < NIter; k++) {
        for (int y = 0; y < (height >> (k + 1)); y++) {
            for (int x = 0; x < (width >> (k + 1)); x++) {
                c = (src.at<float>(2 * y, 2 * x) + src.at<float>(2 * y, 2 * x + 1) + src.at<float>(2 * y + 1, 2 * x) +
                     src.at<float>(2 * y + 1, 2 * x + 1)) * 0.5;
                dst.at<float>(y, x) = c;

                dh = (src.at<float>(2 * y, 2 * x) + src.at<float>(2 * y + 1, 2 * x) - src.at<float>(2 * y, 2 * x + 1) -
                      src.at<float>(2 * y + 1, 2 * x + 1)) * 0.5;
                dst.at<float>(y, x + (width >> (k + 1))) = dh;

                dv = (src.at<float>(2 * y, 2 * x) + src.at<float>(2 * y, 2 * x + 1) - src.at<float>(2 * y + 1, 2 * x) -
                      src.at<float>(2 * y + 1, 2 * x + 1)) * 0.5;
                dst.at<float>(y + (height >> (k + 1)), x) = dv;

                dd = (src.at<float>(2 * y, 2 * x) - src.at<float>(2 * y, 2 * x + 1) - src.at<float>(2 * y + 1, 2 * x) +
                      src.at<float>(2 * y + 1, 2 * x + 1)) * 0.5;
                dst.at<float>(y + (height >> (k + 1)), x + (width >> (k + 1))) = dd;
            }
        }
        dst.copyTo(src);
    }
}

//--------------------------------
//Inverse wavelet transform
//--------------------------------
static void cvInvHaarWavelet(Mat &src, Mat &dst, int NIter, int SHRINKAGE_TYPE = 0, float SHRINKAGE_T = 50) {
    float c, dh, dv, dd;
    assert(src.type() == CV_32FC1);
    assert(dst.type() == CV_32FC1);
    int width = src.cols;
    int height = src.rows;
    //--------------------------------
    // NIter - number of iterations
    //--------------------------------
    for (int k = NIter; k > 0; k--) {
        for (int y = 0; y < (height >> k); y++) {
            for (int x = 0; x < (width >> k); x++) {
                c = src.at<float>(y, x);
                dh = src.at<float>(y, x + (width >> k));
                dv = src.at<float>(y + (height >> k), x);
                dd = src.at<float>(y + (height >> k), x + (width >> k));

                // (shrinkage)
                switch (SHRINKAGE_TYPE) {
                    case HARD:
                        dh = hard_shrink(dh, SHRINKAGE_T);
                        dv = hard_shrink(dv, SHRINKAGE_T);
                        dd = hard_shrink(dd, SHRINKAGE_T);
                        break;
                    case SOFT:
                        dh = soft_shrink(dh, SHRINKAGE_T);
                        dv = soft_shrink(dv, SHRINKAGE_T);
                        dd = soft_shrink(dd, SHRINKAGE_T);
                        break;
                    case GARROT:
                        dh = Garrot_shrink(dh, SHRINKAGE_T);
                        dv = Garrot_shrink(dv, SHRINKAGE_T);
                        dd = Garrot_shrink(dd, SHRINKAGE_T);
                        break;
                }

                //-------------------
                dst.at<float>(y * 2, x * 2) = 0.5 * (c + dh + dv + dd);
                dst.at<float>(y * 2, x * 2 + 1) = 0.5 * (c - dh + dv - dd);
                dst.at<float>(y * 2 + 1, x * 2) = 0.5 * (c + dh - dv - dd);
                dst.at<float>(y * 2 + 1, x * 2 + 1) = 0.5 * (c - dh - dv + dd);
            }
        }
        Mat C = src(Rect(0, 0, width >> (k - 1), height >> (k - 1)));
        Mat D = dst(Rect(0, 0, width >> (k - 1), height >> (k - 1)));
        D.copyTo(C);
    }
}