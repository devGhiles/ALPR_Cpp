//
// Created by Nabil on 28/02/2017.
//

#include "HOT.h"

using namespace std;

HOT::HOT() {}
void printCell(Mat in){
    for (int k = 0; k < in.cols; ++k) {
        for (int i = 0; i < in.rows; ++i) {
            cout << (int)in.at<uchar>(k,i) << " | ";
        }
        cout << "[]" << endl;
    }
    cout << "*-*" << endl;
}

vector<float> HOT::calculer(Mat in) {
    int xWinSize = in.cols/xWins, yWinSize = in.rows/yWins;

    // Formation de la matrice intensité
    uchar intensites[in.rows][in.cols];
    for (int i = 0; i < in.rows; ++i) {
        for (int j = 0; j < in.cols; ++j) {
            intensites[i][j] = in.at<uchar>(i,j);
        }
    }
    // Formation de la matrice magnitude
    uchar magnitudes[in.rows][in.cols];
    Mat Sx, Sy, mag;
    Sobel( in, Sx, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    convertScaleAbs(Sx, Sx);
    Sobel( in, Sy, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
    convertScaleAbs(Sy, Sy);
    addWeighted(Sx, 0.5, Sy, 0.5, 0, mag);
    for (int i = 0; i < in.rows; ++i) {
        for (int j = 0; j < in.cols; ++j) {
            magnitudes[i][j] = mag.at<uchar>(i,j);
        }
    }

    float winBins1[nbrBins];
    float winBins2[nbrBins];
    float templateIntensityMaxSum = 0; int templateIntensityMaxSumIndex = 0;
    float winBins3[nbrBins];
    float winBins4[nbrBins];
    float templateMagnitudeMaxSum = 0; int templateMagnitudeMaxSumIndex = 0;

    vector<float> features(0,0);
    uchar intensityCell[3][3];
    uchar magnitudeCell[3][3];

    for (int i = 0; i < yWins; ++i) { // Pour chaque Block horizontal
        for (int j = 0; j < xWins; ++j) { // Pour chaque Block vertical
            // Réinitialiser winBins
            fill_n(winBins1, nbrBins, 0);
            fill_n(winBins2, nbrBins, 0);
            fill_n(winBins3, nbrBins, 0);
            fill_n(winBins4, nbrBins, 0);
            templateIntensityMaxSum = 0; templateIntensityMaxSumIndex = 0;
            templateMagnitudeMaxSum = 0; templateMagnitudeMaxSumIndex = 0;

            for (int k = i*yWinSize+1; k < (i+1)*yWinSize-1; ++k) { // Pour chaque pixel avec voisinage du block (i,j)
                for (int l = j*xWinSize+1; l < (j+1)*xWinSize-1; ++l) {
                    // Recuperer les intensités du cell actuelle (horizontalement)
                    copy(&intensites[k-1][l-1], &intensites[k-1][l+1], intensityCell[0]);
                    copy(&intensites[k][l-1], &intensites[k][l+1], intensityCell[1]);
                    copy(&intensites[k+1][l-1], &intensites[k+1][l+1], intensityCell[2]);
                    //Recuperer les magnitude des gradients du cell actuelle (horizontalement)
                    copy(&magnitudes[k-1][l-1], &magnitudes[k-1][l+1], magnitudeCell[0]);
                    copy(&magnitudes[k][l-1], &magnitudes[k][l+1], magnitudeCell[1]);
                    copy(&magnitudes[k+1][l-1], &magnitudes[k+1][l+1], magnitudeCell[2]);
                    // Verifier la satisfaction des formules
                    for (int m = 0; m < nbrBins; ++m) {
                        //(1)
                        if(matchTemplate(intensityCell, m)){
                            winBins1[m]++;
                        }
                        //(2)
                        if (matchTemplateByArgMax(intensityCell, m) > templateIntensityMaxSum){
                            templateIntensityMaxSum = matchTemplateByArgMax(intensityCell, m);
                            templateIntensityMaxSumIndex = m;
                        }
                        //(3)
                        if(matchTemplate(magnitudeCell, m)){
                            winBins3[m]++;
                        }
                        //(4)
                        if (matchTemplateByArgMax(magnitudeCell, m) > templateMagnitudeMaxSum){
                            templateMagnitudeMaxSum = matchTemplateByArgMax(magnitudeCell, m);
                            templateMagnitudeMaxSumIndex = m;
                        }
                    }
                    //(2)
                    winBins2[templateIntensityMaxSumIndex]++;
                    //(4)
                    winBins4[templateMagnitudeMaxSumIndex]++;
                }
            }
            // Normalisation puis concaténation des l'histograms
            int binsSum1 = 0, binsSum2 = 0, binsSum3 = 0, binsSum4 = 0;
            //(1)
            for(float feat : winBins1) { binsSum1 += feat; }
            for(float feat : winBins1) { features.push_back(feat/binsSum1); }
            //(2)
            for(float feat : winBins2) { binsSum2 += feat; }
            //for(float feat : winBins2) { features.push_back(feat/binsSum2); }
            //(3)
            for(float feat : winBins3) { binsSum3 += feat; }
            //for(float feat : winBins3) { features.push_back(feat/binsSum3); }
            //(4)
            for(float feat : winBins4) { binsSum4 += feat; }
            //for(float feat : winBins4) { features.push_back(feat/binsSum4); }
        }
    }
    return features;
}

bool HOT::matchTemplate(uchar in[3][3], int bin){
    switch (bin){
        case 0: return (in[1][1] > in[0][1] && in[1][1] > in[2][1]);
        case 1: return (in[1][1] > in[1][0] && in[1][1] > in[1][2]);
        case 2: return (in[1][1] > in[0][0] && in[1][1] > in[2][2]);
        case 3: return (in[1][1] > in[0][2] && in[1][1] > in[2][0]);
        case 4: return (in[1][1] > in[0][1] && in[1][1] > in[1][0]);
        case 5: return (in[1][1] > in[0][1] && in[1][1] > in[1][2]);
        case 6: return (in[1][1] > in[2][1] && in[1][1] > in[1][2]);
        case 7: return (in[1][1] > in[1][0] && in[1][1] > in[2][1]);

        case 8: return (in[1][1] > in[0][1] && in[1][1] > in[0][2]);
        case 9: return (in[1][1] > in[0][2] && in[1][1] > in[2][2]);
        case 10: return (in[1][1] > in[2][0] && in[1][1] > in[2][2]);
        case 11: return (in[1][1] > in[0][0] && in[1][1] > in[2][0]);
        case 12: return (in[1][1] > in[0][0] && in[1][1] > in[2][1]);
        case 13: return (in[1][1] > in[0][2] && in[1][1] > in[2][1]);
        case 14: return (in[1][1] > in[0][1] && in[1][1] > in[2][0]);
        case 15: return (in[1][1] > in[0][1] && in[1][1] > in[2][2]);

        case 16: return (in[1][1] > in[0][2] && in[1][1] > in[1][0]);
        case 17: return (in[1][1] > in[2][2] && in[1][1] > in[1][0]);
        case 18: return (in[1][1] > in[0][0] && in[1][1] > in[1][2]);
        case 19: return (in[1][1] > in[2][0] && in[1][1] > in[1][2]);
    }
}

float HOT::matchTemplateByArgMax(uchar in[3][3], int bin){
    switch (bin){
        case 0: return (in[1][1] + in[0][1] + in[2][1]);
        case 1: return (in[1][1] + in[1][0] + in[1][2]);
        case 2: return (in[1][1] + in[0][0] + in[2][2]);
        case 3: return (in[1][1] + in[0][2] + in[2][0]);
        case 4: return (in[1][1] + in[0][1] + in[1][0]);
        case 5: return (in[1][1] + in[0][1] + in[1][2]);
        case 6: return (in[1][1] + in[2][1] + in[1][2]);
        case 7: return (in[1][1] + in[1][0] + in[2][1]);

        case 8: return (in[1][1] + in[0][1] + in[0][2]);
        case 9: return (in[1][1] + in[0][2] + in[2][2]);
        case 10: return (in[1][1] + in[2][0] + in[2][2]);
        case 11: return (in[1][1] + in[0][0] + in[2][0]);
        case 12: return (in[1][1] + in[0][0] + in[2][1]);
        case 13: return (in[1][1] + in[0][2] + in[2][1]);
        case 14: return (in[1][1] + in[0][1] + in[2][0]);
        case 15: return (in[1][1] + in[0][1] + in[2][2]);

        case 16: return (in[1][1] + in[0][2] + in[1][0]);
        case 17: return (in[1][1] + in[2][2] + in[1][0]);
        case 18: return (in[1][1] + in[0][0] + in[1][2]);
        case 19: return (in[1][1] + in[2][0] + in[1][2]);
    }
}