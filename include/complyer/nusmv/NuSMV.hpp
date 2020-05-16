#pragma once

#include "Module.hpp"
#include "Specification.hpp"

class NuSMV {
  public:

    NuSMV() { }

    inline void addModule(Module module) { modules.addSymbol(module.getName(), module); }
    inline void addSpecification(Specification spec) { specifications.emplace_back(spec); }

    inline Module getModule(std::string module) { return modules.getSymbol(module);  }
    inline std::vector<Module> getModules() { return modules.getSymbols(); }
    inline std::vector<Specification> getSpecifications() { return specifications; }

    inline Kripke toFormula() { 
      return (modules.getSymbol("main")).toFormula();
    }

    inline std::vector<Symbol> getSymbols() {
      return (modules.getSymbol("main")).getSymbols();
    }

    std::map<std::string, std::string> getMapping() {
      auto mods = modules.getSymbols();
      for(auto module : mods) {
        auto moduleMap = module.getMapping();
        substitutionMap.insert(moduleMap.begin(), moduleMap.end());
      }
      return substitutionMap;
    }


  private:

    SymbolTable<Module> modules; 
    std::vector<Specification> specifications;
    std::map<std::string, std::string> substitutionMap;

};
