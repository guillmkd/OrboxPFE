//
// Created by age2pierre on 06/02/17.
//
#include "../Common/Rois.h"
#include "../Common/InputParser.h"
#include "ClassificationTest.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char **argv) {
    InputParser input(argc, argv);
    bool enP(input.cmdOptionExists("-perimeter")),
            enA(input.cmdOptionExists("-area")),
            enH(input.cmdOptionExists("-minHeight")),
            enW(input.cmdOptionExists("-minWidth"));

    int totBeam = stoi(input.getCmdOption("-totalBeam"));
    int truncBeam = stoi(input.getCmdOption("-truncBeam"));

    ClassificationTest test(enP, enA, enH, enW, totBeam, truncBeam, true);

    if(input.cmdOptionExists("-testknn")) {
        test.testKnn(9);
    }

    if(input.cmdOptionExists("-setknn")) {
        test.setKnn(9);
    }

    if(input.cmdOptionExists("-testsvm")) {
        test.testSvm(ml::SVM::C_SVC, ml::SVM::CHI2);
    }

    if(input.cmdOptionExists("-setsvm")) {
        test.setSvm(ml::SVM::C_SVC, ml::SVM::CHI2);
    }
}
