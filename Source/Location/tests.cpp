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
    int num_images = 810;
    int num_candidates = 4050;
    for (int i = 1; i <= num_images; i++) {
        Mat img = imread("images/G1/G1 (" + to_string(i) + ").jpg");
        Mat plate;
        vector<Mat> candidates;
        localize_license_plate(img, plate, candidates);
        filter_plates_by_ratio(candidates);
        for (Mat lp : candidates) {
            candidates_count++;
            imwrite("images/candidates/" + to_string(candidates_count) + ".jpg", lp);
        }
        if (candidates_count > num_candidates) {
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

float test_location() {
    // G1: 810, G2: 700, G3: 743, G4: 572
    int num_images = 810, correctly_located_count = 0;
    double total_time_in_seconds = 0.0;
    vector<int> wrong_location;

    // for each image of the folder
    for (int i = 1; i <= num_images; i++) {
        // localize the plate
        Plaque plaque;
        Mat img = imread("images/G1/G1 (" + to_string(i) + ").jpg");
        clock_t tic = clock();
        localize_license_plate(img, plaque);
        clock_t toc = clock();
        double time_in_seconds = ((double) (toc - tic)) / CLOCKS_PER_SEC;
        total_time_in_seconds += time_in_seconds;

        // create plate mask
        Mat mask = Mat::zeros(img.rows, img.cols, CV_8UC1);
        for (int row = plaque.position.y; row < plaque.position.y + plaque.position.height; row++) {
            for (int col = plaque.position.x; col < plaque.position.x + plaque.position.width; col++) {
                mask.at<uchar>(row, col) = 255;
            }
        }

        // read ground truth
        Mat gt = imread("images/GT1/G1 (" + to_string(i) + ").jpg", IMREAD_GRAYSCALE);
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
        if (ratio >= 0.7) {
            correctly_located_count++;
        } else {
            wrong_location.push_back(i);
        }

        // display advancement
        cout << i << "/" << num_images << " (" << (100.0 * i) / num_images << "%)" << endl;
    }
    // display location rate
    float rate = ((float) correctly_located_count) / num_images;
    cout << "Correctly located: " << correctly_located_count << " / " << num_images << endl;
    cout << "Location rate: " << rate * 100 << "%" << endl;
    cout << "Average time: " << total_time_in_seconds / num_images << endl;
    cout << "Wrong locations: " << endl;
    for (int i : wrong_location) {
        cout << i << endl;
    }
    return rate;
}

float test_location(int bs_prop, float brightness_threshold, int long_line_threshold, int transitions_threshold,
                    int top_lines_to_check, double p, int rough_width, int rough_height, int resize_coeff) {
    // G1: 810, G2: 700, G3: 743, G4: 572
    int num_images = 250, correctly_located_count = 0;
    vector<int> wrong_location;

    // for each image of the folder
    for (int i = 1; i <= num_images; i++) {
        cout << "image " << i << endl;

        // localize the plate
        Plaque plaque;
        Mat img = imread("images/G1/G1 (" + to_string(i) + ").jpg");
        Mat resized_img;
        resize(img, resized_img, Size(img.cols / resize_coeff, img.rows / resize_coeff));
        localize_license_plate(resized_img, plaque, bs_prop, brightness_threshold, long_line_threshold,
                               transitions_threshold, top_lines_to_check, p, rough_width, rough_height);

        plaque.position.x *= resize_coeff;
        plaque.position.y *= resize_coeff;
        plaque.position.width *= resize_coeff;
        plaque.position.height *= resize_coeff;
        subimg(img, plaque.plateImg, plaque.position.y, plaque.position.y + plaque.position.height, plaque.position.x,
               plaque.position.x + plaque.position.width);

        // create plate mask
        Mat mask = Mat::zeros(img.rows, img.cols, CV_8UC1);
        for (int row = plaque.position.y; row < plaque.position.y + plaque.position.height; row++) {
            for (int col = plaque.position.x; col < plaque.position.x + plaque.position.width; col++) {
                mask.at<uchar>(row, col) = 255;
            }
        }

        // read ground truth
        Mat gt = imread("images/GT1/G1 (" + to_string(i) + ").jpg", IMREAD_GRAYSCALE);
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
    }

    return ((float) correctly_located_count) / num_images;
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

void test_system() {
    int num_images = 26;
    double total_time_in_seconds = 0.0;
    string folder = "images/medialab/";  // images folder

    vector<string> numbers;
    read_plate_numbers(folder + "numbers.txt", numbers);

    OCR ocr;
    int correct_recognitions_count = 0;
    for (int i = 1; i <= num_images; i++) {
        Mat img = imread(folder + to_string(i));
        Plaque plaque;
        clock_t tic = clock();
        localize_license_plate(img, plaque);
        convert_to_grayscale(plaque.plateImg, plaque.plateImg);
        if (ocr.run(&plaque) == numbers[i - 1]) {
            correct_recognitions_count++;
        }
        clock_t toc = clock();
        double time_in_seconds = ((double) (toc - tic)) / CLOCKS_PER_SEC;
        total_time_in_seconds += time_in_seconds;
        cout << i << "/" << num_images << " (" << 100.0f * (((float) i) / num_images) << "%)" << endl;
    }

    cout << "Success rate: " << 100.0f * (((float) correct_recognitions_count) / num_images) << endl;
    cout << "Average time: " << total_time_in_seconds / num_images << endl;
}

void find_localization_parameters() {
//    int bs_prop = 9;
//    int long_line_threshold = 20;
//    int transitions_threshold = 7;
//    int top_lines_to_check = 9;
//    float brightness_threshold = 0.0f;
//    double p = 0.3;
//    int rough_width = 90;
//    int rough_height = 9;
//    int resize_coeff = 2;

    int bs_prop = 6;
    int long_line_threshold = 20 / 2;
    int transitions_threshold = 4;
    int top_lines_to_check = 9 / 2;
    float brightness_threshold = 0.0f;
    double p = 0.3;
    int rough_width = 90 / 2;
    int rough_height = 9 / 2;
    int resize_coeff = 2;

    ofstream f;
    f.open("csv/localisation/p2.csv");
    f << "p,success rate,average time\n";

    int row_number = 1;
    int max_rate_row = -1;
    float max_rate = -1.0f;
    for (p = 0.0; p <= 0.5; p += 0.1) {
        cout << "p: " << p << endl;

        row_number++;
        clock_t tic = clock();
        float rate = test_location(bs_prop, brightness_threshold, long_line_threshold, transitions_threshold,
                                   top_lines_to_check, p, rough_width, rough_height, resize_coeff);
        clock_t toc = clock();
        double time_in_seconds = ((double) (toc - tic)) / CLOCKS_PER_SEC;

        if (rate > max_rate) {
            max_rate = rate;
            max_rate_row = row_number;
        }
        f << p << "," << 100.0f * rate << "," << time_in_seconds / 250 << "\n";
    }

    f.close();
    cout << "Max row is: " << max_rate_row << endl;
    cout << "Max rate is: " << max_rate << endl;
}