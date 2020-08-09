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

    inline bool isConcurrent() { return (assignment) && assignment->isConcurrent(); }

    inline void setName(std::string name) { (this->name) = name; }

    inline void setParameters(std::vector<std::string> parameters) { 
      (this->parameters).emplace(parameters); 
    }

    inline void makeCall(std::string callerName, std::vector<std::string> actualParameters, bool async = false) {
      (this->name) = callerName;
      (this->async) = async;
      if((*parameters).size() != actualParameters.size()) {
        throw std::invalid_argument("expected " + std::to_string((*parameters).size()) + " parameters but found " + std::to_string(actualParameters.size()) + " parameters");
      }
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
      if((*call).getName() == name) {
        throw std::logic_error("recursive module calls are not allowed");
      }
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

    inline bool isAsync() { return async; }

    inline std::string constantT() {
      std::string T = "";
      auto symbols = symbolTable.getSymbols();
      std::string prefix = name + ".";
      for(auto symbol : symbols) {
        std::string symbolName = prefix + symbol.getName();
        T = land(T, leq(nextId(symbolName), symbolName));
      }
      return T;
    }

    inline std::vector<std::string> getLocalSymbolNames() {
      std::vector<std::string> symbolNames = getSymbolNames(symbolTable.getSymbols());
      if(parameters) { symbolNames.insert(symbolNames.end(), (*parameters).begin(), (*parameters).end()); }
      return symbolNames;
    }

    inline std::vector<Symbol> getSymbols() { 
      std::vector<Symbol> symbols;
      for(auto call: calls) {
        auto callSymbols = (*call).getSymbols();
        symbols.insert(symbols.end(), callSymbols.begin(), callSymbols.end());
      }
      auto cur = symbolTable.getSymbols();
      std::string prefix;
      if(name != "main") { prefix = name + ".";  }
      for(auto symbol: cur) {
        symbols.push_back(Symbol(symbol.getType(), prefix + symbol.getName()));
      }
      std::sort(symbols.begin(), symbols.end(), [](Symbol a, Symbol b) {
        return a.getName() < b.getName();  
      });
      return symbols; 
    }

    Kripke toFormula() {
      std::string I = "";
      std::string T = "";

      std::vector<std::string> asyncT, asyncConstT;

      if((assignment && init) || (assignment && trans)) {
        throw std::logic_error("ambiguous model - multiple formats for definition");
      }

      std::map<std::string, std::string> mapper;

      if(name != "main") {
        std::string prefix = name + ".";
        auto symbols = symbolTable.getSymbols();
        for(auto symbol: symbols) {
          std::string symbolName = symbol.getName();
          mapper[symbolName] = prefix + symbolName;
          mapper[nextId(symbolName)] = nextId(prefix + symbolName);
        }
      }

      if(parameters) {
        if(!actualParameters) { throw std::invalid_argument("missing parameters for module call"); }
        for(int i = 0; i < (*parameters).size(); ++i) { 
          mapper[(*parameters)[i]] = (*actualParameters)[i]; 
          mapper[nextId((*parameters)[i])] = nextId((*actualParameters)[i]);
        }
      }

      for(auto definition : definitions) { mapper[definition.getId()] = definition.getDefinition(); }
      
      for(int i = 0; i < calls.size(); ++i) {
        auto call = calls[i];
        Kripke k = call->toFormula();
        I = land(I, k.getI());
        if(call->isAsync() == false) {
          T = land(T, k.getT());
        } else {
          asyncT.push_back(k.getT());
          asyncConstT.push_back(parenthesize(call->constantT()));
        }  
      }
      
      if(init) { 
        I = land(I, (*init));
        T = land(T, (*trans));
      } else if(assignment) { 
        Kripke k = assignment->toFormula();
        I = land(I, k.getI());
        T = land(T, k.getT());
      } 

      if(asyncT.size()) {
        std::vector<std::string> clauses;
        for(int i = 0; i < asyncT.size(); ++i) {
          std::string clause = asyncT[i]; 
          for(int j = 0; j < asyncConstT.size(); ++j) {
            if(i != j) {
              clause = land(clause, asyncConstT[j]);
            } 
          }
          clauses.emplace_back(parenthesize(clause));
        }
        T = land(T, exactlyOneTruePb(clauses));
      }

      I = substitute(I, mapper);
      T = substitute(T, mapper);

      return Kripke(I, T);
    }

    std::map<std::string, std::string> getMapping() {
      std::map<std::string, std::string> substitutionMap;
      for(auto call : calls) {
        auto callMap = call->getMapping();
        substitutionMap.insert(callMap.begin(), callMap.end()); 
      }
      std::map<std::string, std::string> curMap;
      if(assignment) { curMap = assignment->getMapping(); }
      else { substitutionMap = std::map<std::string, std::string>(); }
      for(auto definition : definitions) {
        curMap[definition.getId()] = definition.getDefinition();
      }
      if(name != "main") { 
        std::string prefix = name + "."; 
        for(auto m : curMap) { substitutionMap[prefix + m.first] = m.second; }
      } else { substitutionMap.insert(curMap.begin(), curMap.end()); }
      return substitutionMap;
    }

    PetriNet toPetriNet() {
      if(!assignment) {
        throw std::logic_error("cannot build matrix representation without function based definitions");
      }
      return (assignment->toPetriNet(getSymbolNames(getSymbols())));
    }

  private:

    std::string name;
    bool async{false};
    std::optional<std::vector<std::string>> parameters, actualParameters;

    SymbolTable<Symbol> symbolTable;
    std::vector<std::shared_ptr<Module>> calls;
    std::map<std::string, std::string> substitutionMap;

    std::optional<Assignment> assignment;
    std::optional<std::string> init;
    std::optional<std::string> trans;
    std::vector<Definition> definitions;
};
