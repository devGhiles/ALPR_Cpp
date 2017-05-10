//
// Created by ghiles on 4/9/17.
//

#include "tests.h"
#include "location.h"
#include "wavelet.h"

void test_candidate_points(Mat src, vector<pair<int, int>> candidate_points) {
    for (pair<int, int> candidate : candidate_points) {
        int start_row = candidate.first * 2;
        int start_col = candidate.second * 2;
        int height = 40;
        int width = 5 * height;
        int end_row = start_row + height - 1;
        int end_col = start_col + width - 1;
        Mat src_copy = src.clone();
        rectangle(src_copy, Point(start_col, start_row), Point(end_col, end_row), Scalar(0, 255, 0));
        show(src_copy);
    }
}

void test_candidate_plates(vector<Mat> candidate_plates) {
    for (Mat plate : candidate_plates) {
        show(plate);
    }
}

void svm_generate_plates_database() {
    int candidates_count = 0;
    for (int i = 1; i <= 810; i++) {
        Mat img = imread("images/G1/G1 (" + to_string(i) + ").jpg");
        Mat plate;
        vector<Mat> candidates;
        localize_license_plate(img, plate, candidates);
        for (Mat lp : candidates) {
            candidates_count++;
            imwrite("images/candidates/" + to_string(candidates_count) + ".jpg", lp);
        }
        if (candidates_count == 810) {
            break;
        }
    }
}

void main_location() {
    Mat img = imread("images/G1/G1 (1).jpg");
    Mat plate;
    localize_license_plate(img, plate);
    show(plate);
}

void test_black_density() {
    Mat plate = imread("images/plates/G1/1.jpg");
    int n_cols = 16;
    int n_rows = 4;
    vector<float> features;
    features_extraction(plate, features, n_cols, n_rows);
    for (float f : features) {
        cout << f << endl;
    }
}

void test_folder(string test_folder, string target_folder, int num_images) {
    VideoCapture images;
    if (!images.open(test_folder)) {
        cout << "Can not open the folder images" << endl;
        return;
    }

    Mat frame;
    int img_index = 0;
    while (images.read(frame) && (img_index < num_images)) {
        img_index++;
        Mat plate;
        localize_license_plate(frame, plate);
        imwrite(target_folder + to_string(img_index) + ".jpg", plate);
    }
}

void test_location() {
    // G1: 810, G2: 700, G3: 743, G4: 572
    int num_images = 572, correctly_located_count = 0;
    vector<int> wrong_location;

    // for each image of the folder
    for (int i = 1; i <= num_images; i++) {
        // localize the plate
        Plaque plaque;
        Mat img = imread("images/G4/G4 (" + to_string(i) + ").jpg");
        localize_license_plate(img, plaque);

        // create plate mask
        Mat mask = Mat::zeros(img.rows, img.cols, CV_8UC1);
        for (int row = plaque.position.y; row < plaque.position.y + plaque.position.height; row++) {
            for (int col = plaque.position.x; col < plaque.position.x + plaque.position.width; col++) {
                mask.at<uchar>(row, col) = 255;
            }
        }

        // read ground truth
        Mat gt = imread("images/GT4/G4 (" + to_string(i) + ").jpg", IMREAD_GRAYSCALE);
        threshold(gt, gt, 200, 255, THRESH_BINARY);

        // intersection and union between detected plate and ground truth
        Mat intersection, uni;
        bitwise_and(mask, gt, intersection);
        bitwise_or(mask, gt, uni);

        // location ratio
        int whites_in_intersection = 0, whites_in_uni = 0;
        for (int row = 0; row < intersection.rows; row++) {
            for (int col = 0; col < intersection.cols; col++) {
                if (intersection.at<uchar>(row, col) > 0) {
                    whites_in_intersection++;
                }
                if (uni.at<uchar>(row, col) > 0) {
                    whites_in_uni++;
                }
            }
        }
        double ratio = ((double) whites_in_intersection / whites_in_uni);

        // count the correct ones
        if (ratio >= 0.5) {
            correctly_located_count++;
        } else {
            wrong_location.push_back(i);
        }

        // display advancement
        cout << i << "/" << num_images << " (" << (100.0 * i) / num_images << "%)" << endl;
    }
    // display location rate
    double rate = ((double) correctly_located_count) / num_images;
    cout << "Correctly located: " << correctly_located_count << " / " << num_images << endl;
    cout << "Location rate: " << rate * 100 << "%" << endl;
    cout << "Wrong locations: " << endl;
    for (int i : wrong_location) {
        cout << i << endl;
    }
}

void test_recognition() {
    int num_images = 26;
    string folder = "images/medialab/plates/difficult_shadows/"; // plates folder

    vector<string> numbers;
    read_plate_numbers(folder + "numbers.txt", numbers);

    OCR ocr;
    int correct_recognitions_count = 0;
    for (int i = 1; i <= num_images; i++) {
        Plaque plaque;
        plaque.plateImg = imread(folder + to_string(i) + ".jpg");
        convert_to_grayscale(plaque.plateImg, plaque.plateImg);
        if (ocr.run(&plaque) == numbers[i - 1]) {
            correct_recognitions_count++;
        }
        cout << i << "/" << num_images << " (" << 100.0f * (((float) i) / num_images) << "%)" << endl;
    }

    cout << "Recognition rate: " << 100.0f * (((float) correct_recognitions_count) / num_images);
}

// reads a file numbers.txt to the given vector
void read_plate_numbers(string filename, vector<string> &numbers) {
    ifstream f;
    f.open(filename);
    if (f.is_open()) {
        string number;
        while (getline(f, number)) {
            numbers.push_back(number);
        }
        f.close();
    }
}