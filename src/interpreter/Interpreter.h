#pragma once


#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>
#include <utility>
#include <commands/CommandBase.h>

class Interpreter {
  CommandBase command_base;
public:
  explicit Interpreter(CommandBase commandBase) :
    command_base(std::move(commandBase))
  {  }
  void interpret(const std::string& input);
};


