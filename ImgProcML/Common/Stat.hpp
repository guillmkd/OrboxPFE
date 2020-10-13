//
// Created by age2pierre on 06/02/17.
//

#ifndef PROJECT_STAT_H
#define PROJECT_STAT_H

#include <numeric>
#include <vector>
#include <algorithm>
using namespace std;

void calcMeanAndStDev(vector<float> &in, double &mean, double &stdev) {
    double sum = accumulate(in.begin(), in.end(), 0.0);
    mean = sum / in.size();

    vector<double> diff(in.size());
    transform(in.begin(), in.end(), diff.begin(), [mean](double x) {
        return x - mean;
    });
    double sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    stdev = sqrt(sq_sum / in.size());
};

#endif //PROJECT_STAT_H
