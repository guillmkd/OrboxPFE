//
// Created by age2pierre on 31/01/17.
//

#include <string>
#include <iostream>
#include "Screenshots.h"
#include "IdProvider.hpp"

using namespace std;
using namespace cv;

void Screenshots::writeToFile(bool overwritePics) {
    FileStorage fs(dataPath + "/screenshots/" + to_string(selfID) + ".json", FileStorage::WRITE);
    fs << "self_id" << selfID;
    fs << "raw_lightOn" << pathRawLightOn;
    fs << "raw_lightOff" << pathRawLightOff;
    fs << "undist_lightOn" << pathUndistLightOn;
    fs << "undist_lightOff" << pathUndistLightOff;
    fs << "mask_bin" << pathMaskBin;
    fs << "rois_id" << "[";
    for (int id : roisChildId)
        fs << id;
    fs << "]";
    fs.release();
    if (overwritePics) {
        imwrite(dataPath + pathRawLightOn, lightOn);
        imwrite(dataPath + pathRawLightOff, lightOff);
        imwrite(dataPath + pathUndistLightOn, undistLightOn);
        imwrite(dataPath + pathUndistLightOff, undistLightOff);
        imwrite(dataPath + pathMaskBin, maskBin);
    }
}

void Screenshots::readFromFile() {
    FileStorage fs(dataPath + "/screenshots/" + to_string(selfID) + ".json",
                   FileStorage::READ);
    fs["self_id"] >> selfID;
    fs["raw_lightOn"] >> pathRawLightOn;
    fs["raw_lightOff"] >> pathRawLightOff;
    fs["undist_lightOn"] >> pathUndistLightOn;
    fs["undist_lightOff"] >> pathUndistLightOff;
    fs["mask_bin"] >> pathMaskBin;
    FileNode fn = fs["rois_id"];
    FileNodeIterator it = fn.begin();
    while (it != fn.end()) {
        roisChildId.insert((int) *it);
        it++;
    }
    lightOn = imread(dataPath + pathRawLightOn, IMREAD_COLOR);
    lightOff = imread(dataPath + pathRawLightOff, IMREAD_COLOR);
    undistLightOff = imread(dataPath + pathUndistLightOff, IMREAD_COLOR);
    undistLightOn = imread(dataPath + pathUndistLightOn, IMREAD_COLOR);
    maskBin = imread(dataPath + pathMaskBin, IMREAD_GRAYSCALE);
}

Screenshots::Screenshots(std::string path, int id) {
    selfID = id;
    dataPath = path;
    readFromFile();
}

Screenshots::Screenshots(cv::Mat rawLitOn, cv::Mat rawLitOff,
                         cv::Mat undistOn, cv::Mat undistOff,
                         std::string dataPath, int id) {
    selfID = id;
    auto strId = to_string(selfID);
    this->dataPath = dataPath;
    lightOn = rawLitOn;
    lightOff = rawLitOff;
    undistLightOff = undistOff;
    undistLightOn = undistOn;
    pathRawLightOn = "pics/" + strId + "RO.jpg";
    pathRawLightOff = "pics/" + strId + "RF.jpg";
    pathUndistLightOn = "pics/" + strId + "UO.jpg";
    pathUndistLightOff = "pics/" + strId + "UF.jpg";
    pathMaskBin = "pics/" + strId + "MB.jpg";
}

void Screenshots::segmentation() {
    Mat diff, blurred, bin, hsv;
    vector<Mat> hsv_split;
    // subtract the two images, only objects lit by the LEDs will appear
    absdiff(undistLightOn, undistLightOff, diff);

    // once converted to HSV, only the value channel will be used
    // as it contained information on how much light a pixel get
    cvtColor(diff, hsv, cv::COLOR_BGR2HSV);
    split(hsv, hsv_split);

    // blurring is used to get better result for thresholding
    blur(hsv_split[2], blurred, Size(15, 15));

    // thresholding applied to get a binary map
    threshold(blurred, bin, 0, 255, THRESH_TRIANGLE);

    // erosion and & dilation applied to clean the binary map
    erode(bin, bin, getStructuringElement(MORPH_RECT, Size(30, 30)));
    dilate(bin, bin, getStructuringElement(MORPH_RECT, Size(30, 30)));
    maskBin = bin;


    //find the bounding rectangle of each white spot and calculate outputs
    std::vector<std::vector<cv::Point>> contours;
    findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    IdProvider idProvider(dataPath + "/indexRois.json");
    cerr << "contour size : " << contours.size() << endl;
    for (vector<Point> contour : contours) {
        int roiId = idProvider.getNewId();
        Rect rect = boundingRect(Mat(contour));
        rect.x += 29;
        rect.y += 37;
        Rois roi(undistLightOn(rect),
                 bin(rect),
                 rect.x,
                 rect.y,
                 contour,
                 dataPath,
                 roiId
        );
        roi.writeToFile(true);
        roisChildId.insert(roiId);
    }
}







