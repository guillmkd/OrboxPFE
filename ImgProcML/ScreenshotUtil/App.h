
#ifndef SCREENSHOTUTIL_APP_H
#define SCREENSHOTUTIL_APP_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgproc/imgproc_c.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <mutex>
#include <thread>
#include <chrono>
#include <pigpio.h>
#include <unistd.h>

constexpr unsigned int PWM_PIN = 18;
constexpr int deviceId = 0;

class App {
private:
    cv::VideoCapture videoCapture;
    cv::Mat camMap1, camMap2;

public:
    void initCamera();
    cv::Mat takeCroppedPicture(int x, int y, int width, int height);
    void initLight(); // Init Pigpio deamon (returns -1 in case of an error)
    void turnLightOn();
    void turnLightOff();
    void calibrateCamera();
    void undistord(cv::Mat &src, cv::Mat &dst);
    void close();
};


#endif //SCREENSHOTUTIL_APP_H
