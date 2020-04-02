#pragma once

#include <string> 

class Kripke {
  public:

    Kripke() { }
    Kripke(std::string I, std::string T) : I(I),
    T(T) { }

    inline void setI(std::string I) { (this->I) = I; }
    inline void setT(std::string T) { (this->T) = T; }

    inline std::string getI() { return I; }
    inline std::string getT() { return T; } 

  private:

    std::string I;
    std::string T;
};