//
// Created by User on 29-03-2020.
//

#ifndef SMV_COMMANDLENGTH_H
#define SMV_COMMANDLENGTH_H

#include <stringConstants.h>
#include <verification-algorithms/common/verifier.hpp>
#include <algorithm>
#include "commandInterface.h"

class commandLength : public commandInterface {
    Verifier *verifier;
public:
    explicit commandLength(Verifier &v) : commandInterface(stringConstants::LENGTH) {
        verifier = &v;
    }

    void setVerifier(Verifier *x) {
        verifier = x;
    }

    bool parse(std::string line) override {
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        std::transform(command.begin(),command.end(),command.begin(),::toupper);
        return (command == stringConstants::LENGTH);
    }

    std::string perform() override {
        return std::to_string(verifier->getLength());
    }
};
#endif //SMV_COMMANDLENGTH_H
