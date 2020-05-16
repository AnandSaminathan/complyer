#pragma once

#include <string>
#include <map>
#include <vector>

template<class symbolType>
class SymbolTable {
  public:

    SymbolTable() {  };

    inline void addSymbol(std::string name, symbolType symbol) { 
      assert(nameToIdx.find(name) == nameToIdx.end());
      symbols.push_back(symbol);
      nameToIdx[name] = symbols.size() - 1;
    }

    inline symbolType getSymbol(std::string name) {
      auto itr = nameToIdx.find(name);
      assert(itr != nameToIdx.end());
      return symbols[itr->second];
    }

    inline std::vector<symbolType> getSymbols() { return symbols;  }

  private:
    
    std::map<std::string, int> nameToIdx;
    std::vector<symbolType> symbols;

};

