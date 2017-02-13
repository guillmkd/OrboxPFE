//
// Created by age2pierre on 31/01/17.
//

#include <string>
#include <iostream>
#include "Screenshots.h"
#include "IdProvider.hpp"

using namespace std;
using namespace cv;

void Screenshots::writeToFile(std::string path, bool overwritePics) {
    FileStorage fs(path, FileStorage::WRITE);
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
        string scrId = to_string(selfID);
        imwrite(dataPath + "pics/" + scrId + "RO.jpg", lightOn);
        imwrite(dataPath + "pics/" + scrId + "RF.jpg", lightOff);
        imwrite(dataPath + "pics/" + scrId + "UO.jpg", undistLightOn);
        imwrite(dataPath + "pics/" + scrId + "UF.jpg", undistLightOff);
        imwrite(dataPath + "pics/" + scrId + "MB.jpg", maskBin);
    }
}

void Screenshots::readFromFile(std::string path) {
    FileStorage fs(path, FileStorage::READ);
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
}

std::vector<Rois> Screenshots::segmentation() {
    Mat diff, blurred, bin, hsv;
    vector<Mat> hsv_split;
    vector<Rois> out;
    // subtract the two images, only objects lit by the LEDs will appear
    absdiff(lightOn, lightOff, diff);

    // once converted to HSV, only the value channel will be used
    // as it contained information on how much light a pixel get
    cvtColor(diff, hsv, CV_BGR2HSV);
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
    vector<vector<Point>> contours;
    findContours(bin(Rect(29, 37, 1035, 1006)), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    IdProvider idProvider(dataPath + "indexRois.json");
    for (vector<Point> contour : contours) {
        int roiId = idProvider.getNewId();
        Rect rect = boundingRect(Mat(contour));
        rect.x += 29;
        rect.y += 37;
        out.push_back(Rois(lightOn(rect),
                           bin(rect),
                           rect.x,
                           rect.y,
                           contour,
                           roiId
        ));
        roisChildId.insert(roiId);
    }
    return out;
}





