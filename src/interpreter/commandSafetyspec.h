//
// Created by User on 29-03-2020.
//

#ifndef SMV_COMMANDSAFETYSPEC_H
#define SMV_COMMANDSAFETYSPEC_H

#include <verification-algorithms/k-induction/k-induction.hpp>
#include <utility>
#include <stringConstants.h>
#include "commandInterface.h"

class commandSafetyspec : public commandInterface {
    kInduction *k_induction_verifier;
    std::map<std::string, std::string> label_mapper;
    std::string property;
public:
    commandSafetyspec(kInduction &a, std::map<std::string, std::string> labelMapper)
      :commandInterface(stringConstants::SAFETYSPEC), label_mapper(std::move(labelMapper)) {
        k_induction_verifier = &a;
    }

    bool parse(std::string line) override {
        std::stringstream ss(line);
        std::string command;
        ss >> command;
        std::transform(command.begin(),command.end(),command.begin(),::toupper);
        if(command != stringConstants::SAFETYSPEC) return false;
        std::getline(ss,property);
        if(property.empty()) throw std::invalid_argument("Property is empty");
        return true;
    }

    std::string perform() override {
        FormulaTree tree(property);
        tree.substitute(this->label_mapper);
        property = tree.getFormula();
        bool res = k_induction_verifier->check(property);
        if(res) return "model satisfies the given property";
        else return "model does not satisfy the given property";
    }
};

#endif //SMV_COMMANDSAFETYSPEC_H
