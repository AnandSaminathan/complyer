#pragma once

#include "Module.hpp"
#include "Specification.hpp"

class NuSMV {
  public:

    NuSMV() { }

    inline void addModule(Module module) { modules.emplace_back(module); }
    inline void addSpecification(Specification spec) { specifications.emplace_back(spec); }

    inline std::vector<Module> getModules() { return modules; }
    inline std::vector<Specification> getSpecifications() { return specifications; }

    inline Kripke toFormula() { 
      assert(modules.size() == 1);
      return modules[0].toFormula();
    }

    inline std::vector<Symbol> getSymbols() {
      std::vector<Symbol> symbols;
      for(auto module : modules) {
        auto cur = module.getSymbols();
        symbols.insert(symbols.end(), cur.begin(), cur.end());
      }
      return symbols;
    }

    std::map<std::string, std::string> getMapping() {
      for(auto module : modules) {
        auto moduleMap = module.getMapping();
        substitutionMap.insert(moduleMap.begin(), moduleMap.end());
      }
      return substitutionMap;
    }


  private:

    std::vector<Module> modules;
    std::vector<Specification> specifications;
    std::map<std::string, std::string> substitutionMap;

};
