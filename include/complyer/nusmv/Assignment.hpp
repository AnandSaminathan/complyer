#pragma once

#include "Functions.hpp"
#include "complyer/util/Kripke.hpp"
#include "complyer/util/PetriNet.hpp"

class Assignment {
  public:

    Assignment() { }

    inline void addInit(Init init) { inits.emplace_back(init); }
    inline void addSeqNext(SeqNext seqNext) { seqNexts.emplace_back(seqNext); }
    inline void addConNext(ConNext conNext) { conNexts.emplace_back(conNext); }

    inline std::vector<Init> getInits() { return inits; }
    inline std::vector<SeqNext> getSeqNexts() { return seqNexts; }
    inline std::vector<ConNext> getConNexts() { return conNexts; }

    PetriNet toPetriNet(std::vector<std::string> symbols) {
      assert(conNexts.size() && !seqNexts.size());

      int places = symbols.size();
      int transitions = conNexts.size();

      std::sort(symbols.begin(), symbols.end());
      std::map<std::string, int> symbolPos;
      for(int i = 0; i < symbols.size(); ++i) { symbolPos[symbols[i]] = i; } 


      std::vector<std::string> initial_marking(places, "0");
      std::vector<std::vector<std::string>> incidence_matrix(places, std::vector<std::string>(transitions, "0"));

      for(auto init: inits) {
        int idx = symbolPos[init.getId()];
        initial_marking[idx] = (init.getDefinition())->toFormulaString("");
      }

      for(int i = 0; i < transitions; ++i) {
        std::vector<std::string> column = conNexts[i].toColumn(); 
        std::vector<std::string> ids = conNexts[i].getIds();  
        for(int j = 0; j < ids.size(); ++j) {
          int idx = symbolPos[ids[j]];
          incidence_matrix[i][idx] = column[i];
        }
      }

      return PetriNet(initial_marking, incidence_matrix);
    }

    Kripke toFormula() {
      assert(!(seqNexts.size() && conNexts.size()));

      std::string I = "";
      for(auto init : inits) {
        if(I == "") {I = init.toFormulaString(); }
        else { I = land(I, init.toFormulaString()); }
      }

      std::string T = "";
      if(seqNexts.size()) {
        std::vector<std::string> clauses;
        for(auto seqNext : seqNexts) {
          clauses.emplace_back(seqNext.toFormulaString());
        }
        T = allTrue(clauses);
      } else {
        std::vector<std::string> clauses;
        for(auto conNext : conNexts)  {
          clauses.emplace_back(conNext.toFormulaString());
        }
        T = exactlyOneTruePb(clauses);
      }

      return Kripke(I, T);
    }

    std::map<std::string, std::string> getMapping() {
      std::map<std::string, std::string> labeToTransition;
      for(auto conNext : conNexts) {
        auto definition = conNext.getDefinition();
        labeToTransition[definition.getLabel()] = definition.getAntecedent();
      }
      return labeToTransition;
    }


  private:

    std::vector<Init> inits;
    std::vector<SeqNext> seqNexts;
    std::vector<ConNext> conNexts;
};

