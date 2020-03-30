#pragma once

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

