//
// Created by age2pierre on 07/02/17.
//

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include "ClassificationTest.h"

using namespace std;
using namespace cv;

void ClassificationTest::calcMeanAndStDev(vector<float> &in, double &mean, double &stdev) {
    double sum = accumulate(in.begin(), in.end(), 0.0);
    mean = sum / in.size();

    vector<double> diff(in.size());
    transform(in.begin(), in.end(), diff.begin(), [mean](double x) {
        return x - mean;
    });
    double sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    stdev = sqrt(sq_sum / in.size());
}

float ClassificationTest::normValue(float val, float lowerBound, float upperBound) {
    return (val - lowerBound) / (upperBound - lowerBound);
}

void ClassificationTest::runSimpleKnn(int K = 10) {
    vector<float> rslts;
    Ptr<ml::KNearest> kNearest(ml::KNearest::create());
    kNearest->setDefaultK(K);
    for (int i = 0; i < KNN_KFOLD; i++) {
        kNearest->train(knnTrainData[i]);
        Mat resp;
        float rslt = kNearest->calcError(knnTestData[i], false, resp);
        rslts.push_back(rslt);
    }
    double mean, stdev;
    calcMeanAndStDev(rslts, mean, stdev);
    cout << '\t' << K << "-NN testing with " << KNN_KFOLD << "-fold cross validation"
            "\n\tMean error " << mean << endl;
}

void ClassificationTest::runSimpleSVM(ml::SVM::Types type, ml::SVM::KernelTypes kernelType) {
    vector<float> rslts;
    double mean, stdev;
    for (int i = 0; i < SVM_KFOLD; i++) {
        Ptr<ml::SVM> svm(ml::SVM::create());
        svm->setType(type);
        svm->setKernel(kernelType);
        svm->trainAuto(svmTrainData[i]);
        Mat resp;
        float rslt = svm->calcError(svmTestData[i], false, resp);
        rslts.push_back(rslt);
    }
    calcMeanAndStDev(rslts, mean, stdev);
    cout << "\tSVM testing with " << SVM_KFOLD << "-fold cross validation" <<
            "\n\tMean error " << mean << endl;
}

ClassificationTest::ClassificationTest() {
    FileStorage inFs("./data/indexRois.json", FileStorage::READ);
    FileNodeIterator it = inFs["index"].begin();
    while (it != inFs["index"].end()) {
        Rois currentRoi("./data/rois/" + to_string((int) *it) + ".json");
        data[(currentRoi.getClassId() - 1)].push_back(currentRoi);
        it++;
    }
    inFs.release();
    for (auto &vecOfRoi : data)
        for (auto &roi : vecOfRoi) {
            vecA.push_back(roi.getArea());
            vecH.push_back(roi.getMinRectHeight());
            vecP.push_back(roi.getPerimeter());
            vecW.push_back(roi.getMinRectWidth());
        }
    calcMeanAndStDev(vecP, meanP, stDevP);
    calcMeanAndStDev(vecA, meanA, stDevA);
    calcMeanAndStDev(vecH, meanH, stDevH);
    calcMeanAndStDev(vecW, meanW, stDevW);

    minMaxA = minmax_element(vecA.begin(), vecA.end());
    minMaxP = minmax_element(vecP.begin(), vecP.end());
    minMaxH = minmax_element(vecH.begin(), vecH.end());
    minMaxW = minmax_element(vecW.begin(), vecW.end());
}

