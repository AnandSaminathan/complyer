#pragma once

#include <string>
#include <vector>
#include "common/symbol.hpp"

inline std::string land(std::string op1, std::string op2) { return (op1 + " && " + op2); }
inline std::string lor(std::string op1, std::string op2) { return (op1 + " || " + op2);  }
inline std::string limplies(std::string op1, std::string op2) { return (op1 + " -> " + op2);  }
inline std::string parenthesize(std::string op1) { return "(" + op1 + ")";  }
inline std::string leq(std::string op1, std::string op2) { return (op1 + " == " + op2); }
inline std::string lnot(std::string op1) { return "!" + op1; }


class ModelSpecification {
  public:

    ModelSpecification(std::string I, std::string T, std::string P) : I(I), T(T), P(P) {}
    ModelSpecification() {}

    inline void setI(std::string I) { this->I = I; }
    inline void setT(std::string T) { this->T = T; }
    inline void setP(std::string P) { this->P = P; }

    inline std::string getI() { return I;  }
    inline std::string getT() { return T;  }
    inline std::string getP() { return P;  }
    inline std::vector<Symbol> getSymbols() { return symbols;  }

    inline void addSymbol(Symbol s) { symbols.push_back(s); }

  private:

    std::string I, T;
    std::string P;
    std::vector<Symbol> symbols;
};

