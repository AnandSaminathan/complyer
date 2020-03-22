#pragma once

#include <string>
#include <vector>
#include "verification-algorithms/common/symbol.hpp"

inline std::string land(std::string op1, std::string op2) { return (op1 + " && " + op2); }
inline std::string lor(std::string op1, std::string op2) { return (op1 + " || " + op2);  }
inline std::string limplies(std::string op1, std::string op2) { return (op1 + " -> " + op2);  }
inline std::string parenthesize(std::string op1) { return "(" + op1 + ")";  }
inline std::string leq(std::string op1, std::string op2) { return (op1 + " == " + op2); }
inline std::string lnot(std::string op1) { return "!" + op1; }
inline std::string lle(std::string op1, std::string op2) { return (op1 + "<=" + op2); }
inline std::string lge(std::string op1, std::string op2) { return (op1 + ">=" + op2); }

inline std::string atMostOne(std::vector<std::string> clauses) {
  std::string form = "";
  for(int i = 0; i < clauses.size(); ++i) {
    std::string clause = clauses[i];
    for(int j = 0; j < clauses.size(); ++j) {
      if(j != i) {
       clause = land(clause, lnot(clauses[j]));
      }
    }
    clause = parenthesize(clause);
    if(i == 0) form = clause;
    else form = lor(form, clause);
  }
  return form;
}

class ModelSpecification {
  public:

    ModelSpecification(std::string I, std::string T, std::string P) : I(I), T(T), P(P), ltl(false) {}
    ModelSpecification() : ltl(false) {}

    inline void setI(std::string I) { this->I = I; }
    inline void setT(std::string T) { this->T = T; }
    inline void setP(std::string P) { this->P = P; }
    inline void setLtl() { this->ltl = true; }
    inline void resetLtl() { this->ltl = false; }
    inline void setBound(int bound) { this->bound = bound; }

    inline std::string getI() { return I;  }
    inline std::string getT() { return T;  }
    inline std::string getP() { return P;  }
    inline std::map<std::string, std::string> getLabelMapper() { return labelMapper; }
    inline std::vector<Symbol> getSymbols() { return symbols;  }
    inline int getBound() { assert(ltl == true); return bound; }

    inline bool isLtl() { return ltl; }

    inline void addSymbol(Symbol s) { 
        symbols.push_back(s);
        symbolTable[s.getName()] = symbols.size() - 1;
    }

    inline type getTypeOfSymbol(std::string name) {
        auto it = symbolTable.find(name);
        assert(it != symbolTable.end());
        return symbols[it->second].getType();
    }

    inline Symbol getSymbol(std::string name) {
        auto it = symbolTable.find(name);
        assert(it != symbolTable.end());
        return symbols[it->second];
    }

    inline void addLabel(std::string label, std::string formula) {
        assert(labelMapper.find(label) == labelMapper.end());
        assert(!isPresent(label));
        labelMapper[label] = formula;
    }

    inline bool isPresent(std::string name) { return (symbolTable.find(name) != symbolTable.end()); }

  private:

    std::string I, T;
    std::string P;

    bool ltl;
    int bound;
    std::vector<Symbol> symbols;
    std::map<std::string, int> symbolTable;
    std::map<std::string, std::string> labelMapper;
};

