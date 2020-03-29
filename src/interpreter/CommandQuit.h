//
// Created by User on 29-03-2020.
//

#ifndef SMV_COMMANDQUIT_H
#define SMV_COMMANDQUIT_H

#include <StringConstants.h>
#include <algorithm>
#include <sstream>
#include "CommandInterface.h"

class CommandQuit : public CommandInterface {
public:
    CommandQuit() : CommandInterface(StringConstants::QUIT) {}

    bool parse(std::string line) override {
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        std::transform(command.begin(),command.end(),command.begin(),::toupper);
        return (command == StringConstants::QUIT);
    }

    std::string perform() override {
        exit(0);
    }
};
#endif //SMV_COMMANDQUIT_H
