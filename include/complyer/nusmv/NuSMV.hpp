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

  private:

    std::vector<Module> modules;
    std::vector<Specification> specifications;

};
