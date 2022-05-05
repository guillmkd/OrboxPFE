#ifndef COMMON_INPUTPARSER_H
#define COMMON_INPUTPARSER_H

#include <string>
#include <vector>

class InputParser {
private:
    std::vector<std::string> tokens;
    std::string empty_string;

public:
    InputParser(int &argc, char **argv);

    /// @author iain
    const std::string &getCmdOption(const std::string &option) const;

    /// @author iain
    bool cmdOptionExists(const std::string &option) const;

};

#endif //COMMON_INPUTPARSER_H
