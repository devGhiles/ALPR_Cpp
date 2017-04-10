//
// Created by Nabil on 14/02/2017.
//

#include <opencv2/ml.hpp>
#include <dirent.h>

#include "HOT.h"
#include "OCRTarbia.h"

using namespace cv::ml;

int SZ = 20;
float affineFlags = WARP_INVERSE_MAP|INTER_LINEAR;

OCRTarbia::OCRTarbia() {}

void OCRTarbia::run(){
    svm = SVM::create();

    vector<Mat> trainCells;
    vector<Mat> testCells;
    vector<int> trainLabels;
    vector<int> testLabels;
    loadTrainTestLabel(fichierTarbia,trainCells,testCells,trainLabels,testLabels);
    cout << "Load des données ##" << endl;

    vector<Mat> deskewedTrainCells;
    vector<Mat> deskewedTestCells;
    CreateDeskewedTrainTest(deskewedTrainCells,deskewedTestCells,trainCells,testCells);
    cout << "Deskew ##" << endl;

    vector<vector<float> > trainHOT;
    vector<vector<float> > testHOT;
    CreateTrainTestHOT(trainHOT,testHOT,deskewedTrainCells,deskewedTestCells);
    cout << "Creation du trainTestHOT ##" << endl;

    int descriptor_size = trainHOT[0].size();
    cout << "Descriptor Size : " << descriptor_size << endl;

    Mat trainMat(trainHOT.size(),descriptor_size,CV_32FC1);
    Mat testMat(testHOT.size(),descriptor_size,CV_32FC1);
    stringstream ss(stringstream::in | stringstream::out);
    ss << "../Assets/SVM/trainData.jpg";imwrite(ss.str(),trainMat);ss.flush();
    ss << "../Assets/SVM/testData.jpg";imwrite(ss.str(),testMat);

    ConvertVectortoMatrix(trainHOT,testHOT,trainMat,testMat);

    Mat testResponse;
    SVMtrain(trainMat,trainLabels,testResponse,testMat, 43.5, 24.5);

    float count = 0;
    float accuracy = 0 ;
    SVMevaluate(testResponse,count,accuracy,testLabels);
    cout  <<" ===> TS :" << accuracy << endl;

    // Evaluation des paramétres du SVM
    /*Mat testResponse;
    float maxAccuracy = 0;
    float acc[60][60];
    for (int c = 1; c <= 60; c=c+1) {
        for (int gamma = 1; gamma <= 60; gamma=gamma+1) {
            SVMtrain(trainMat,trainLabels,testResponse,testMat, c, gamma);

            float count = 0;
            float accuracy = 0 ;
            SVMevaluate(testResponse,count,accuracy,testLabels);

            cout << "C : "<< c << ", Gamma : " << gamma <<" ===> TS :" << accuracy << endl;
            acc[c-1][gamma-1] = accuracy;
            if(accuracy > maxAccuracy) maxAccuracy = accuracy;
        }
    }
    for (int i = 1; i <=60; i=i+1) cout <<";"<<i; cout << endl;
    for (int i = 1; i <=60; i=i+1) {
        cout <<i;
        for (int j = 1; j <= 60; j=j+1) {
            cout <<";"<<acc[i-1][j-1];
        }
        cout <<endl;
    }
    cout << "Max accuracy : " << maxAccuracy;*/
}

HOGDescriptor hog(
        Size(20,20), //winSize
        Size(10,10), //blocksize
        Size(5,5), //blockStride,
        Size(10,10), //cellSize,
        9, //nbins,
        1, //derivAper,
        -1, //winSigma,
        0, //histogramNormType,
        0.2, //L2HysThresh,
        0,//gammal correction,
        64,//nlevels=64
        1);

