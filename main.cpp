#include <iostream>

#include "Source/OCR/OCR.h"
#include "Source/Location/location.h"
#include "Source/Segmentation/SegmentationByContours.h"

using namespace std;
using namespace cv;

int main() {
    Mat img = imread("images/slika/1.jpg");
    Plaque plaque1;
    localize_license_plate(img, plaque1);
    rectangle(img, Point(plaque1.position.x - 2, plaque1.position.y - 2),
              Point(plaque1.position.x + plaque1.position.width + 2, plaque1.position.y + plaque1.position.height + 2),
              Scalar(0, 255, 0), 2);
    show(img);
    exit(0);
//    train_svm();
    main_location();
//    test_folder("images/G1/G1 (%d).jpg", "images/plates/tests/", 20);
//    test_folder("images/slika/%d.jpg", "images/plates/tests/", 10);
    exit(0);

    string nomFichierOriginal;
    /*nomFichierOriginal = "medialab/3.jpg";//
    //nomFichierOriginal = "image_0032.jpg";//
    //nomFichierOriginal = "greece.jpg";
    //nomFichierOriginal = "greece_skew.jpg";
    //nomFichierOriginal = "greek.jpg";
    //nomFichierOriginal = "Luxembourg.jpg";
    //nomFichierOriginal = "ghiles/18.jpg";
    //nomFichierOriginal = "massach.jpg";
    //nomFichierOriginal = "damaged.jpg";
    //nomFichierOriginal = "damaged_2.jpg";
    //nomFichierOriginal = "damaged_3.jpg";//
    //nomFichierOriginal = "damaged_4.jpg";//
    //nomFichierOriginal = "lux_skew.jpg";

    cv::Mat plaqueImg = cv::imread("../Assets/Img/" + nomFichierOriginal);

    */
    nomFichierOriginal = "2";

    // read and show the original image
    cv::Mat plaqueImg = cv::imread("images/slika/" + nomFichierOriginal + ".jpg");

    if (plaqueImg.empty()) {
        cout << "Erreur lors de l'ouverture de l'image" << endl;
        return 0;
    }
    imshow("Image", plaqueImg);

    // perform lp location
    Mat lp;
    localize_license_plate(plaqueImg, lp);

    // show the lp
    imshow("LP", lp);

    Mat plaque_gray;
    cvtColor(lp, plaque_gray, CV_BGR2GRAY);
    blur(plaque_gray, plaque_gray, Size(3, 3));
    equalizeHist(plaque_gray, plaque_gray);//plaque_gray=histeq(plaque_gray);
    Plaque plaque(plaque_gray, Rect(0, 0, 0, 0));

    //imshow("Image d'origine", plaque.plateImg);

    OCR ocr("");
    ocr.DEBUG = true;
    ocr.segmentsSauvgarde = true;
    ocr.nomFichierOriginal = nomFichierOriginal;
    string resultat = ocr.run(&plaque);

    cout << "Résultat : " << resultat;

    waitKey(0);

    return 0;
}