#pragma once

#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>
#include <utility>
#include <commands/CommandBase.h>
#include <functional>

class Interpreter {
  CommandBase command_base;
  std::function<void(CommandResponse,CommandBase&)> print_function;
public:
  explicit Interpreter(CommandBase &commandBase): command_base(commandBase) {}
  void setPrinter(std::function<void(CommandResponse,CommandBase&)> printer){
    print_function = std::move(printer);
  }
  void interpret(const std::string& input) {
    CommandResponse command_response = command_base.perform(input);
    print_function(command_response,command_base);
  }
};


