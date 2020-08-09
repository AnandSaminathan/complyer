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

    inline bool isPetriNet() {
      return (modules.getSymbolCount() == 1) && (modules.getSymbol("main").isConcurrent());
    }

    inline PetriNet toPetriNet() {
      if(modules.getSymbolCount() != 1) {
        throw std::logic_error("matrix representation can be built only for single module models");
      }
      return (modules.getSymbol("main")).toPetriNet();
    }

    inline std::vector<Symbol> getSymbols() {
      return (modules.getSymbol("main")).getSymbols();
    }

    std::map<std::string, std::string> getMapping() {
      return (modules.getSymbol("main")).getMapping();
    }


  private:

    SymbolTable<Module> modules; 
    std::vector<Specification> specifications;
    std::map<std::string, std::string> substitutionMap;

};
