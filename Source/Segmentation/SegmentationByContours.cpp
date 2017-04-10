//
// Created by Nabil on 14/01/2017.
//
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include <iostream>

#include "SegmentationByContours.h"

using namespace std;
using namespace cv;

vector<CharSegment> SegmentationByContours::segmenter(Plaque plaque) {
    Mat input=plaque.plateImg;
    vector<CharSegment> out;
    // Binarization
    Mat plaque_threshold;
    threshold(input, plaque_threshold, 60, 255, CV_THRESH_BINARY_INV);
    if(DEBUG)
        imshow("Plaque binaire", plaque_threshold);
    Mat plaque_contours;
    plaque_threshold.copyTo(plaque_contours);
    // Recherche des contours des caractéres
    vector< vector< Point> > contours;
    findContours(plaque_contours,
                 contours,
                 CV_RETR_EXTERNAL, // Retrouver les contours extairne
                 CV_CHAIN_APPROX_NONE); // Tout les pixels de chaque contour

    // Draw blue contours on a white image
    cv::Mat resultat;
    plaque_threshold.copyTo(resultat);
    cvtColor(resultat, resultat, CV_GRAY2RGB);
    cv::drawContours(resultat,contours,
                     -1, // déssiner tout les contours
                     cv::Scalar(255,0,0), // contours colorés en bleu
                     1); // thickness = 1

    vector<vector<Point> >::iterator itc= contours.begin();
    // Suppression des contours n'appartenant pas au ratio et la region de la plaque
    while (itc!=contours.end()) {
        Rect mr= boundingRect(Mat(*itc));
        rectangle(resultat, mr, Scalar(0,255,0));
        //Crop image
        //Mat auxRoi(plaque_threshold, mr);
        Mat auxRoi = plaque_threshold(mr);
        if(verifierRatio(auxRoi)){
            auxRoi=verifierRatio(auxRoi);
            // Recuperation du meme segment à partir de l'image grayscal
            Mat graySeg = input(mr);
            out.push_back(CharSegment(graySeg, mr));
            rectangle(resultat, mr, Scalar(0,125,255));
        }
        ++itc;
    }
    if(DEBUG)
        cout << "Num chars: " << out.size() << "\n";
    if(DEBUG)
        imshow("Segmented Chars", resultat);

    return out;
}

bool SegmentationByContours::verifierRatio(Mat charSegment) {
    //Char sizes 45x77
    float aspect=45.0f/77.0f;
    float charAspect= (float)charSegment.cols/(float)charSegment.rows;
    float error=0.35;
    float minHeight=15;
    float maxHeight=28;
    //We have a different aspect ratio for number 1, and it can be ~0.2
    float minAspect=0.2;
    float maxAspect=aspect+aspect*error;
    //area of pixels
    float area=countNonZero(charSegment);
    //bb area
    float bbArea=charSegment.cols*charSegment.rows;
    //% of pixel in area
    float percPixels=area/bbArea;

    if(DEBUG)
        cout << "Aspect: "<< aspect << " ["<< minAspect << "," << maxAspect << "] "  << "Area "<< percPixels <<" Char aspect " << charAspect  << " Height char "<< charSegment.rows << endl;
    if(percPixels < 0.8 && charAspect > minAspect && charAspect < maxAspect && charSegment.rows >= minHeight && charSegment.rows < maxHeight)
        return true;
    else
        return false;
}
