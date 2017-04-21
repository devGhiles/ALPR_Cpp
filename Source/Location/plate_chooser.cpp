//
// Created by ghiles on 4/9/17.
//

#include "plate_chooser.h"
#include "../Utils/opencv.h"
#include "wavelet.h"

void choose_plate(vector<Mat> candidate_plates, Mat &chosen_one) {
    if (!candidate_plates.empty()) {
        filter_plates_by_ratio(candidate_plates);
    }
    if (candidate_plates.empty()) {
        chosen_one = Mat::zeros(80, 20, CV_8UC3);
    } else {
        choose_using_svm(candidate_plates, chosen_one);
    }
}

void choose_lowest(vector<Mat> candidate_plates, Mat &chosen_one) {
    chosen_one = candidate_plates[0].clone();
}

double average_brightness(Mat m) {
    int count = 0;
    for (int row = 0; row < m.rows; row++) {
        for (int col = 0; col < m.cols; col++) {
            if (m.at<int>(row, col) > 0) {
                count += 1;
            }
        }
    }
    return count / (m.rows * m.cols);
}

void choose_highest_average_brightness(vector<Mat> candidate_plates, Mat &chosen_one) {
    double max_average_brightness = -1.0;

    for (Mat plate : candidate_plates) {
        Mat binary;
        Mat gray;
        cvtColor(plate, gray, CV_BGR2GRAY);
        threshold(gray, binary, 100, 255, CV_THRESH_BINARY);
        double avg = average_brightness(binary);
        if (avg > max_average_brightness) {
            cout << avg << endl;
            max_average_brightness = avg;
            chosen_one = plate.clone();
        }
    }
}

void choose_highest_average_brightness_in_v(vector<Mat> candidate_plates, Mat &chosen_one) {
    Mat h, v;
    double max_avg = -1.0;
    int chosen_index = -1;

    for (uint i = 0; i < candidate_plates.size(); i++) {
        Mat plate = candidate_plates[i];
        Mat gray;
        cvtColor(plate, gray, CV_BGR2GRAY);
        dwt2(gray, h, v);
        double current_avg = average(v);
        if (current_avg > max_avg) {
            max_avg = current_avg;
            chosen_index = i;
        }
    }

    chosen_one = candidate_plates[chosen_index].clone();
}

void choose_using_svm(vector<Mat> candidate_plates, Mat &chosen_one) {
    float max_score = -2.0f;
    Ptr<SVM> svm = Algorithm::load<ml::SVM>("svm_plates.xml");
    for (Mat plate : candidate_plates) {
        vector<float> features;
        features_extraction(plate, features);
        Mat featuresMat(1, (int) features.size(), CV_32FC1, &features[0]);
        Mat responses;
        svm->predict(featuresMat, responses, StatModel::RAW_OUTPUT);
        float score = responses.at<float>(0, 0);
        if (score > max_score) {
            max_score = score;
            chosen_one = plate;
        }
//        cout << "score: " << score << endl;
//        show(plate);
    }
    chosen_one = chosen_one.clone();
    svm->clear();
}

void filter_plates_by_ratio(vector<Mat> &candidate_plates) {
    int min_ratio = 3, max_ratio = 5;
    vector<Mat> kept_plates;
    for (Mat plate : candidate_plates) {
        if (((plate.cols / plate.rows) >= min_ratio) && ((plate.cols / plate.rows) <= max_ratio)) {
            kept_plates.push_back(plate);
        }
    }
    candidate_plates = kept_plates;
}