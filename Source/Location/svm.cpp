//
// Created by ghiles on 4/13/17.
//

#include "svm.h"

void train_svm() {
    Ptr<SVM> svm;
    trainAndTest(svm);
    svm->save("svm_plates.yml");
}

void trainAndTest(Ptr<SVM> &svm) {
    vector<vector<float>> trainingData;
    vector<int> responsesData;
    vector<vector<float>> testData;
    vector<float> testResponsesData;

    int num_for_test = 200;  // on 810 samples

    // Get the non plate images
    readFolderAndExtractFeatures("images/svm_plates/plates_all/%d.jpg", 1, num_for_test, trainingData, responsesData,
                                 testData, testResponsesData);
    readFolderAndExtractFeatures("images/svm_plates/not_plates_all/%d.jpg", -1, num_for_test, trainingData,
                                 responsesData,
                                 testData, testResponsesData);

    // Merge all data
    Mat trainingDataMat, testDataMat;
    convert_vector_to_mat(trainingData, trainingDataMat);
    Mat responses((int) responsesData.size(), 1, CV_32SC1, &responsesData[0]);
    convert_vector_to_mat(testData, testDataMat);
    Mat testResponses((int) testResponsesData.size(), 1, CV_32FC1, &testResponsesData[0]);

    svm = SVM::create();
    svm->setGamma(0.5);
    svm->setC(10);
    svm->setKernel(SVM::RBF);
    svm->setType(SVM::C_SVC);
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

bool readFolderAndExtractFeatures(string folder, int label, int num_for_test, vector<vector<float>> &trainingData,
                                  vector<int> &responsesData, vector<vector<float>> &testData,
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

        if (img_index >= num_for_test) {
            trainingData.push_back(features);
            responsesData.push_back(label);
        } else {
            testData.push_back(features);
            testResponsesData.push_back(label);
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
    int n_cols = 16;
    int n_rows = 4;
    features_extraction(plate, features, n_cols, n_rows);
}

void v_features_extraction(Mat plate, vector<float> &features, int n_cols, int n_rows) {
    // DWT transform to get v
    Mat h_dwt, v_dwt, gray;
    cvtColor(plate, gray, CV_BGR2GRAY);
    dwt2(gray, h_dwt, v_dwt);

    // variables
    int W = v_dwt.cols;
    int H = v_dwt.rows;
    int w = (int) round(W / n_cols);
    int h = (int) round(H / n_rows);

    // the real thing
    for (int p_row = 0; p_row < n_rows; p_row++) {
        int start_row = p_row * h;
        int end_row = min(start_row + w - 1, v_dwt.rows - 1);
        for (int p_col = 0; p_col < n_cols; p_col++) {
            int start_col = p_col * w;
            int end_col = min(start_col + h - 1, v_dwt.cols - 1);

            Mat cell;
            subimg(v_dwt, cell, start_row, end_row, start_col, end_col);
            features.push_back((float) mean(cell).val[0]);
        }
    }
}

void v_features_extraction(Mat plate, vector<float> &features) {
    int n_cols = 16;
    int n_rows = 4;
    v_features_extraction(plate, features, n_cols, n_rows);
}