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
#include "../Utils/strings.h"

using namespace std;
using namespace cv;

const char OCR::strCharacters[] = {'!', '#', '0','1','2','3','4','5','6','7','8','9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

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
    segByContours.DEBUG = false;
    segByContours.minH = OCRminH;
    vector<CharSegment> segments = segByContours.segmenter(*plaque, thresh);

    // Lancer la procédure d'apprentissage (Devra ce faire avec .load())
    OCRTarbia tarbia;
    tarbia.fichierTarbia="Assets/SVM/digits.png";
    // Donnnées d'apprentissage par dossiers d'images
    tarbia.nbrClasses = 36;
    tarbia.nbrTrainImgParClass = 450;
    tarbia.nbrTestImgParClass = 35;

    tarbia.run();

    for(int i=0; i<segments.size(); i++){
        // Pré-traitement des images pour les avoir avec la meme taille
        Mat segment = preTraiterSegment(segments[i].img);

        // Extraction des caractéristiques pour chaque segment
        //Mat grayChar;
        //cvtColor(segments[i].img, grayChar, CV_RGB2GRAY);

        Mat toSave = segment;

        Mat f;
        if(config.hot)
            f = hotFeatures(segment, tarbia.hot);
        else
            f = hogFeatures(segment, tarbia.hog);

         // classification des chaque segment par ses caractéristiques "f"
        int charValue = tarbia.svm->predict(f);

        if(DEBUG){
                plaque->chars.push_back(strCharacters[charValue]);
                plaque->charsPos.push_back(segments[i].pos);
        }
        else if(charValue >= 2){
            //if((!(plaque->chars.size() == 0 && charValue == 3))){ //!(i == segments.size()-1 && charValue == 3) && (
                plaque->chars.push_back(strCharacters[charValue]);
                plaque->charsPos.push_back(segments[i].pos);
            //}
        }

        // Sauvgarde des segment pour etre visualisées
        if(segmentsSauvgarde){
            stringstream ss(stringstream::in | stringstream::out);
            ss << "./Assets/Segments_Sauvgarde/" /*<< strCharacters[charValue] << "/"*/ << nomFichierOriginal << "_" << i + 1 << ".jpg";
            imwrite(ss.str(),toSave);
        }
    }


    if( plaque->chars.size() == 0)
        return "";
    else if(plaque->chars.size() == 6)
        return plaque->str();

    string result = plaque->str();
    int size = result.size();

    while(result.size() > 6 && result[0] == '1')
        result.erase(result.begin() + 0);
    while(result.size() > 6 && result[result.size() - 1] == '1')
        result.erase(result.begin() + result.size() - 1);

    return result;
}

Mat OCR::hotFeatures(Mat &in, HOT hot){
    vector<float> feat = hot.calculer(in);
    Mat featMat(1, feat.size(), CV_32FC1);
    // Conversion de du vecteur caractéristique en matrice
    for(int i = 0;i<feat.size();i++){
        featMat.at<float>(0,i) = feat[i];
    }

    return featMat;
}

Mat OCR::hogFeatures(Mat &in, HOGDescriptor hog){
    vector<float> feat;
    resize(in, in, Size(config.car_x, config.car_y));
    hog.compute(in,feat);
    Mat featMat(1, feat.size(), CV_32FC1);
    // Conversion de du vecteur caractéristique en matrice
    for(int i = 0;i<feat.size();i++){
        featMat.at<float>(0,i) = feat[i];
    }

    return featMat;
}

Mat OCR::preTraiterSegment(Mat &in){
    /*//Remap image
    int h=in.rows;
    int w=in.cols;
    Mat transformMat=Mat::eye(2,3,CV_32F);
    int m=max(w,h);
    transformMat.at<float>(0,2)=m/2 - w/2;
    transformMat.at<float>(1,2)=m/2 - h/2;

    Mat warpImage(m,m, in.type());
    warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );*/

    Mat out;
    //resize(in, out, Size(config.car_x, config.car_y), 0, 0, INTER_LINEAR );
out=in;
    blur(out, out, Size(3,3));
    equalizeHist(out, out);
    threshold(out, out, 127, 255, CV_THRESH_BINARY_INV);

    return out;
}