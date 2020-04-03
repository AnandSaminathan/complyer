#pragma once

#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
  #include <utility>
  #include <StringConstants.h>
  #include "CommandInterface.h"

class CommandBound : public CommandInterface {

    ltlBmc *ltl_bmc_verifier;
    int bound;

  public:

    explicit CommandBound(ltlBmc &a) :CommandInterface(StringConstants::BOUND) {
      ltl_bmc_verifier = &a;
      bound = 100000;
    }

    bool parse(std::string line) override {
      std::stringstream ss(line);
      std::string command;
      ss >> command;
      std::transform(command.begin(),command.end(),command.begin(),::toupper);
      if(command != getOperation()) return false;
      if(ss>>bound) return true;
      else throw std::invalid_argument("Bound is empty");
    }

    std::string perform() override {
      ltl_bmc_verifier->setBound(bound);
      return std::string();
    }
};