void OCRTarbia::CreateTrainTestHOT(vector<vector<float> > &trainHOT, vector<vector<float> > &testHOT, vector<Mat> &deskewedtrainCells, vector<Mat> &deskewedtestCells){
    HOT hot;
    hot.xWins=1;
    hot.yWins=1;
    hot.nbrBins=20;
    for(int y=0;y<deskewedtrainCells.size();y++){
        trainHOT.push_back(hot.calculer(deskewedtrainCells[y]));
        /*vector<float> descriptors;
        hog.compute(deskewedtrainCells[y],descriptors);
        trainHOT.push_back(descriptors);*/
    }
    cout << "nbr de donnée ..." << trainHOT.size() <<endl;
    cout << "taille descri ..." << trainHOT[0].size() <<endl;
    for(int y=0;y<deskewedtestCells.size();y++){
        testHOT.push_back(hot.calculer(deskewedtestCells[y]));
        /*vector<float> descriptors;
        hog.compute(deskewedtestCells[y],descriptors);
        trainHOT.push_back(descriptors);*/
    }
}

void OCRTarbia::SVMtrain(Mat &trainMat,vector<int> &trainLabels, Mat &testResponse,Mat &testMat, float c, float gamma){
    svm->setGamma(gamma);//0.50625
    svm->setC(c);//12.5
    svm->setKernel(SVM::RBF);
    svm->setType(SVM::C_SVC);
    Ptr<TrainData> td = TrainData::create(trainMat, ROW_SAMPLE, trainLabels);
    svm->train(td);
    //svm->trainAuto(td);
    //svm->save("../SVM/model.yml");
    svm->predict(testMat, testResponse);

    //printSVMParams(svm);
}

/*void OCRTarbia::loadTrainTestLabelFromDataFolders(string &pathName, vector<Mat> &trainCells, vector<Mat> &testCells,vector<int> &trainLabels, vector<int> &testLabels){

    Mat img;
    int ImgCount = 0;
    vector<string> folders = list_folder(dossierTarbia);
    if(folders.size() <= 0)
        cout << "Dossier 'Data' vide" << endl;
    if(folders.size() != nbrClasses)
        cout << "Pas autant de dossiers que de classes" << endl;
    for(size_t i = 0; i < folders.size(); ++i)
    {
        vector<string> files = list_file(folders.at(i));
        if(files.size() <= 0)
            cout << "Dossier vide ou nombre d'image insuffisant";
        string folder_path = folders[i]; // was at()
        string label_folder = folder_path.substr(folder_path.length() - 1);
        for(size_t j = 0; j < files.size(); ++j)
        {
            img = imread(files.at(j));
            if(img.empty()) cout << "Impossible de lire un fichier data" << j << endl;
            if(j < int(0.9*files.size()))
            {
                trainCells.push_back(img);
            }
            else
            {
                testCells.push_back(img);
            }
            ImgCount++;
        }
    }
    cout << "Nombre d'images : " << ImgCount << endl;

    int digitClassNumber = 0; // was a float
    for(int z=0;z<int(0.9*ImgCount);z++){
        if(z % nbrTrainImgParClass == 0 && z != 0){
            digitClassNumber = digitClassNumber + 1;
        }
        trainLabels.push_back(digitClassNumber);
    }
    digitClassNumber = 0;
    for(int z=0;z<int(0.1*ImgCount);z++){
        if(z % nbrTestImgParClass == 0 && z != 0){
            digitClassNumber = digitClassNumber + 1;
        }
        testLabels.push_back(digitClassNumber);
    }
}*/

void OCRTarbia::loadTrainTestLabel(string &pathName, vector<Mat> &trainCells, vector<Mat> &testCells,vector<int> &trainLabels, vector<int> &testLabels){
    Mat img = imread(pathName,CV_LOAD_IMAGE_GRAYSCALE);
    if (img.empty()) {
        cout << "Erreur lors de l'ouverture de l'image: " << pathName << endl;
    }
    int ImgCount = 0;
    for(int i = 0; i < img.rows; i = i + SZ)
    {
        for(int j = 0; j < img.cols; j = j + SZ)
        {
            // 90% des images servent à l'entrainement | 10% restante servent aux tests
            Mat digitImg = (img.colRange(j,j+SZ).rowRange(i,i+SZ)).clone();
            if(j < int(0.9*img.cols))
            {
                trainCells.push_back(digitImg);
            }
            else
            {
                testCells.push_back(digitImg);
            }
            ImgCount++;
        }
    }

    cout << "Nombre d'images : " << ImgCount << endl;
    int digitClassNumber = 0; // was a float

    for(int z=0;z<int(0.9*ImgCount);z++){
        if(z % 450 == 0 && z != 0){
            digitClassNumber = digitClassNumber + 1;
        }
        trainLabels.push_back(digitClassNumber);
    }
    digitClassNumber = 0;
    for(int z=0;z<int(0.1*ImgCount);z++){
        if(z % 50 == 0 && z != 0){
            digitClassNumber = digitClassNumber + 1;
        }
        testLabels.push_back(digitClassNumber);
    }
}

