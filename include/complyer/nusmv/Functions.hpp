#pragma once

#include "Expression.hpp"

class Init {
  public:

    Init() { }
    Init(std::string id, SeqExpression definition) : id(id),
    definition(definition) { }

    inline void setId(std::string id) { (this->id) = id; }
    inline void setDefinition(SeqExpression definition) { (this->definition) = definition; }

    inline std::string getId() { return id; }
    inline SeqExpression getDefinition() { return definition; }

    std::string toFormulaString() {
      return definition.toFormulaString(id);
    }

  private:

    std::string id;
    SeqExpression definition;
};

class SeqNext {
  public:

    SeqNext() { }
    SeqNext(std::string id, SeqExpression definition) : id(id),
    definition(definition) { }

    inline void setId(std::string id) { (this->id) = id; } 
    inline void setDefinition(SeqExpression definition) { (this->definition) = definition; }

    inline std::string getId() { return id; }
    inline SeqExpression getDefinition() { return definition; }

    std::string toFormulaString() {
      return definition.toFormulaString(id, true);
    }

  private:

    std::string id;
    SeqExpression definition;
};


class ConNext {
  public:

    ConNext() { }
    ConNext(std::vector<std::string> ids, ConExpression definition) : ids(ids),
    definition(definition) { }

    inline void setIds(std::vector<std::string> ids) { (this->ids) = ids; }
    inline void setDefinition(ConExpression definition) { (this->definition) = definition; }

    inline std::vector<std::string> getIds() { return ids; }
    inline ConExpression getDefinition() { return definition; } 

    std::string toFormulaString() {
      return definition.toFormulaString(ids);
    }   

  private:

    std::vector<std::string> ids;
    ConExpression definition;
};
