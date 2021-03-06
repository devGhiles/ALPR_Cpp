//
// Created by ghiles on 4/13/17.
//

#ifndef ALPR_CPP_SVM_H
#define ALPR_CPP_SVM_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "../Utils/opencv.h"
#include "wavelet.h"
#include "noise.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

void train_svm();

void find_parameters();

float trainAndTest(Ptr<SVM> &svm);

float trainAndTest(Ptr<SVM> &svm, float c, float gamma);

bool readFolderAndExtractFeatures(string folder, int label, int num_for_test, vector<vector<float>> &trainingData,
                                  vector<int> &responsesData, vector<vector<float>> &testData,
                                  vector<float> &testResponsesData);

void features_extraction(Mat plate, vector<float> &features, int n_cols, int n_rows);

void features_extraction(Mat plate, vector<float> &features);

void v_features_extraction(Mat plate, vector<float> &features, int n_cols, int n_rows);

void v_features_extraction(Mat plate, vector<float> &features);

void hist_features_extraction(Mat plate, vector<float> &features);

void hog_features_extraction(Mat plate, vector<float> &features);

#endif //ALPR_CPP_SVM_H
