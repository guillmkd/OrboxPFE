//
// Created by age2pierre on 13/12/16.
//

#ifndef SCREENSHOTUTIL_APP_H
#define SCREENSHOTUTIL_APP_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <wiringPi.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <mutex>
#include <thread>
#include <chrono>


class App {
private:
    cv::VideoCapture videoCapture;
    int lightParam = 100;
    int deviceId = 0;
    cv::Mat camMap1, camMap2;

public:
    void initCamera();

    cv::Mat takeCroppedPicture(int x, int y, int width, int height);

    void initLight();

    void turnLightOn(){
        pwmWrite(1, lightParam);
    };

    void turnLightOff() {
        pwmWrite(1, 0);
    };

    void calibrateCamera();

    void undistord(cv::Mat &src, cv::Mat &dst);

    void close(){
        videoCapture.release();
    };
};


#endif //SCREENSHOTUTIL_APP_H
