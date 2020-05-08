#pragma once

#include <optional>
#include <memory>
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

    inline void makeCall(std::string callerName, std::vector<std::string> actualParameters) {
      (this->name) = callerName;
      assert((*parameters).size() == actualParameters.size());
      (this->actualParameters).emplace(actualParameters);
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

    inline void addCall(std::shared_ptr<Module> call) {
      assert((*call).getName() != name);
      calls.emplace_back(call);
    }

    inline void addDefinition(Definition definition) {
      definitions.emplace_back(definition);
    }

    inline std::string getName() { return name; }
    inline std::optional<std::vector<std::string>> getParameters() { return parameters; }
    inline std::optional<Assignment> getAssignment() { return assignment; }
    inline std::optional<std::string> getInit() { return init; }
    inline std::optional<std::string> getTrans() { return trans; }

    inline std::vector<Symbol> getSymbols() { 
      std::vector<Symbol> symbols;
      for(auto call: calls) {
        auto callSymbols = (*call).getSymbols();
        symbols.insert(symbols.end(), callSymbols.begin(), callSymbols.end());
      }
      auto cur = symbolTable.getSymbols();
      std::string prefix = (name != "main") ? name + "." : "";
      for(auto s: cur) {
        symbols.push_back(Symbol(s.getType(), prefix + s.getName()));
      }
      return symbols; 
    }

    Kripke toFormula() {
      std::string I = "";
      std::string T = "";

      assert(!(assignment && init) && !(assignment && trans));

      std::map<std::string, std::string> mapper;

      if(name != "main") {
        std::string prefix = name + ".";
        auto symbols = symbolTable.getSymbols();
        for(auto symbol: symbols) {
          std::string symbolName = symbol.getName();
          mapper[symbolName] = prefix + symbolName;
          mapper["next_" + symbolName] = "next_" + prefix + symbolName;
        }
      }

      if(parameters) {
        assert(actualParameters);
        for(int i = 0; i < (*parameters).size(); ++i) { 
          mapper[(*parameters)[i]] = (*actualParameters)[i]; 
        }
      }

      for(auto definition : definitions) { mapper[definition.getId()] = definition.getDefinition(); }
      
      for(auto call : calls) {
        Kripke k = (*call).toFormula();
        I = (I == "") ? k.getI() : land(I, k.getI());
        T = (T == "") ? k.getT() : land(T, k.getT());
      }
      
      if(init) { 
        I = (I == "") ? (*init) : land(I, (*init));
        T = (T == "") ? (*trans) : land(T, (*trans));
      } else if(assignment) { 
        Kripke k = assignment->toFormula();
        I = (I == "") ? k.getI() : land(I, k.getI());
        T = (T == "") ? k.getT() : land(T, k.getT());
      } 

      I = substitute(I, mapper);
      T = substitute(T, mapper);

      return Kripke(I, T);
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
    std::vector<std::shared_ptr<Module>> calls;
    std::map<std::string, std::string> substitutionMap;

    std::optional<Assignment> assignment;
    std::optional<std::string> init;
    std::optional<std::string> trans;
    std::vector<Definition> definitions;
};
