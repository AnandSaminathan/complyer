//
// Created by User on 29-03-2020.
//

#ifndef SMV_COMMANDQUIT_H
#define SMV_COMMANDQUIT_H

#include <stringConstants.h>
#include <algorithm>
#include <sstream>
#include "commandInterface.h"

class commandQuit : public commandInterface {
public:
    commandQuit() : commandInterface(stringConstants::QUIT) {}

    bool parse(std::string line) override {
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        std::transform(command.begin(),command.end(),command.begin(),::toupper);
        return (command == stringConstants::QUIT);
    }

    std::string perform() override {
        exit(0);
    }
};
#endif //SMV_COMMANDQUIT_H
