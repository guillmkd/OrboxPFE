//
// Created by age2pierre on 31/01/17.
//

#ifndef PROJECT_SCREENSHOTS_H
#define PROJECT_SCREENSHOTS_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <set>
#include <iostream>
#include "Rois.h"


class Screenshots {
private:
    int selfID;
    std::string pathRawLightOn;
    std::string pathRawLightOff;
    std::string pathUndistLightOn;
    std::string pathUndistLightOff;
    std::string pathMaskBin;
    std::string dataPath;

    cv::Mat lightOn;
    cv::Mat lightOff;
    cv::Mat undistLightOn;
    cv::Mat undistLightOff;
    cv::Mat maskBin;

    std::set<int> roisChildId;
public:
    Screenshots(std::string dataPath, int id);
    Screenshots(cv::Mat rawLitOn,cv::Mat rawLitOff, cv::Mat undistOn, cv::Mat undistOff, std::string dataPath, int id);
    void readFromFile();
    void writeToFile(bool overwritePics);
    std::vector<Rois> segmentation();
};


#endif //PROJECT_SCREENSHOTS_H
