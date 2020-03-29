#pragma once

#include <string>
#include <z3++.h>

class Verifier {
  public:
    virtual int getLength() = 0;
    virtual bool check(std::string) = 0;
    virtual z3::model getTrace() = 0;
};
