#pragma once

#include <string>
#include <optional>

enum propertyType { SAFETYSPEC, LTLSPEC };

class Specification {
  public:

    Specification() { }

    inline void setProperty(std::string property) { (this->property) = property; }
    inline void setType(propertyType type) { (this->type) = type; }
    inline void setBound(int val) { bound.emplace(val); }

    inline std::string getProperty() { return property; }
    inline propertyType getType() {  return type; }
    inline int getBound() { return bound.value_or(1000000); }

  private:

    std::string property;
    propertyType type;
    std::optional<int> bound;
};
