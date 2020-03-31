#pragma once

#include "Expression.hpp"

class Init {
  public:

    Init() { }

    inline void setId(std::string id) { (this->id) = id; }
    inline void setDefinition(SeqExpression definition) { (this->definition) = definition; }

    inline std::string getId() { return id; }
    inline SeqExpression getDefinition() { return definition; }


  private:

    std::string id;
    SeqExpression definition;
};

class SeqNext {
  public:

    SeqNext() { }

    inline void setId(std::string id) { (this->id) = id; } 
    inline void setDefinition(SeqExpression definition) { (this->definition) = definition; }

    inline std::string getId() { return id; }
    inline SeqExpression getDefinition() { return definition; }

  private:

    std::string id;
    SeqExpression definition;
};


class ConNext {
  public:

    ConNext() { }

    inline void setIds(std::vector<std::string> ids) { (this->ids) = ids; }
    inline void setDefinition(ConExpression definition) { (this->definition) = definition; }

    inline std::vector<std::string> getIds() { return ids; }
    inline ConExpression getDefinition() { return definition; }    

  private:

    std::vector<std::string> ids;
    ConExpression definition;
};