void ClassificationTest::createDataSet(
        bool enPerimeter, bool enArea, bool enMinH, bool enMinW,
        int totalHistBeam, int truncHistBeam, bool normalize = true
) {
    vector<tuple<Mat_<float>, int>> vecSamples;
    int nbOfSamples = 0;
    int nbOfFeatures = (enArea ? 1 : 0) + (enPerimeter ? 1 : 0) + (enMinH ? 1 : 0) + (enMinW ? 1 : 0) + totalHistBeam;
    // for each samples calc the features hist + shape descriptions
    for (auto &vecOfRoi : data)
        for (auto &roi : vecOfRoi) {
            Mat_<float> currentRow(1, nbOfFeatures);
            float p(roi.getPerimeter()), a(roi.getArea()), h(roi.getMinRectHeight()), w(roi.getMinRectWidth());
            int i = 0;
            if (enPerimeter)
                currentRow(0, i++) = (normalize ? normValue(p, *minMaxP.first,
                                                            /*(float) min((3 * meanP + stDevP), (double)*/ *minMaxP.second)/*)*/
                                                : p);
            if (enArea)
                currentRow(0, i++) = (normalize ? normValue(a, *minMaxA.first,
                                                            /*(float) min((3 * meanA + stDevA), (double)*/ *minMaxA.second)/*)*/
                                                : a);
            if (enMinH)
                currentRow(0, i++) = (normalize ? normValue(h, *minMaxH.first,
                                                            /*(float) min((3 * meanH + stDevH), (double)*/ *minMaxH.second)/*)*/
                                                : h);
            if (enMinW)
                currentRow(0, i++) = (normalize ? normValue(w, *minMaxW.first,
                                                            /*(float) min((3 * meanW + stDevW), (double)*/ *minMaxW.second)/*)*/
                                                : w);
            for (float beam : roi.calcHueHist(totalHistBeam, 1.0))
                currentRow(0, i++) = beam;
            vecSamples.push_back(make_tuple(currentRow, roi.getClassId()));
            nbOfSamples++;
        }
    // print info
    cout << "Dataset number of samples : " << nbOfSamples << " - Features used : " <<
         (enArea ? "Area, " : "") << (enPerimeter ? "Perimter, " : "") <<
         (enMinH ? "Height of smallest bounding rect, " : "") << (enMinW ? "Width of smallest bounding rect, " : "")
         << " and " << truncHistBeam << '/' << totalHistBeam << " hue hist beam" << endl;
    // shuffle the samples
    random_shuffle(vecSamples.begin(), vecSamples.end());
    // create the dataset containing all the samples
    Mat_<float> matSamples;
    Mat_<int> matResponses;
    matSamples.reserve((size_t) nbOfSamples);
    matResponses.reserve((size_t) nbOfSamples);
    for (auto &item : vecSamples) {
        Mat_<float> sample;
        int response;
        tie(sample, response) = item;
        matSamples.push_back(sample);
        matResponses.push_back(response);
    }
    this->fullDataSet = ml::TrainData::create(matSamples, ml::ROW_SAMPLE, matResponses);
    // create the datasets for Kfold cross validation
    for (int i = 0; i < KNN_KFOLD; i++) {
        Mat_<float> matTestSamples, matTrainSamples;
        Mat_<int> matTestResponses, matTrainResponses;
        int h = 0;
        for (auto &item : vecSamples) {
            Mat_<float> sample;
            int response;
            tie(sample, response) = item;
            if (h++ % KNN_KFOLD == i) {
                matTrainSamples.push_back(sample);
                matTrainResponses.push_back(response);
            } else {
                matTestSamples.push_back(sample);
                matTestResponses.push_back(response);
            }
        }
        knnTestData[i] = ml::TrainData::create(matTrainSamples, ml::ROW_SAMPLE, matTrainResponses);
        knnTrainData[i] = ml::TrainData::create(matTestSamples, ml::ROW_SAMPLE, matTestResponses);
    }
    // create the datasets for Kfold cross validation
    for (int i = 0; i < SVM_KFOLD; i++) {
        Mat_<float> matTestSamples, matTrainSamples;
        Mat_<int> matTestResponses, matTrainResponses;
        int h = 0;
        for (auto &item : vecSamples) {
            Mat_<float> sample;
            int response;
            tie(sample, response) = item;
            if (h++ % SVM_KFOLD == i) {
                matTrainSamples.push_back(sample);
                matTrainResponses.push_back(response);
            } else {
                matTestSamples.push_back(sample);
                matTestResponses.push_back(response);
            }
        }
        svmTestData[i] = ml::TrainData::create(matTrainSamples, ml::ROW_SAMPLE, matTrainResponses);
        svmTrainData[i] = ml::TrainData::create(matTestSamples, ml::ROW_SAMPLE, matTestResponses);
    }

}
