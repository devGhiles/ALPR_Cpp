//
// Created by Nabil on 28/02/2017.
//

#ifndef OPENCV_TEST_OCRTARBIA_H
#define OPENCV_TEST_OCRTARBIA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <opencv2/ml.hpp>

#include <opencv/cv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::ml;

class OCRTarbia{
    public:
        OCRTarbia();
        void run();

        Ptr<SVM> svm;

        string fichierTarbia;
        string dossierTarbia;
        int nbrClasses;
        int nbrTrainImgParClass;
        int nbrTestImgParClass;

        void CreateTrainTestHOT(vector<vector<float> > &trainHOG, vector<vector<float> > &testHOG, vector<Mat> &deskewedtrainCells, vector<Mat> &deskewedtestCells);
        void SVMtrain(Mat &trainMat,vector<int> &trainLabels, Mat &testResponse,Mat &testMat, float c, float gamma);
        void SVMevaluate(Mat &testResponse,float &count, float &accuracy,vector<int> &testLabels);
        void loadTrainTestLabel(string &pathName, vector<Mat> &trainCells, vector<Mat> &testCells,vector<int> &trainLabels, vector<int> &testLabels);
        void loadTrainTestLabelFromDataFolders(string &pathName, vector<Mat> &trainCells, vector<Mat> &testCells,vector<int> &trainLabels, vector<int> &testLabels);
    private:
        void CreateDeskewedTrainTest(vector<Mat> &deskewedTrainCells,vector<Mat> &deskewedTestCells, vector<Mat> &trainCells, vector<Mat> &testCells);
        void ConvertVectortoMatrix(vector<vector<float> > &trainHOG, vector<vector<float> > &testHOG, Mat &trainMat, Mat &testMat);
        void printSVMParams(SVM *svm);
        Mat deskew(Mat& img);

        vector<string> list_folder(string path);
        vector<string> list_file(string folder_path);
};

#endif //OPENCV_TEST_OCRTARBIA_H
