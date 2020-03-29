//
// Created by User on 29-03-2020.
//

#ifndef SMV_INTERPRETER_H
#define SMV_INTERPRETER_H


#include <verification-algorithms/ltl-bmc/ltl-bmc.hpp>
#include <verification-algorithms/k-induction/k-induction.hpp>
#include <utility>
#include "commandLtlspec.h"
#include "commandSafetyspec.h"
#include "commandBound.h"
#include "commandTrace.h"
#include "commandLength.h"
#include "commandQuit.h"

class Interpreter {
    ltlBmc ltl_bmc_verifier;
    kInduction k_induction_verifier;
    Verifier *common_verifier = NULL;
    std::map<std::string, std::string> label_mapper;

    commandLtlspec ltlspec;
    commandSafetyspec safetyspec;
    commandBound bound;
    commandTrace trace;
    commandLength length;
    commandQuit quit;
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


#endif //SMV_INTERPRETER_H
