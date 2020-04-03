#pragma once

#include <verification-algorithms/common/verifier.hpp>
#include <StringConstants.h>
#include <algorithm>
#include "CommandInterface.h"

class CommandTrace : public CommandInterface {

    Verifier *verifier;

public:

  explicit CommandTrace(Verifier &v) : CommandInterface(StringConstants::TRACE) {
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
    return (command == getOperation());
  }

  std::string perform() override {
    std::stringstream ss;
    ss << verifier->getTrace();
    return ss.str();
  }
};
