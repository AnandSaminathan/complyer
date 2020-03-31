#pragma once

#include "Functions.hpp"

class Assignment {
  public:

    Assignment() { }

    inline void addInit(Init init) { inits.emplace_back(init); }
    inline void addSeqNext(SeqNext seqNext) { seqNexts.emplace_back(seqNext); }
    inline void addConNext(ConNext conNext) { conNexts.emplace_back(conNext); }

    inline std::vector<Init> getInits() { return inits; }
    inline std::vector<SeqNext> getSeqNexts() { return seqNexts; }
    inline std::vector<ConNext> getConNexts() { return conNexts; }


  private:

    std::vector<Init> inits;
    std::vector<SeqNext> seqNexts;
    std::vector<ConNext> conNexts;
};
