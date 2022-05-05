//
// Created by age2pierre on 31/01/17.
//

#ifndef PROJECT_IDPROVIDER_H
#define PROJECT_IDPROVIDER_H

#include <set>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

class IdProvider {
private:
    std::set<int> ids;
    std::string pathIndex;

public:
    IdProvider(std::string pathToIndex) {
        pathIndex = pathToIndex;
        cv::FileStorage fs(pathIndex, cv::FileStorage::READ);
        cv::FileNode features = fs["index"];
        cv::FileNodeIterator it = features.begin(), it_end = features.end();
        for( ; it != it_end; ++it)
            ids.insert(((int)(*it)));
        fs.release();
    }

    int getNewId() {
        int rslt;
        do {
            rslt = 10000000 + (std::rand() % 10000000);
        } while (ids.find(rslt) != ids.end());
        ids.insert(rslt);
        cv::FileStorage fs(pathIndex, cv::FileStorage::WRITE);
        fs << "index" << "[";
        for(int id : ids)
            fs << id;
        fs << "]";
        fs.release();
        return rslt;
    };
};


#endif //PROJECT_IDPROVIDER_H