void OCRTarbia::SVMevaluate(Mat &testResponse,float &count, float &accuracy,vector<int> &testLabels){

    for(int i=0;i<testResponse.rows;i++)
    {
        //cout << testResponse.at<float>(i,0) << " " << testLabels[i] << endl;
        if(testResponse.at<float>(i,0) == testLabels[i]){
            count = count + 1;
        }
    }
    accuracy = (count/testResponse.rows)*100;
}

void OCRTarbia::CreateDeskewedTrainTest(vector<Mat> &deskewedTrainCells,vector<Mat> &deskewedTestCells, vector<Mat> &trainCells, vector<Mat> &testCells){

    for(int i=0;i<trainCells.size();i++){

        Mat deskewedImg = deskew(trainCells[i]);
        deskewedTrainCells.push_back(deskewedImg);
    }

    for(int i=0;i<testCells.size();i++){

        Mat deskewedImg = deskew(testCells[i]);
        deskewedTestCells.push_back(deskewedImg);
    }
}

Mat OCRTarbia::deskew(Mat& img){
    Moments m = moments(img);
    if(abs(m.mu02) < 1e-2){
        return img.clone();
    }
    float skew = m.mu11/m.mu02;
    Mat warpMat(1,1,CV_32FC1); warpMat = (Mat_<float>(2, 3) << 1, skew, -0.5 * SZ * skew, 0, 1, 0); // was Mat
    Mat imgOut = Mat::zeros(img.rows, img.cols, img.type());
    warpAffine(img, imgOut, warpMat, imgOut.size(),affineFlags);

    return imgOut;
}

void OCRTarbia::ConvertVectortoMatrix(vector<vector<float> > &trainHOT, vector<vector<float> > &testHOT, Mat &trainMat, Mat &testMat)
{

    int descriptor_size = trainHOT[0].size();

    for(int i = 0;i<trainHOT.size();i++){
        for(int j = 0;j<descriptor_size;j++){
            trainMat.at<float>(i,j) = trainHOT[i][j];
        }
    }
    for(int i = 0;i<testHOT.size();i++){
        for(int j = 0;j<descriptor_size;j++){
            testMat.at<float>(i,j) = testHOT[i][j];
        }
    }
}

void OCRTarbia::printSVMParams(SVM *svm)
{
    cout << "Kernel type     : " << svm->getKernelType() << endl;
    cout << "Type            : " << svm->getType() << endl;
    cout << "C               : " << svm->getC() << endl;
    cout << "Degree          : " << svm->getDegree() << endl;
    cout << "Nu              : " << svm->getNu() << endl;
    cout << "Gamma           : " << svm->getGamma() << endl;
}

vector<string> OCRTarbia::list_folder(string path)
{
    vector<string> folders;
    DIR *dir = opendir(path.c_str());
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
        {
            string folder_path = path + "/" + string(entry->d_name);
            folders.push_back(folder_path);
        }
    }
    closedir(dir);
    return folders;

}
vector<string> OCRTarbia::list_file(string folder_path)
{
    vector<string> files;
    DIR *dir = opendir(folder_path.c_str());
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
        {
            string file_path = folder_path + "/" + string(entry->d_name);
            files.push_back(file_path);
        }
    }
    closedir(dir);
    return files;
}