#pragma once

#include <string>
#include <map>
#include <vector>

template<class symbolType>
class SymbolTable {
  public:

    SymbolTable() {  };

    inline void addSymbol(std::string name, symbolType symbol) { 
      if(nameToIdx.find(name) != nameToIdx.end()) {
        throw std::invalid_argument("redeclaration of symbol: " + name);
      }
      symbols.push_back(symbol);
      nameToIdx[name] = symbols.size() - 1;
    }

    inline symbolType getSymbol(std::string name) {
      auto itr = nameToIdx.find(name);
      if(itr == nameToIdx.end()) {
        throw std::invalid_argument("symbol " + name + " not found");
      }
      return symbols[itr->second];
    }

    inline std::vector<symbolType> getSymbols() { return symbols;  }
    inline int getSymbolCount() { return symbols.size(); }

  private:
    
    std::map<std::string, int> nameToIdx;
    std::vector<symbolType> symbols;

};

