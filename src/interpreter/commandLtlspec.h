//
// Created by User on 29-03-2020.
//

#ifndef SMV_COMMANDLTLSPEC_H
#define SMV_COMMANDLTLSPEC_H


#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <utility>
#include <stringConstants.h>
#include "commandInterface.h"

class commandLtlspec : public commandInterface {
    ltlBmc *ltl_bmc_verifier;
    std::map<std::string, std::string> label_mapper;
    std::string property;
public:
    commandLtlspec(ltlBmc &a,std::map<std::string, std::string> labelMapper)
      :commandInterface(stringConstants::LTLSPEC), label_mapper(std::move(labelMapper)) {
        ltl_bmc_verifier = &a;
    }

    bool parse(std::string line) override {
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        std::transform(command.begin(),command.end(),command.begin(),::toupper);
        if(command != stringConstants::LTLSPEC) return false;
        std::getline(ss,property);
        if(property.empty()) throw std::invalid_argument("Property is empty");
        return true;
    }

    std::string perform() override {
        FormulaTree tree(property);
        tree.substitute(this->label_mapper);
        property = tree.getFormula();
        bool res = ltl_bmc_verifier->check(property);
        if(res) return "model satisfies the given property";
        else return "model does not satisfy the given property";
    }
};
#endif //SMV_COMMANDLTLSPEC_H
