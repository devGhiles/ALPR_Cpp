//
// Created by ghiles on 4/13/17.
//

#include "svm.h"

void main_svm() {
    Ptr<SVM> svm;
    trainAndTest(svm);
    svm->save("plates.svm");
}

void trainAndTest(Ptr<SVM> &svm) {
    vector<float> trainingData;
    vector<int> responsesData;
    vector<float> testData;
    vector<float> testResponsesData;

    int num_for_test = 20;

    // Get the non plate images
    readFolderAndExtractFeatures("images/svm_plates/plates/%d.jpg", 0, num_for_test, trainingData, responsesData,
                                 testData, testResponsesData);
    readFolderAndExtractFeatures("images/svm_plates/not_plates/%d.jpg", 1, num_for_test, trainingData, responsesData,
                                 testData, testResponsesData);

    // Some printing
    cout << "Num of train samples: " << responsesData.size() << endl;
    cout << "Num of test samples: " << testResponsesData.size() << endl;

    // Merge all data
    Mat trainingDataMat((int) trainingData.size() / 2, 2, CV_32FC1, &trainingData[0]);
    Mat responses((int) responsesData.size(), 1, CV_32SC1, &responsesData[0]);

    Mat testDataMat((int) testData.size() / 2, 2, CV_32FC1, &testData[0]);
    Mat testResponses((int) testResponsesData.size(), 1, CV_32FC1, &testResponsesData[0]);
    svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::CHI2);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

    svm->train(trainingDataMat, ROW_SAMPLE, responses);

    if (testResponsesData.size() > 0) {
        cout << "Evaluation" << endl;
        cout << "==========" << endl;
        // Test the ML Model
        Mat testPredict;
        svm->predict(testDataMat, testPredict);
        cout << "Prediction Done" << endl;
        // Error calculation
        Mat errorMat = testPredict != testResponses;
        float error = 100.0f * countNonZero(errorMat) / testResponsesData.size();
        cout << "Error: " << error << "%" << endl;
    }
}

bool readFolderAndExtractFeatures(string folder, int label, int num_for_test, vector<float> &trainingData,
                                  vector<int> &responsesData, vector<float> &testData,
                                  vector<float> &testResponsesData) {
    VideoCapture images;
    if (!images.open(folder)) {
        cout << "Can not open the folder images" << endl;
        return false;
    }

    Mat frame;
    int img_index = 0;
    while (images.read(frame)) {
        // Extract features
        vector<float> features;
        features_extraction(frame, features);

        for (int i = 0; i < features.size(); i++) {
            if (img_index >= num_for_test) {
                trainingData.push_back(features[i]);
                trainingData.push_back(features[i]);
                responsesData.push_back(label);
            } else {
                testData.push_back(features[i]);
                testData.push_back(features[i]);
                testResponsesData.push_back((float) label);
            }
        }
        img_index++;
    }
    return true;
}

void features_extraction(Mat plate, vector<float> &features, int n_cols, int n_rows) {
    // variables
    int W = plate.cols;
    int H = plate.rows;
    int w = (int) round(W / n_cols);
    int h = (int) round(H / n_rows);

    // convert to binary (OTSU)
    Mat gray, binary;
    cvtColor(plate, gray, CV_BGR2GRAY);
    threshold(gray, binary, 100, 255, CV_THRESH_OTSU);

    // the real thing
    for (int p_row = 0; p_row < n_rows; p_row++) {
        int start_row = p_row * h;
        int end_row = min(start_row + w - 1, plate.rows - 1);
        for (int p_col = 0; p_col < n_cols; p_col++) {
            int start_col = p_col * w;
            int end_col = min(start_col + h - 1, plate.cols - 1);

            Mat cell;
            subimg(binary, cell, start_row, end_row, start_col, end_col);
            features.push_back(black_density(cell));
        }
    }
}

void features_extraction(Mat plate, vector<float> &features) {
    int n_cols = 10;
    int n_rows = 5;
    features_extraction(plate, features, n_cols, n_rows);
}