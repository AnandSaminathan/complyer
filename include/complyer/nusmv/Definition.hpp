#pragma once

#include <string>
#include <memory>
#include "Expression.hpp"

class Definition {
  public:

    Definition() { }
    Definition(std::string id, std::shared_ptr<SeqExpression> definition) : id(id),
    definition(definition) { }

    inline void setId(std::string id) { (this->id) = id; }
    inline void setDefinition(std::shared_ptr<SeqExpression> definition) { (this->definition) = definition; }

    inline std::string getId() { return id; }
    inline std::string getDefinition() { return definition->toFormulaString(""); }

  private:

    std::string id;
    std::shared_ptr<SeqExpression> definition;
};
