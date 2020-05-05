#pragma once

#include <optional>
#include "Assignment.hpp"
#include "Definition.hpp"
#include "verification-algorithms/common/symbol.hpp"
#include "complyer/util/SymbolTable.hpp"

class Module {
  public:

    Module() { }

    inline void setName(std::string name) { (this->name) = name; }

    inline void setParameters(std::vector<std::string> parameters) { 
      (this->parameters).emplace(parameters); 
    }

    inline void setActualParameters(std::vector<std::string> actualParameters) {
      assert((*parameters).size() == actualParameters.size());
      (this->actualParameters).emplace(actualParameters);
    }

    inline void setVariablePrefix(std::string prefix) {
      for(auto& symbol: symbols) {
        symbol.setName(prefix + "." + symbol.getName());
      }
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
      symbolTable.addSymbol(symbol.getName(), symbol);
    }

    inline void addDefinition(Definition definition) {
      definitions.emplace_back(definition);
    }

    inline std::string getName() { return name; }
    inline std::optional<std::vector<std::string>> getParameters() { return parameters; }
    inline std::optional<Assignment> getAssignment() { return assignment; }
    inline std::optional<std::string> getInit() { return init; }
    inline std::optional<std::string> getTrans() { return trans; }
    inline std::vector<Symbol> getSymbols() { return symbolTable.getSymbols(); }

    Kripke toFormula() {
      std::string I = "";
      std::string T = "";

      assert(!(assignment && init) && !(assignment && trans));

      std::map<std::string, std::string> mapper;
      for(int i = 0; i < (*parameters).size(); ++i) { mapper[(*parameters)[i]] = (*actualParameters)[i]; }
      for(auto definition : definitions) { mapper[definition.getId()] = definition.getDefinition(); }
      
      if(init) { 
        (*init) = substitute((*init), mapper);
        (*trans) = substitute((*trans), mapper);
        return Kripke(*init, *trans); 
      } else { 
        Kripke kripke = assignment->toFormula();
        kripke.setI(substitute(kripke.getI(), mapper));
        kripke.setT(substitute(kripke.getT(), mapper));
        return kripke;
      } 
    }

    std::map<std::string, std::string> getMapping() {
      if(assignment) { substitutionMap = assignment->getMapping(); }
      else { substitutionMap = std::map<std::string, std::string>(); }
      for(auto definition : definitions) {
        substitutionMap[definition.getId()] = definition.getDefinition();
      }
      return substitutionMap;
    }

  private:

    std::string name;
    std::optional<std::vector<std::string>> parameters, actualParameters;
    SymbolTable<Symbol> symbolTable;
    std::optional<Assignment> assignment;
    std::optional<std::string> init;
    std::optional<std::string> trans;
    std::vector<Definition> definitions;
    std::map<std::string, std::string> substitutionMap;

};
