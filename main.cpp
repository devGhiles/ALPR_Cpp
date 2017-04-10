#include <iostream>

#include "Source/OCR/OCR.h"
#include "Source/Location/location.h"

using namespace std;
using namespace cv;

int main() {
    string nomFichierOriginal;
    nomFichierOriginal = "medialab/3.jpg";//
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

    if (plaqueImg.empty()) {
        cout << "Erreur lors de l'ouverture de l'image" << endl;
        return 0;
    }

    Mat plaque_gray;
    cvtColor(plaqueImg, plaque_gray, CV_BGR2GRAY);
    blur(plaque_gray, plaque_gray, Size(3,3));
    equalizeHist(plaque_gray, plaque_gray);//plaque_gray=histeq(plaque_gray);
    Plaque plaque(plaque_gray,Rect(0,0,0,0));

    //imshow("Image d'origine", plaque.plateImg);

    OCR ocr("");
    ocr.DEBUG = true;
    ocr.segmentsSauvgarde = true;
    ocr.nomFichierOriginal = nomFichierOriginal;
    string resultat = ocr.run(&plaque);

    cout << "Résultat : " << resultat;

    waitKey(0);

    // read and show the original image
    // char filename[] = "images/sliding.png";
    char filename[] = "images/slika/7.jpg";
    Mat img = imread(filename, 1);
    imshow("Image", img);

    // perform lp location
    Mat lp;
    localize_license_plate(img, lp);

    // show the lp
    imshow("LP", lp);
    return 0;
}