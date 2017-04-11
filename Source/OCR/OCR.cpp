//
// Created by Nabil on 31/01/2017.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>

#include "OCR.h"
#include "../ML/HOT.h"
#include "../ML/OCRTarbia.h"
#include "../Segmentation/SegmentationByContours.h"

using namespace std;
using namespace cv;

const char OCR::strCharacters[] = {'0','1','2','3','4','5','6','7','8','9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

OCR::OCR() { }

OCR::OCR(string tarbiaFile) {
    svmTarbiaFile = tarbiaFile;
    tailleSegment = 10;
    segmentsSauvgarde = false;
    mrabi = false;
}

string OCR::run(Plaque *plaque) {
    // Segmentation de la plaque
    SegmentationByContours segByContours;
    segByContours.DEBUG = true;
    vector<CharSegment> segments = segByContours.segmenter(*plaque);

    // Lancer la procédure d'apprentissage (Devra ce faire avec .load())
    OCRTarbia tarbia;
    tarbia.fichierTarbia="Assets/SVM/digits.png";
    // Donnnées d'apprentissage par dossiers d'images
    tarbia.dossierTarbia = "../SVM/Data";
    tarbia.nbrClasses = 36;
    tarbia.nbrTrainImgParClass = 450;
    tarbia.nbrTestImgParClass = 50;

    tarbia.run();

    for(int i=0; i<segments.size(); i++){
        // Pré-traitement des images pour les avoir avec la meme taille
        // Mat segment = preTraiterSegment(segments[i].img);
        // Sauvgarde des segment pour etre visualisées
        if(segmentsSauvgarde){
            stringstream ss(stringstream::in | stringstream::out);
            ss << "../Assets/Segments_Sauvgarde/" << nomFichierOriginal << "_" << i + 1 << ".jpg";
            imwrite(ss.str(),segments[i].img);
        }
        // Extraction des caractéristiques pour chaque segment
        Mat segment_threshold;
        threshold(segments[i].img, segment_threshold, 60, 255, CV_THRESH_BINARY);

        const Mat& f = hotFeatures(segment_threshold);
         // classification des chaque segment par ses caractéristiques "f"
        int charValue = tarbia.svm->predict(f);

        plaque->chars.push_back(strCharacters[charValue]);
        plaque->charsPos.push_back(segments[i].pos);
    }
    return plaque->str();
}

Mat OCR::hotFeatures(Mat in){
    HOT hot;
    hot.xWins=1;
    hot.yWins=1;
    hot.nbrBins=20;

    vector<float> feat = hot.calculer(in);
    Mat featMat(1, feat.size(), CV_32FC1);
    // Conversion de du vecteur caractéristique en matrice
    for(int i = 0;i<feat.size();i++){
        featMat.at<float>(0,i) = feat[i];
    }

    return featMat;
}

Mat OCR::preTraiterSegment(Mat in){
    //Remap image
    int h=in.rows;
    int w=in.cols;
    Mat transformMat=Mat::eye(2,3,CV_32F);
    int m=max(w,h);
    transformMat.at<float>(0,2)=m/2 - w/2;
    transformMat.at<float>(1,2)=m/2 - h/2;

    Mat warpImage(m,m, in.type());
    warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );

    //Mat out;
    //resize(warpImage, out, Size(charSize, charSize) );

    return warpImage;
}