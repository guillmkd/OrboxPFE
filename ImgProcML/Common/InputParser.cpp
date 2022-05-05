//
// Created by age2pierre on 27/01/17.
//

#include <algorithm>
#include "InputParser.h"

using namespace std;

InputParser::InputParser(int &argc, char **argv) {
    for (int i = 1; i < argc; ++i)
        this->tokens.push_back(string(argv[i]));
}

const string &InputParser::getCmdOption(const string &option) const {
    std::vector<std::string>::const_iterator itr;
    itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()){
        return *itr;
    }
    return empty_string;
}

bool InputParser::cmdOptionExists(const std::string &option) const {
    return std::find(this->tokens.begin(), this->tokens.end(), option)
           != this->tokens.end();
}

