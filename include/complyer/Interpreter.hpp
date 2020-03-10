#pragma once

#include <string>
#include <vector>
#include "verification-algorithms/k-induction/k-induction.hpp"
#include "verification-algorithms/ltl-bmc/ltl-bmc.hpp"

class Interpreter {
  public:

    Interpreter(std::vector<Symbol> symbols, std::string I, std::string T) : ltlBmcVerifier(symbols, I, T), kInductionVerifier(symbols, I, T) {  }

    void interpret(std::string);

  private:

    ltlBmc ltlBmcVerifier;
    kInduction kInductionVerifier;

    void verifyLtlSpec(std::string);
    void verifySafetySpec(std::string);
    void setLtlBound(int);
    bool handleRejection();
    
    inline void quit() { exit(0); }
};
