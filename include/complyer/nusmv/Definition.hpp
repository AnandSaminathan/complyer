#pragma once

#include <string>

class Definition {
  public:

    Definition() { }
    Definition(std::string id, std::string definition) : id(id),
    definition(definition) { }

    inline void setId(std::string id) { (this->id) = id; }
    inline void setDefinition(std::string definition) { (this->definition) = definition; }

    inline std::string getId() { return id; }
    inline std::string getDefinition() { return definition; }

  private:

    std::string id;
    std::string definition;
};