#pragma once

#include <string>
#include <vector>


class SeqExpression {

};

class SeqSimpleExpr : SeqExpression {
  public:

    SeqSimpleExpr() { }

    inline void setFormula(std::string formula) { (this->formula) = formula; }
    inline std::string getFormula() { return formula; }

  private:

    std::string formula;
};

class SeqCaseSubExpr {
  public:

    SeqCaseSubExpr() { }

    inline void setAntecedent(std::string antecedent) { (this->antecedent) = antecedent; }
    inline void setConsequent(SeqExpression consequent) { (this->consequent) = consequent; }

    inline std::string getAntecedent() { return antecedent; }
    inline SeqExpression getConsequent() {  return consequent; }

  private:

    std::string antecedent;
    SeqExpression consequent;
};

class SeqCaseExpr : SeqExpression {
  public:

    SeqCaseExpr() { }

    inline void addSubExpression(SeqCaseSubExpr exp) { subExpressions.emplace_back(exp); }
    inline std::vector<SeqCaseSubExpr> getSubExpressions() { return subExpressions; } 

  private:

    std::vector<SeqCaseSubExpr> subExpressions;

};

class SeqIntervalExpr : SeqExpression {
  public:

    SeqIntervalExpr() { }

    inline void setFrom(std::string from) { (this->from) = from; }
    inline void setTo(std::string to) { (this->to) = to; }

    inline std::string getFrom() { return from; }
    inline std::string getTo() { return to; }

  private:

    std::string from;
    std::string to;
};

class SeqSetExpr : SeqExpression {
  public:

    SeqSetExpr() { }

    inline void addSubExpression(SeqExpression exp) { subExpressions.emplace_back(exp); }

    inline std::vector<SeqExpression> getSubExpressions() { return subExpressions; }

  private:

    std::vector<SeqExpression> subExpressions;
};


class ConExpression {
  public:

    ConExpression() { }

    inline void setAntecedent(std::string antecedent) { (this->antecedent) = antecedent; }
    inline void setLabel(std::string label) { (this->label) = label; }
    inline void setConsequent(SeqSetExpr consequent) { (this->consequent) = consequent; }

    inline std::string getAntecedent() { return antecedent; }
    inline std::string getLabel() { return label; }
    inline SeqSetExpr getConsequent() { return consequent; }

  private:

    std::string antecedent;
    std::string label;
    SeqSetExpr consequent;
};

