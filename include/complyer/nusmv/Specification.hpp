#pragma once

#include <string>
#include <optional>

enum propertyType { SAFETYSPEC, LTLSPEC };

class Specification {
  public:

    Specification() { }
    Specification(propertyType type, std::string property) : type(type),
    property(property) { }

    inline void setProperty(std::string property) { (this->property) = property; }
    inline void setType(propertyType type) { (this->type) = type; }
    inline void setBound(std::string val) { bound.emplace(val); }

    inline std::string getProperty() { return property; }
    inline propertyType getType() {  return type; }
    inline std::string getBound() { return bound.value_or("1000000"); }

  private:

    std::string property;
    propertyType type;
    std::optional<std::string> bound;
};
