#include "../Common/Rois.h"
#include "../Common/InputParser.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char **argv) {

    string dataPath;
    FileStorage configFs("../config.json", FileStorage::READ);
    configFs["dataPath"] >> dataPath;
    configFs.release();

    InputParser input(argc, argv);
    auto argStrId = input.getCmdOption("-id");
    int roiId = stoi(argStrId);
    Rois roi(dataPath, roiId);

    float minA, maxA, minP, maxP, minH, maxH, minW, maxW;
    bool enA, enP, enH, enW, useSvm;
    int pTot, pTrun;

    FileStorage fs(dataPath + "predictParam.json", cv::FileStorage::READ);
    fs["useSVM"] >> useSvm;
    fs["usePerimeter"] >> enP;
    fs["useArea"] >> enA;
    fs["useMinH"] >> enH;
    fs["useMinW"] >> enW;
    fs["minPerimeter"] >> minP;
    fs["maxPerimeter"] >> maxP;
    fs["minArea"] >> minA;
    fs["maxArea"] >> maxA;
    fs["minHeight"] >> minH;
    fs["maxHeight"] >> maxH;
    fs["minWidth"] >> minW;
    fs["mawWidth"] >> maxW;
    fs["totalHistBeam"] >> pTot;
    fs["truncHistBeam"] >> pTrun;
    fs.release();

    int nbOfFeatures = (enA ? 1 : 0) + (enP ? 1 : 0) + (enH ? 1 : 0) + (enW ? 1 : 0) + pTrun;

    Mat_<float> descriptorOfRoi(1, nbOfFeatures);
    int i= 0;
    if (enP) descriptorOfRoi(0, i++) = ((roi.getArea() - minA) / (maxA - minA));
    if (enA) descriptorOfRoi(0, i++) = ((roi.getPerimeter() - minP) / (maxP - minP));
    if (enH) descriptorOfRoi(0, i++) = ((roi.getMinRectHeight() - minH) / (maxH - minH));
    if (enW) descriptorOfRoi(0, i++) = ((roi.getMinRectWidth() - minW) / (maxW - minW));
    for (float beam : roi.calcHueHist(pTot, 1.0)) {
        descriptorOfRoi(0, i++) = beam;
        if(i == nbOfFeatures)
            break;
    }

    Mat results;
    if (useSvm) {
        cv::Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>(dataPath + "svmconfig.yaml");
        svm->predict(descriptorOfRoi, results);
    } else {
        cv::Ptr<ml::KNearest> kNearest = Algorithm::load<ml::KNearest>(dataPath + "knnconfig.yaml");
        kNearest->predict(descriptorOfRoi, results);
    }

    cout << results.at<float>(0) ;

    return 0;
}