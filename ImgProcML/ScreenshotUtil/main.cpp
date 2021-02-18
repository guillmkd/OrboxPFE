#include <iostream>
#include <thread>
#include <string>
#include <math.h>
#include "App.h"
#include "../Common/InputParser.h"
#include "../Common/Screenshots.h"

using namespace std;
using namespace cv;


int main(int argc, char **argv) {
    try{
    InputParser input(argc, argv);
    int id;
    const std::string &argStrId = input.getCmdOption("-id");
    if (!argStrId.empty())
        id = stoi(argStrId);

    FileStorage configFs("../config.json", FileStorage::READ);
    string dataPath;
    configFs["dataPath"] >> dataPath;
    configFs.release();

    App myApp;
    myApp.calibrateCamera();
    myApp.initCamera();
    cerr << "INFO : init camera ok" << endl;
    //myApp.initLight();
    //myApp.turnLightOff();

    Mat rawLitOn, rawLitOff, undistOn, undistOff;
    //vector<thread> workers;

    //myApp.turnLightOn();
    rawLitOn = myApp.takeCroppedPicture(0, 0, 1280, 960);
    /*workers.push_back(thread([&]() {
        myApp.undistord(rawLitOn, undistOn);
    }));*/
    cerr << "INFO : rawLitOn ok" << endl;

    //myApp.turnLightOff();
/*    rawLitOff = myApp.takeCroppedPicture(0, 0, 1280, 960);
    workers.push_back(thread([&]() {
        myApp.undistord(rawLitOff, undistOff);
    }));
    cerr << "INFO : rawLitOff ok" << endl;
*/

    /*for_each(workers.begin(), workers.end(), [](thread &t) {
        t.join();
    });*/

/*
    rawLitOn = imread(dataPath + "pics/10502060RO.jpg");
    rawLitOff = imread(dataPath + "pics/10502060RF.jpg");
    undistOn = imread(dataPath + "pics/10502060UO.jpg");
    undistOff = imread(dataPath + "pics/10502060UF.jpg");
*/
    cerr << "INFO : init screenshot" << endl;
    //Screenshots screenshot(rawLitOn, rawLitOff, undistOn, undistOff, dataPath, id);
    Screenshots screenshot(rawLitOn, rawLitOn, undistOn, undistOn, dataPath, id);

    screenshot.segmentation();

    cerr << "INFO : segmentation ok" << endl;
    screenshot.writeToFile(true);
    cerr << "INFO : ecriture du fichier" << endl;

    myApp.close();
}
catch(Exception e)
{
    cerr << "ERREUR OPENCV : " << e.msg << endl;
}
    return 0;
}
