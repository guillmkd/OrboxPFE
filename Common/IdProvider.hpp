//
// Created by age2pierre on 31/01/17.
//

#ifndef PROJECT_IDPROVIDER_H
#define PROJECT_IDPROVIDER_H

#include <set>
#include <string>
#include <fstream>


class IdProvider {
private:
    std::set<int> ids;
    std::string pathIndex;

public:
    IdProvider(std::string pathToIndex) {
        int i;
        pathIndex = pathToIndex;
        ids.insert(0);
        std::ifstream infile(pathIndex);
        while(infile >> i)
            ids.insert(i);
        infile.close();
    }

    int getNewId() {
        int rslt;
        do {
            rslt = 10000000 + (std::rand() % 10000000);
        } while (ids.find(rslt) != ids.end());
        std::ofstream outfile(pathIndex, std::ios_base::app);
        outfile << (std::to_string(rslt) + "\n");
        ids.insert(rslt);
        return rslt;
    };
};


#endif //PROJECT_IDPROVIDER_H
