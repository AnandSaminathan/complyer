#pragma once

#include <verification-algorithms/common/symbol.hpp>
#include <utility>
#include <vector>
#include <complyer/util/Kripke.hpp>
#include <map>

class ModelSpecification {
  public:
    ModelSpecification()= default;
    ModelSpecification(std::vector<Symbol> s, Kripke k, std::map<std::string, std::string> lm)
      : symbols(std::move(s)), kripke(std::move(k)), label_mapper(std::move(lm)){};

    inline void setSymbols(std::vector<Symbol> s){ (this->symbols) = std::move(s); }
    inline void setKripke(Kripke k){ (this->kripke) = std::move(k); }
    inline void setLabelMapper(std::map<std::string, std::string> lm){ (this->label_mapper) = std::move(lm);}

    inline std::vector<Symbol> getSymbols(){ return symbols; }
    inline Kripke getKripke(){ return kripke; }
    inline std::map<std::string, std::string> getLabelMapper(){ return label_mapper; }
  private:
    std::vector<Symbol> symbols;
    Kripke kripke;
    std::map<std::string, std::string> label_mapper;
};