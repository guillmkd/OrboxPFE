#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <math.h>
#include "App.h"
#include "../Common/InputParser.h"
#include "../Common/Screenshots.h"
#include "../Common/Rois.h"

using namespace std;
using namespace cv;


int main(int argc, char **argv) {
    InputParser input(argc, argv);

    FileStorage configFs("../config.json", FileStorage::READ);
    string dataPath;
    configFs["dataPath"] >> dataPath;
    configFs.release();

    App myApp;
    myApp.calibrateCamera();
    myApp.initCamera();
    myApp.initLight();
    myApp.turnLightOff();


    Mat rawLitOn, rawLitOff, undistOn, undistOff;
    vector<thread> workers;

    myApp.turnLightOn();
    rawLitOn = myApp.takeCroppedPicture(428, 13, 1100, 1067);
    workers.push_back(thread([&]() {
//        imwrite(dataPath + "pics/" + rawId + "_lightOn.jpg", rawLitOn);
        myApp.undistord(rawLitOn, undistOn);
//        imwrite(dataPath + "pics/" + rawId + "_lightOn.jpg", undistOn);
    }));

    myApp.turnLightOff();
    rawLitOff = myApp.takeCroppedPicture(428, 13, 1100, 1067);
    workers.push_back(thread([&]() {
//        imwrite("./pics/raw/" + rawId + "_lightOff.jpg", rawLitOff);
        myApp.undistord(rawLitOff, undistOff);
//        imwrite("./pics/undist/" + rawId + "_lightOff.jpg", undistOff);
    }));

    for_each(workers.begin(), workers.end(), [](thread &t) {
        t.join();
    });
    // myApp.segmentation(undistOn, undistOff, rois);

    // TODO make screenshotobj and call segmentation
    myApp.close();
    return 0;
}

