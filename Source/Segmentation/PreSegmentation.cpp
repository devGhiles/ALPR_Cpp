//
// Created by Nabil on 24/12/2016.
//

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>
#include <iostream>
#include "PreSegmentation.h"

using namespace std;
using namespace cv;

double compute_skew(Mat src) {
    cv::Size size = src.size();
    std::vector<cv::Vec4i> lines;
    /*
     * Probabiliste Hough Transform
     * Threshold     : 100, le nombre d'intersections minimum pour detecter une ligne
     * minLineLength : width/2, les caractére devrait prendre au moin la moitier de la longeur de la plaque
     * maxLineGap    : 20, écart des points à l'interieur d'un ligne
     */
    cv::HoughLinesP(src, lines, 1, CV_PI/180, 100, size.width / 2.f, 20);
    cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
    double angle = 0.;
    unsigned nb_lines = lines.size();
    for (unsigned i = 0; i < nb_lines; ++i)
    {
        cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]),
                             cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0 ,0));
        angle += atan2((double)lines[i][3] - lines[i][1],
                       (double)lines[i][2] - lines[i][0]);
    }
    angle /= nb_lines; // mean angle, in radians.

    std::cout << "File " << "Immatriculation" << ": " << angle * 180 / CV_PI << std::endl;

    //cv::imshow("Lines", disp_lines);
    return angle * 180 / CV_PI;
}

/*Mat deskew(Mat img, double angle) {
    //cv::bitwise_not(img, img);

    std::vector<cv::Point> points;
    cv::Mat_<uchar>::iterator it = img.begin<uchar>();
    cv::Mat_<uchar>::iterator end = img.end<uchar>();
    for (; it != end; ++it)
        if (*it)
            points.push_back(it.pos());

    cv::RotatedRect box = cv::minAreaRect(cv::Mat(points));
    cv::Mat rot_mat = cv::getRotationMatrix2D(box.center, angle, 1);
    cv::Mat rotated;
    cv::warpAffine(img, rotated, rot_mat, img.size(), cv::INTER_CUBIC);

    cv::Size box_size = box.size;
    if (box.angle < -45.)
        std::swap(box_size.width, box_size.height);
    cv::Mat cropped;
    cv::getRectSubPix(rotated, box_size, box.center, cropped);
    //imshow("Cropped", cropped);
    return cropped;
}*/

void deleteLines(Mat in){
    // Create the images that will use to extract the horizontal and vertical lines
    Mat horizontal = in.clone();
    Mat vertical = in.clone();

    // Specify size on horizontal axis
    int horizontalsize = horizontal.cols/30;
    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize,1));
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    // Show extracted horizontal lines
    imshow("horizontal", horizontal);
    //Need to find horizontal contours, so as to not damage letters
    vector<Vec4i> hierarchy;
    vector<vector<Point> >contours;
    findContours(horizontal, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    for (const auto& c : contours)
    {
        Rect r = boundingRect(c);

        float percentage_height = (float)r.height / (float)in.rows;
        float percentage_width = (float)r.width / (float)in.cols;

        //These exclude contours that probably are not dividing lines
        if (percentage_height > 0.05)
            continue;

        if (percentage_width < 0.50)
            continue;
        //fills in line with white rectange
        rectangle(in, r, Scalar(255,255,255), CV_FILLED);
    }
    imshow("horizontal deleted", in);
}

/*

detect-skew-angle: http://felix.abecassis.me/2011/09/opencv-detect-skew-angle/

rotation-deskewing: http://felix.abecassis.me/2011/10/opencv-rotation-deskewing/

bounding-box-skew-angle: http://felix.abecassis.me/2011/10/opencv-bounding-box-skew-angle/

 */