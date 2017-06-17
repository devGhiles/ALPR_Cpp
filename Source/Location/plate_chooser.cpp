//
// Created by ghiles on 4/9/17.
//

#include "plate_chooser.h"
#include "../Utils/opencv.h"
#include "wavelet.h"

/* returns the index in candidate_plates of the chosen plate */
int choose_plate(vector<Mat> candidate_plates, Mat &chosen_one) {
    if (!candidate_plates.empty()) {
        filter_plates_by_ratio(candidate_plates);
    }
    if (candidate_plates.empty()) {
        chosen_one = Mat::zeros(20, 80, CV_8UC3);
        return -1;
    } else {
        return choose_using_svm(candidate_plates, chosen_one);
    }
}

int choose_plate(vector<Plaque> candidate_plates, Plaque &chosen_one) {
    if (!candidate_plates.empty()) {
        filter_plates_by_ratio(candidate_plates);
    }
    if (candidate_plates.empty()) {
        chosen_one = Plaque(Mat::zeros(80, 20, CV_8UC3), Rect(0, 0, 80, 20));
        return -1;
    } else {
        return choose_using_svm(candidate_plates, chosen_one);
    }
}

int choose_lowest(vector<Mat> candidate_plates, Mat &chosen_one) {
    chosen_one = candidate_plates[0].clone();
    return 0;
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

int choose_highest_average_brightness_in_v(vector<Mat> candidate_plates, Mat &chosen_one) {
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
    return chosen_index;
}

int choose_using_svm(vector<Mat> candidate_plates, Mat &chosen_one) {
    float max_score = -2.0f;
    int chosen_index = -1;
    int first_index = -1, second_index = -1, third_index = -1;
    float first_score = -1000.0f, second_score = -1000.0f, third_score = -1000.0f;
    Ptr<SVM> svm = Algorithm::load<ml::SVM>("svm_plates.xml");
    for (int i = 0; i < candidate_plates.size(); i++) {
        Mat plate = candidate_plates[i];
        vector<float> features;
        hog_features_extraction(plate, features);
        Mat featuresMat(1, (int) features.size(), CV_32FC1, &features[0]);
        Mat responses;
        svm->predict(featuresMat, responses, StatModel::RAW_OUTPUT);
        float score = responses.at<float>(0, 0);
        if (score > max_score) {
            max_score = score;
            chosen_one = plate;
            chosen_index = i;
        }
        // Top 3
        if (score > first_score) {
            third_score = second_score;
            second_score = first_score;
            first_score = score;
            third_index = second_index;
            second_index = first_index;
            first_index = i;
        } else if (score > second_score) {
            third_score = second_score;
            second_score = score;
            third_index = second_index;
            second_index = i;
        } else if (score > third_score) {
            third_score = score;
            third_index = i;
        }
//        cout << "score: " << score << endl;
//        show(plate);
    }

    // Choose among top 3
    vector<Mat> top_candidates;
    if (first_index >= 0) {
        top_candidates.push_back(candidate_plates[first_index].clone());
    }
    if (second_index >= 0) {
        top_candidates.push_back(candidate_plates[second_index].clone());
    }
    if (third_index >= 0) {
        top_candidates.push_back(candidate_plates[third_index].clone());
    }
    chosen_index = choose_using_ocr(top_candidates, chosen_one);

    chosen_one = chosen_one.clone();
    svm->clear();
    if (chosen_index == 0) {
        return first_index;
    } else if (chosen_index == 1) {
        return second_index;
    } else {
        return third_index;
    }
}

int choose_using_svm(vector<Plaque> candidate_plates, Plaque &chosen_one) {
    float max_score = -10.0f;
    int chosen_index = -1;
    Ptr<SVM> svm = Algorithm::load<ml::SVM>("svm_plates.xml");
    for (int i = 0; i < candidate_plates.size(); i++) {
        Plaque plaque = candidate_plates[i];
        vector<float> features;
        hog_features_extraction(plaque.plateImg, features);
        Mat featuresMat(1, (int) features.size(), CV_32FC1, &features[0]);
        Mat responses;
        svm->predict(featuresMat, responses, StatModel::RAW_OUTPUT);
        float score = responses.at<float>(0, 0);
        if (score > max_score) {
            max_score = score;
            chosen_one = plaque;
            chosen_index = i;
        }
    }
    chosen_one.plateImg = chosen_one.plateImg.clone();
    svm->clear();
    return chosen_index;
}

int choose_using_ocr(vector<Mat> candidate_plates, Mat &chosen_one) {
    OCR ocr;
    int top_length = -1, top_length_index = -1;
    for (int i = 0; i < candidate_plates.size(); i++) {
        Mat plate = candidate_plates[i];
        Plaque plaque(plate.clone(), Rect(0, 0, 0, 0));
        convert_to_grayscale(plaque.plateImg, plaque.plateImg);
        invert_grayscale(plaque.plateImg, plaque.plateImg);
        String plateNumber = ocr.run(&plaque);

        if (plateNumber.length() >= 7) {
            chosen_one = plate.clone();
            return i;
        }

        if ((plateNumber.length() < 7) && (plateNumber.length() > top_length)) {
            top_length = (int) plateNumber.length();
            cout << plateNumber << endl;
            top_length_index = i;
        }
    }

    if (top_length_index >= 0) {
        chosen_one = candidate_plates[top_length_index].clone();
        return top_length_index;
    } else {
        return -1;
    }
}

void filter_plates_by_ratio(vector<Mat> &candidate_plates) {
    int min_ratio = 2, max_ratio = 5;
    vector<Mat> kept_plates;
    for (Mat plate : candidate_plates) {
        if (((plate.cols / plate.rows) >= min_ratio) && ((plate.cols / plate.rows) <= max_ratio)) {
            kept_plates.push_back(plate);
        }
    }
    candidate_plates = kept_plates;
}

void filter_plates_by_ratio(vector<Plaque> &candidate_plates) {
    int min_ratio = 2, max_ratio = 5;
    vector<Plaque> kept_plates;
    for (Plaque plaque : candidate_plates) {
        Mat plate = plaque.plateImg;
        if (((plate.cols / plate.rows) >= min_ratio) && ((plate.cols / plate.rows) <= max_ratio)) {
            kept_plates.push_back(plaque);
        }
    }
    candidate_plates = kept_plates;
}