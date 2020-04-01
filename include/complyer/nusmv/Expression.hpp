#pragma once

#include <string>
#include <vector>
#include "complyer/util/FormulaStringUtil.hpp"


class SeqExpression {
  public:
    virtual std::string toFormulaString(std::string, bool = false);
};

class SeqSimpleExpr : public SeqExpression {
  public:

    SeqSimpleExpr() { }
    SeqSimpleExpr(std::string formula) : formula(formula) { }

    inline void setFormula(std::string formula) { (this->formula) = formula; }
    inline std::string getFormula() { return formula; }

    std::string toFormulaString(std::string id, bool isNext = false) override {
      if(!isNext) { return parenthesize(leq(id, formula)); }
      else { return parenthesize(leq(nextId(id), formula)); }
    }

  private:

    std::string formula;
};

class SeqCaseSubExpr {
  public:

    SeqCaseSubExpr() { }
    SeqCaseSubExpr(std::string antecedent, SeqExpression consequent) : antecedent(antecedent),
    consequent(consequent) { }

    inline void setAntecedent(std::string antecedent) { (this->antecedent) = antecedent; }
    inline void setConsequent(SeqExpression consequent) { (this->consequent) = consequent; }

    inline std::string getAntecedent() { return antecedent; }
    inline SeqExpression getConsequent() {  return consequent; }

    std::string toFormulaString(std::string id, bool isNext = false) {
      return parenthesize(limplies(antecedent, consequent.toFormulaString(nextId(id))));
    }

  private:

    std::string antecedent;
    SeqExpression consequent;
};

class SeqCaseExpr : public SeqExpression {
  public:

    SeqCaseExpr() { }

    inline void addSubExpression(SeqCaseSubExpr exp) { subExpressions.emplace_back(exp); }
    inline std::vector<SeqCaseSubExpr> getSubExpressions() { return subExpressions; } 

    std::string toFormulaString(std::string id, bool isNext = false) override {
      std::string formula = "";
      for(auto expression : subExpressions) {
        if(formula == "") { formula = expression.toFormulaString(id); }
        else { formula = lor(formula, expression.toFormulaString(id)); }
      }
      return parenthesize(formula);
    }

  private:

    std::vector<SeqCaseSubExpr> subExpressions;

};

class SeqIntervalExpr : public SeqExpression {
  public:

    SeqIntervalExpr() { }
    SeqIntervalExpr(std::string from, std::string to) : from(from),
    to(to) { }

    inline void setFrom(std::string from) { (this->from) = from; }
    inline void setTo(std::string to) { (this->to) = to; }

    inline std::string getFrom() { return from; }
    inline std::string getTo() { return to; }

    std::string toFormulaString(std::string id, bool isNext = false) override {
      if(!isNext) { return parenthesize(land(lge(id, from), lle(id, to))); }
      else { return parenthesize(land(lge(nextId(id), from), lle(nextId(id), to))); } 
    }

  private:

    std::string from;
    std::string to;
};

class SeqSetExpr : public SeqExpression {
  public:

    SeqSetExpr() { }

    inline void addSubExpression(SeqExpression exp) { subExpressions.emplace_back(exp); }

    inline std::vector<SeqExpression> getSubExpressions() { return subExpressions; }

    std::string toFormulaString(std::string id, bool isNext = false) override {
      if(isNext){ id = nextId(id); }

      std::string formula = "";
      for(auto expression : subExpressions) {
        if(formula == "") { formula = expression.toFormulaString(id); }
        else { formula = lor(formula, expression.toFormulaString(id)); }
      }
      return parenthesize(formula);
    }

  private:

    std::vector<SeqExpression> subExpressions;
};


class ConExpression {
  public:

    ConExpression() { }
    ConExpression(std::string antecedent, std::string label, SeqSetExpr consequent) :
    antecedent(antecedent),
    label(label),
    consequent(consequent) { }

    inline void setAntecedent(std::string antecedent) { (this->antecedent) = parenthesize(antecedent); }
    inline void setLabel(std::string label) { (this->label) = label; }
    inline void setConsequent(SeqSetExpr consequent) { (this->consequent) = consequent; }

    inline std::string getAntecedent() { return antecedent; }
    inline std::string getLabel() { return label; }
    inline SeqSetExpr getConsequent() { return consequent; }

    std::string toFormulaString(std::vector<std::string> ids) {
      auto subExpressions = consequent.getSubExpressions();
      assert(subExpressions.size() == ids.size());

      std::string formula = "";
      for(int i = 0; i < ids.size(); ++i) {
        std::string clause = parenthesize(subExpressions[i].toFormulaString(nextId(ids[i])));
        if(i == 0) { formula =  clause; }
        else { formula = land(formula, clause); }
      }

      return parenthesize(land(antecedent, formula));
    }

  private:

    std::string antecedent;
    std::string label;
    SeqSetExpr consequent;
};

