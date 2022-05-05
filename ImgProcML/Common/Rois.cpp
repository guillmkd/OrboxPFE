//
// Created by age2pierre on 30/01/17.
//

#include "Rois.h"

using namespace std;
using namespace cv;

void Rois::readFromFile() {
    FileStorage fs(dataPath + "rois/" + to_string(selfId) + ".json", FileStorage::READ);
    fs["self_id"] >> selfId;
    fs["parent_id"] >> parentId;
    fs["bound_rect_height"] >> boundRectHeight;
    fs["bound_rect_width"] >> boundRectWidth;
    fs["bound_rect_x"] >> boundRectX;
    fs["bound_rect_y"] >> boundRectY;
    fs["color_pic_path"] >> colorPicPath;
    fs["mask_pic_path"] >> maskPicPath;
    fs["class_id"] >> classId;
    fs.release();

    colorMat = imread(dataPath + colorPicPath, IMREAD_COLOR);
    maskMat = imread(dataPath + maskPicPath, IMREAD_GRAYSCALE);
}

void Rois::writeToFile(bool overwritePics) {
    FileStorage fs(dataPath + "rois/" + to_string(selfId) + ".json", FileStorage::WRITE);
    fs << "self_id" << selfId;
    fs << "parent_id" << parentId;
    fs << "bound_rect_height" << boundRectHeight;
    fs << "bound_rect_width" << boundRectWidth;
    fs << "bound_rect_x" << boundRectX;
    fs << "bound_rect_y" << boundRectY;
    fs << "color_pic_path" << colorPicPath;
    fs << "mask_pic_path" << maskPicPath;
    fs << "class_id" << classId;
    fs.release();
    if (overwritePics) {
        imwrite(dataPath + colorPicPath, colorMat);
        imwrite(dataPath + maskPicPath, maskMat);
    }
}

Rois::Rois(string argDataPath, int id) {
    dataPath = argDataPath;
    selfId = id;
    readFromFile();
}

Rois::Rois(cv::Mat color, cv::Mat mask, int X, int Y, std::vector<cv::Point> contour, string dataPath, int selfId) {
    this->dataPath = dataPath;
    colorMat = color;
    maskMat = mask;
    boundRectX = X;
    boundRectY = Y;
    this->contour = contour;
    this->selfId = selfId;
    colorPicPath = "pics/" + to_string(selfId) + "CL.png";
    maskPicPath = "pics/" + to_string(selfId) + "MK.png";
}

float Rois::getPerimeter() {
    if (contourPerimeter < 0)
        contourPerimeter = (float) cv::arcLength(getContour(), true);
    return contourPerimeter;
}

float Rois::getArea() {
    if (contourArea < 0)
        contourArea = (float) cv::contourArea(getContour());
    return contourArea;
}

float Rois::getCircularity() {
    if (contourCircularity < 0) {
        float perimeter = getPerimeter();
        contourCircularity = (float) ((4 * M_PI * getArea()) / (perimeter * perimeter));
    }
    return contourCircularity;
}

vector<float> Rois::calcHueHist(int numberOfBar, double normUpBound) {
    Mat hsvRoi, hist, rslt, rsltNorm;
    float hueRanges[] = {0, 180};
    const float *ranges[] = {hueRanges};
    int channels[] = {0};
    int histSize[] = {numberOfBar};

    cvtColor(colorMat, hsvRoi, cv::COLOR_BGR2HSV);
    calcHist(&hsvRoi, 1, channels, maskMat, hist, 1, histSize, ranges);

    rslt = hist.t();
    transpose(hist, rslt);
    normalize(rslt, rsltNorm, normUpBound, 0.0, NORM_MINMAX);
    vector<float> outVec;
    MatToVector(rsltNorm, outVec);
    return outVec;
}

std::vector<cv::Point> Rois::getContour() {
    if (contour.empty()) {
        vector<vector<Point>> contours;
        Mat maskWithBorder, bin;
        copyMakeBorder(maskMat, maskWithBorder,
                       5, 5, 5, 5,
                       BORDER_CONSTANT, Scalar(0));
        threshold(maskWithBorder, bin, 128, 255, THRESH_BINARY);
        findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        int index = 0;
        if (contours.size() > 1) {
            double maxArea = -1;
            int i = 0;
            for (auto &it : contours) {
                if (maxArea < cv::contourArea(it)) {
                    maxArea = cv::contourArea(it);
                    index = i;
                }
                i++;
            }
        }
        contour = contours[index];
    }
    return contour;
}

// Convert a 1-channel Mat<float> object to a vector.
void Rois::MatToVector(const Mat &in, vector<float> &out) {
    if (in.isContinuous()) { out.assign((float *) in.datastart, (float *) in.dataend); }
    else {
        for (int i = 0; i < in.rows; ++i)
            out.insert(out.end(), in.ptr<float>(i), in.ptr<float>(i) + in.cols);
    }
    return;
}

float Rois::getMinRectHeight() {
    if (minH < 0)
        minH = minAreaRect(getContour()).size.height;
    return minH;
}

float Rois::getMinRectWidth() {
    if (minW < 0)
        minW = minAreaRect(getContour()).size.width;
    return minW;
}

























