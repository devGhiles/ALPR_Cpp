//
// Created by Nabil on 31/01/2017.
//

#ifndef OPENCV_TEST_OCR_H
#define OPENCV_TEST_OCR_H

#include <cvaux.h>

#include "../Models/Plaque.h"
#include "../Models/CharSegment.h"

using namespace std;
using namespace cv;

class OCR{
    public:
        static const char strCharacters[];
        string svmTarbiaFile;
        int tailleSegment;
        bool mrabi;

        OCR();
        OCR(string tarbiaFile);
        string run(Plaque *plaque);

        Mat preTraiterSegment(Mat in);

        void train(Mat trainData, Mat trainClasses, int nlayers);
        Mat hotFeatures(Mat input);

        bool DEBUG;
        bool segmentsSauvgarde;
        string nomFichierOriginal;
    private:

};

#endif //OPENCV_TEST_OCR_H
