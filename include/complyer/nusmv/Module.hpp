#pragma once

#include <optional>
#include "Assignment.hpp"
#include "Definition.hpp"
#include "verification-algorithms/common/symbol.hpp"

class Module {
  public:

    Module() { }

    inline void setName(std::string name) { (this->name) = name; }

    inline void setParameters(std::vector<std::string> parameters) { 
      (this->parameters).emplace(parameters); 
    }

    inline void setAssignment(Assignment assignment) { 
      (this->assignment).emplace(assignment); 
    }

    inline void setInit(std::string init) {
      (this->init).emplace(init);
    } 

    inline void setTrans(std::string trans) {
      (this->trans).emplace(trans);
    }

    inline void addSymbol(Symbol symbol) {
      symbols.emplace_back(symbol);
    }

    inline void addDefinition(Definition definition) {
      definitions.emplace_back(definition);
    }

    inline std::string getName() { return name; }
    inline std::optional<std::vector<std::string>> getParameters() { return parameters; }
    inline std::optional<Assignment> getAssignment() { return assignment; }
    inline std::optional<std::string> getInit() { return init; }
    inline std::optional<std::string> getTrans() { return trans; }
    inline std::vector<Symbol> getSymbols() { return symbols; }

    Kripke toFormula() {
      std::string I = "";
      std::string T = "";

      assert(!(assignment && init) && !(assignment && trans));

      if(init) { return Kripke(*init, *trans); }
      else { return assignment->toFormula(); } 
    }

  private:

    std::string name;
    std::optional<std::vector<std::string>> parameters;
    std::vector<Symbol> symbols;
    std::optional<Assignment> assignment;
    std::optional<std::string> init;
    std::optional<std::string> trans;
    std::vector<Definition> definitions;

};
