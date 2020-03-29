//
// Created by User on 29-03-2020.
//

#pragma once

#include <StringConstants.h>
#include <verification-algorithms/common/verifier.hpp>
#include <algorithm>
#include "CommandInterface.h"

class CommandLength : public CommandInterface {

  Verifier *verifier;

public:

  explicit CommandLength(Verifier &v) : CommandInterface(StringConstants::LENGTH) {
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
    return (command == StringConstants::LENGTH);
  }

  std::string perform() override {
    return std::to_string(verifier->getLength());
  }
};
