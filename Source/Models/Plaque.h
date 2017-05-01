//
// Created by Nabil on 19/02/2017.
//

#ifndef OPENCV_TEST_PLAQUE_H
#define OPENCV_TEST_PLAQUE_H

#include <opencv/cv.hpp>

using namespace std;
using namespace cv;

class Plaque{
public:
    Rect position;
    Mat plateImg;
    vector<char> chars; // Résultat de la classification effectuée par "OCR.cpp"
    vector<Rect> charsPos;

    Plaque();
    Plaque(Mat img, Rect pos);
    string str();
    Plaque clone();
};

#endif //OPENCV_TEST_PLAQUE_H
