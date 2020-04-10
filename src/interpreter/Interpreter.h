#pragma once


#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>
#include <utility>
#include <commands/CommandBase.h>

class Interpreter {
  CommandBase command_base;
public:
  Interpreter(const std::vector<Symbol>& symbols, Kripke kripke, const std::map<std::string, std::string>& labelMapper) :
    command_base(symbols,std::move(kripke),labelMapper)
  {  }
  void interpret(const std::string& input);
};


