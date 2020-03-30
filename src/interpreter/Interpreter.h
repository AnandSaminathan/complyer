#pragma once


#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>
#include <utility>
#include "interpreter/commands/CommandLtlspec.h"
#include "interpreter/commands/CommandSafetyspec.h"
#include "interpreter/commands/CommandBound.h"
#include "interpreter/commands/CommandTrace.h"
#include "interpreter/commands/CommandLength.h"
#include "interpreter/commands/CommandQuit.h"

class Interpreter {

  ltlBmc ltl_bmc_verifier;
  kInduction k_induction_verifier;
  Verifier *common_verifier = NULL;
  std::map<std::string, std::string> label_mapper;

  CommandLtlspec ltlspec;
  CommandSafetyspec safetyspec;
  CommandBound bound;
  CommandTrace trace;
  CommandLength length;
  CommandQuit quit;

public:

  Interpreter(const std::vector<Symbol>& symbols, const std::string& I, const std::string& T, std::map<std::string, std::string> labelMapper) :
    ltl_bmc_verifier(symbols, I, T),
    k_induction_verifier(symbols, I, T),
    label_mapper(std::move(labelMapper)),
    ltlspec(ltl_bmc_verifier,label_mapper),
    safetyspec(k_induction_verifier,label_mapper),
    bound(ltl_bmc_verifier),
    trace(*common_verifier),
    length(*common_verifier)
  {  }
  void interpret(const std::string& input);
};


