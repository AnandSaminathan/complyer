#pragma once


#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <utility>
#include <StringConstants.h>
#include "CommandInterface.h"

class CommandLtlspec : public CommandInterface {

  ltlBmc *ltl_bmc_verifier;
  std::map<std::string, std::string> label_mapper;
  std::string property;

public:

  CommandLtlspec(ltlBmc &a,std::map<std::string, std::string> labelMapper)
    :CommandInterface(StringConstants::LTLSPEC), label_mapper(std::move(labelMapper)) {
    ltl_bmc_verifier = &a;
  }

  bool parse(std::string line) override {
    std::stringstream ss(line);
    std::string command;
    ss >> command;
    std::transform(command.begin(),command.end(),command.begin(),::toupper);
    if(command != StringConstants::LTLSPEC) return false;
    std::getline(ss,property);
    if(property.empty()) throw std::invalid_argument("Property is empty");
    return true;
  }

  std::string perform() override {
    std::cout << "Checking: " << property << "\n";
    FormulaTree tree(property);
    tree.substitute(this->label_mapper);
    property = tree.getFormula();
    bool res = ltl_bmc_verifier->check(property);
    if(res) return "model satisfies the given property";
    else return "model does not satisfy the given property";
  }
};
