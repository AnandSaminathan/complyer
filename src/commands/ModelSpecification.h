#pragma once

#include <utility>
#include <vector>
#include <map>

#include <verification-algorithms/common/symbol.hpp>

#include <complyer/util/Kripke.hpp>
#include "complyer/util/PetriNet.hpp"

class ModelSpecification {
  public:
    ModelSpecification()= default;
    ModelSpecification(std::vector<Symbol> s, Kripke k, std::map<std::string, std::string> lm)
      : symbols(std::move(s)), kripke(std::move(k)), label_mapper(std::move(lm)){};

    inline void setSymbols(std::vector<Symbol> s){ (this->symbols) = std::move(s); }
    inline void setKripke(Kripke k){ (this->kripke) = std::move(k); }
    inline void setLabelMapper(std::map<std::string, std::string> lm){ (this->label_mapper) = std::move(lm);}
    inline void setPetriNet(PetriNet p) { (this->petrinet) = std::move(p); }
    inline void setConcurrent(bool concurrent) { (this->concurrent) = concurrent; }

    inline std::vector<Symbol> getSymbols(){ return symbols; }
    inline Kripke getKripke(){ return kripke; }
    inline PetriNet getPetriNet() { return petrinet; }
    inline bool isConcurrent() { return concurrent; }
    inline std::map<std::string, std::string> getLabelMapper(){ return label_mapper; }
  private:
    bool concurrent;
    std::vector<Symbol> symbols;
    Kripke kripke;
    PetriNet petrinet;
    std::map<std::string, std::string> label_mapper;
};

