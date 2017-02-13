//
// Created by age2pierre on 01/02/17.
//

#include "../Common/Rois.h"
#include "../Common/Stat.hpp"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#define HUE_UPBOUND_NORM 255

using namespace std;
using namespace cv;

int main() {
    FileStorage configFs("../config.json", FileStorage::READ);
    string dataPath;
    configFs["dataPath"] >> dataPath;
    configFs.release();

    cout << "Retrieving data from files" << endl;
    FileStorage inFs(dataPath + "indexRois.json", FileStorage::READ);
    FileNodeIterator it = inFs["index"].begin();
    vector<Rois> collection;
    while (it != inFs["index"].end()) {
        collection.push_back(Rois(dataPath + "rois/" + to_string((int) *it) + ".json", dataPath));
        it++;
    }
    inFs.release();

    cout << "Collecting features & storing cloud points..." << endl;
    array<vector<float>, 8> perimeters;
    array<vector<float>, 8> circularities;
    array<vector<float>, 8> areas;
    array<vector<float>, 8> minHeights;
    array<vector<float>, 8> minWidths;
    array<vector<vector<float>>, 8> hueHists;
    FileStorage cldPtFs(dataPath + "reports/cldPt.json", FileStorage::WRITE);
    cldPtFs << "index" << "[";
    for (auto &roi : collection) {
        cldPtFs << "{";
        int index = roi.getClassId() - 1;
        cldPtFs << "class_id" << roi.getClassId();
        perimeters[index].push_back(roi.getPerimeter());
        cldPtFs << "perimeter" << roi.getPerimeter();
        circularities[index].push_back(roi.getCircularity());
        areas[index].push_back(roi.getArea());
        cldPtFs << "area" << roi.getArea();
        minHeights[index].push_back(roi.getMinRectHeight());
        cldPtFs << "minHeight" << roi.getMinRectHeight();
        minWidths[index].push_back(roi.getMinRectWidth());
        cldPtFs << "minWidth" << roi.getMinRectWidth();
        hueHists[index].push_back(roi.calcHueHist(180, HUE_UPBOUND_NORM));
        cldPtFs << "}";
    }
    cldPtFs << "]";
    cldPtFs.release();

    cout << "Calculating statistics ..." << endl;
    FileStorage statFs(dataPath + "reports/stats.json", FileStorage::WRITE);
    statFs << "index" << "[";
    FileStorage hgFs(dataPath + "reports/histgraph.json", FileStorage::WRITE);
    for (int i = 0; i < 8; i++) {
        double mean, stdev;
        statFs << "{";
        statFs << "class_id" << (i + 1);
        statFs << "population" << (int) perimeters[i].size();
        calcMeanAndStDev(perimeters[i], mean, stdev);
        statFs << "perimeter_mean" << mean;
        statFs << "perimeter_stdev" << stdev;
        calcMeanAndStDev(circularities[i], mean, stdev);
        statFs << "cirucularity_mean" << mean;
        statFs << "cirucularity_stdev" << stdev;
        calcMeanAndStDev(areas[i], mean, stdev);
        statFs << "area_mean" << mean;
        statFs << "area_stdev" << stdev;
        calcMeanAndStDev(minHeights[i], mean, stdev);
        statFs << "minHeight_mean" << mean;
        statFs << "minHeight_stdev" << stdev;
        calcMeanAndStDev(minWidths[i], mean, stdev);
        statFs << "minWidth_mean" << mean;
        statFs << "minWidth_stdev" << stdev;

        array<vector<float>, 180> arrayOfBeams;
        int j;
        hgFs << ("hist_of_" + to_string(i + 1)) << "[";
        for (auto &hueHist :  hueHists[i]) {
            j = 0;
            for (auto &hueBeam : hueHist)
                arrayOfBeams[j++].push_back(hueBeam);
        }
        j = 0;
        for (auto &vecOfBeam: arrayOfBeams) {
            calcMeanAndStDev(vecOfBeam, mean, stdev);
            hgFs << "{";
            hgFs << "beam_index" << (j++);
            hgFs << "beam_mean" << mean;
            hgFs << "beam_upper" << (((mean + stdev) > HUE_UPBOUND_NORM) ? HUE_UPBOUND_NORM : (mean + stdev));
            hgFs << "beam_lower" << (((mean - stdev) > 0.0) ? (mean - stdev) : 0.0);
            hgFs << "}";
        }
        hgFs << "]";
        statFs << "}";
    }
    statFs << "]";
    statFs.release();
    hgFs.release();

}