//
// Created by User on 29-03-2020.
//

#ifndef SMV_COMMANDTRACE_H
#define SMV_COMMANDTRACE_H

#include <verification-algorithms/common/verifier.hpp>
#include <stringConstants.h>
#include <algorithm>
#include "commandInterface.h"

class commandTrace : public commandInterface {
    Verifier *verifier;
public:
    explicit commandTrace(Verifier &v) : commandInterface(stringConstants::TRACE) {
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
        return (command == stringConstants::TRACE);
    }

    std::string perform() override {
        std::stringstream ss;
        ss << verifier->getTrace();
        return ss.str();
    }
};
#endif //SMV_COMMANDTRACE_H
