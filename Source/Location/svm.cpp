//
// Created by ghiles on 4/13/17.
//

#include "svm.h"

void train_svm() {
    Ptr<SVM> svm;
    trainAndTest(svm);
    svm->save("svm_plates.xml");
}

void find_parameters() {
    // Create the csv file
    ofstream csv_file;
    csv_file.open("svm_plates2.csv");
    csv_file << "c,gamma,error rate\n";

    // the real thing
    Ptr<SVM> svm;
    float c_init = 4.5f;  // 0.5f
    float gamma_init = 2.2f;  // 0.1f
    float c_final = 20.0f;  // 20.0f
    float gamma_final = 2.2f;  // 5.0f
    float c_step = 0.5f;
    float gamma_step = 0.1f;

    for (float c = c_init; c <= c_final; c += c_step) {
        for (float gamma = gamma_init; gamma <= gamma_final; gamma += gamma_step) {
            float error_rate = trainAndTest(svm, c, gamma);
            csv_file << to_string(c) + "," + to_string(gamma) + "," + to_string(error_rate) + "\n";
            csv_file.flush();
        }
        cout << 100.0f * (c - c_init + 1) / (c_final - c_init + 1) << "% done." << endl;
    }

    // Close the file
    csv_file.close();
}

/* returns the error rate (in %) */
float trainAndTest(Ptr<SVM> &svm) {
    float c = 5.0f, gamma = 2.2f;
    return trainAndTest(svm, c, gamma);
}

float trainAndTest(Ptr<SVM> &svm, float c, float gamma) {
    vector<vector<float>> trainingData;
    vector<int> responsesData;
    vector<vector<float>> testData;
    vector<float> testResponsesData;

    int num_for_test = 847;

    // Get the non plate images
    readFolderAndExtractFeatures("images/svm_plates/plates/%d.jpg", 1, num_for_test, trainingData, responsesData,
                                 testData, testResponsesData);
    readFolderAndExtractFeatures("images/svm_plates/not_plates/%d.jpg", -1, num_for_test, trainingData,
                                 responsesData,
                                 testData, testResponsesData);

    // Merge all data
    Mat trainingDataMat, testDataMat;
    convert_vector_to_mat(trainingData, trainingDataMat);
    Mat responses((int) responsesData.size(), 1, CV_32SC1, &responsesData[0]);
    convert_vector_to_mat(testData, testDataMat);
    Mat testResponses((int) testResponsesData.size(), 1, CV_32FC1, &testResponsesData[0]);

    svm = SVM::create();
    svm->setGamma(gamma);
    svm->setC(c);
    svm->setKernel(SVM::RBF);
    svm->setType(SVM::C_SVC);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

//    svm->train(trainingDataMat, ROW_SAMPLE, responses);
    svm->trainAuto(TrainData::create(trainingDataMat, ROW_SAMPLE, responses));

    if (testResponsesData.size() > 0) {
        // Test the ML Model
        Mat testPredict;
        svm->predict(testDataMat, testPredict);

        // Error calculation
        Mat errorMat = testPredict != testResponses;
        float error = 100.0f * countNonZero(errorMat) / testResponsesData.size();
        cout << "error rate: " << error << "%" << endl;
        return error;
    }

    return 100.0f;  // if no tests were made
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
//        features_extraction(frame, features);
//        hist_features_extraction(frame, features);
//        v_features_extraction(frame, features);
        hog_features_extraction(frame, features);

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
    int n_cols = 25;
    int n_rows = 3;
    features_extraction(plate, features, n_cols, n_rows);
}

void v_features_extraction(Mat plate, vector<float> &features, int n_cols, int n_rows) {
    // DWT transform to get v
    Mat h_dwt, v_dwt, gray;
    cvtColor(plate, gray, CV_BGR2GRAY);
    dwt2(gray, h_dwt, v_dwt);
    remove_noise_from_v(v_dwt, 0, 100);

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
    int n_cols = 10;
    int n_rows = 1;
    v_features_extraction(plate, features, n_cols, n_rows);
}

void hist_features_extraction(Mat plate, vector<float> &features) {
    Mat hls;
    cvtColor(plate, hls, CV_BGR2HLS);
    for (int i = 0; i < 256; i++) {
        features.push_back(0);
    }
    for (int row = 0; row < plate.rows; row++) {
        for (int col = 0; col < plate.cols; col++) {
            features[hls.at<Vec3b>(row, col)[0]]++;
        }
    }
}

void hog_features_extraction(Mat plate, vector<float> &features) {
    Mat gray;
    cvtColor(plate, gray, CV_BGR2GRAY);
    resize(gray, gray, Size(108, 36));
    HOGDescriptor hog(
            Size(108,36), //winSize
            Size(4,4), //blocksize
            Size(4,4), //blockStride,
            Size(2,2), //cellSize,
            9, //nbins,
            1, //derivAper,
            -1, //winSigma,
            0, //histogramNormType,
            0.2, //L2HysThresh,
            0, //gammal correction,
            64, //nlevels=64
            1);
    hog.compute(gray, features);
}