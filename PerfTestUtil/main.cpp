//
// Created by age2pierre on 06/02/17.
//
#include "../Common/Rois.h"
#include "ClassificationTest.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <iostream>

using namespace std;
using namespace cv;



int main() {

    ClassificationTest test1;
    test1.createDataSet(true, true, true, true, 42, 10, true);
    test1.runSimpleKnn(9);
    cout << "C_SVC with CHI2 kernel" <<endl;
    test1.runSimpleSVM(ml::SVM::C_SVC, ml::SVM::CHI2);
}
