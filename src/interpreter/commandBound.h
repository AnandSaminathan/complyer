//
// Created by User on 29-03-2020.
//

#ifndef SMV_COMMANDBOUND_H
#define SMV_COMMANDBOUND_H

#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <utility>
#include <stringConstants.h>
#include "commandInterface.h"

class commandBound : public commandInterface {
    ltlBmc *ltl_bmc_verifier;
    int bound;
public:
    explicit commandBound(ltlBmc &a) :commandInterface(stringConstants::BOUND) {
        ltl_bmc_verifier = &a;
        bound = 100000;
    }

    bool parse(std::string line) override {
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        std::transform(command.begin(),command.end(),command.begin(),::toupper);
        if(command != stringConstants::BOUND) return false;
        if(ss>>bound) return true;
        else throw std::invalid_argument("Bound is empty");
    }

    std::string perform() override {
        ltl_bmc_verifier->setBound(bound);
        return std::string();
    }
};
#endif //SMV_COMMANDBOUND_H
