#pragma once

#include <set>
#include <algorithm>
#include "NuSMVLexer.h"
#include "NuSMVParser.h"
#include "NuSMVParserBaseListener.h"
#include "complyer/ModelSpecification.h"

#include "antlr4-runtime.h"

class NuSMVListener : public NuSMVParserBaseListener {
  public:

    virtual void enterVarDeclaration(NuSMVParser::VarDeclarationContext *ctx) override {
      std::string typeString = (ctx->type())->getText();
      std::string name = (ctx->id())->getText();

      assert(!spec.isPresent(name));

      type symbolType;
      if(typeString == "boolean") symbolType = bool_const;
      if(typeString == "integer") symbolType = int_const;

      Symbol symbol(symbolType, name);
      spec.addSymbol(symbol);
    }

    virtual void enterModule(NuSMVParser::ModuleContext *ctx) override {
      I = T = P = curExpression = "";
      isSeq = isCon = insideCon = false;
    }

    virtual void exitModule(NuSMVParser::ModuleContext *ctx) override {
      if(isCon) { prepareConcurrentT(); }
      spec.setI(parenthesize(I)); spec.setT(parenthesize(T)); spec.setP(P); 
      if(isLtl) { spec.setLtl(); spec.setBound(bound); }
    }

    virtual void exitInit(NuSMVParser::InitContext *ctx) override { 
      std::string id = (ctx->id())->getText();
      assert(spec.isPresent(id));

      std::string clause = getClause(id);

      if(I == "") { I = clause; }
      else { I = land(I, clause); }
    }

    virtual void enterNext(NuSMVParser::NextContext *ctx) override {
      isSeq = true;
      assert(!isCon);
    }

    virtual void exitNext(NuSMVParser::NextContext *ctx) override {
      std::string id = (ctx->id())->getText();
      assert(spec.isPresent(id));

      id = "next_" + id;

      std::string clause = getClause(id);

      if(T == "") { T = clause; }
      else { T = land(T, clause); }
    }
    
    virtual void enterConcurrentNext(NuSMVParser::ConcurrentNextContext *ctx) override { 
      isCon = true;
      insideCon = true;
      assert(!isSeq);
    }

    virtual void exitConcurrentNext(NuSMVParser::ConcurrentNextContext *ctx) override {
      transitions.emplace_back(getTransition());
      concurrentSet.clear();
      insideCon = false;
    }

    virtual void enterConExpression(NuSMVParser::ConExpressionContext *ctx) override {
      conAntecedent = (ctx->antecedent)->getText();
    }

    virtual void exitSafetySpecBlock(NuSMVParser::SafetySpecBlockContext *ctx) override { 
      isLtl = false;
      P = curExpression; 
      curExpression = "";
    }

    virtual void exitLtlSpecBlock(NuSMVParser::LtlSpecBlockContext *ctx) override {
      isLtl = true;
      P = curExpression;
      bound = std::stoi((ctx->bound)->getText());
      curExpression = "";
    }


    virtual void enterSet(NuSMVParser::SetContext *ctx) override {
      insideSet = true;
      curSet.clear();
    }
    
    virtual void exitSet(NuSMVParser::SetContext *ctx) override {
      if(insideCon) {
        if(concurrentSet.empty()) { concurrentSet = curSet; } 
        else { conConsequent = curSet; assert(conConsequent.size() == concurrentSet.size()); }
      }
      insideSet = false;
    }

    virtual void enterSimpleExpression(NuSMVParser::SimpleExpressionContext *ctx) override {
      curExpression = (ctx->formula())->getText();
      if(insideSet) { curSet.emplace_back(curExpression); }
    }
    
    virtual void enterCaseSubExpression(NuSMVParser::CaseSubExpressionContext *ctx) override {
      antecedents.emplace_back(parenthesize((ctx->antecedent)->getText()));
      consequents.emplace_back(parenthesize((ctx->consequent)->getText()));
    }

    virtual void enterInterval(NuSMVParser::IntervalContext *ctx) override {
      intervalFrom = (ctx->from)->getText();
      intervalTo = (ctx->to)->getText();;
    }

    virtual void enterSimpleExpr(NuSMVParser::SimpleExprContext *ctx) override { expressionType = 1; }
    virtual void enterCaseExpr(NuSMVParser::CaseExprContext *ctx) override { expressionType = 2; }
    virtual void enterIntervalExpr(NuSMVParser::IntervalExprContext *ctx) override { expressionType = 3; }
    virtual void enterSetExpr(NuSMVParser::SetExprContext *ctx) override { expressionType = 4; }

    ModelSpecification getSpecification() { return spec; }

  private:

    std::string I, T;
    std::string P;
    std::string curExpression;
    std::vector<std::string> antecedents;
    std::vector<std::string> consequents;

    std::string conAntecedent;
    std::vector<std::string> concurrentSet;
    std::vector<std::string> curSet;
    std::vector<std::string> conConsequent;
    std::vector<std::string> transitions;
    
    ModelSpecification spec;

    bool isSeq;
    bool isCon;
    bool insideSet;
    bool isLtl;
    bool insideCon;

    int bound;
    int expressionType;
    std::string intervalFrom, intervalTo;

    std::string getClause(std::string id) {
      std::string clause = "";

      if(expressionType == 1) { clause = leq(id, curExpression); curExpression = ""; }
      if(expressionType == 4) { clause = land(parenthesize(lge(id, intervalFrom)), parenthesize(lle(id, intervalTo))); }
      if(expressionType == 3) {
        clause = parenthesize(leq(id, curSet[0]));
        for(int i = 1; i < curSet.size(); ++i) { clause = lor(clause, parenthesize(leq(id, curSet[i]))); }
      }
      if(expressionType == 2) {
        for(int i = 0; i < antecedents.size(); ++i) {
          std::string sub = limplies(antecedents[i], parenthesize(leq(id, consequents[i])));
          sub = parenthesize(sub);
          if(i == 0) { clause = sub; }
          else { clause = land(clause, sub); }
        }
        antecedents.clear(); consequents.clear();
      } 

      return clause;
    }

    std::string getTransition() {
      std::vector<std::string> notPresent;
      std::string transition = conAntecedent;
      auto declaredSymbols = spec.getSymbols();

      std::vector<std::string> declaredSymbolNames;
      for(auto symbol: declaredSymbols) {
        declaredSymbolNames.emplace_back(symbol.getName());
      }

      sort(declaredSymbolNames.begin(), declaredSymbolNames.end());
      sort(concurrentSet.begin(), concurrentSet.end());
      std::set_difference(declaredSymbolNames.begin(), declaredSymbolNames.end(), 
        concurrentSet.begin(), concurrentSet.end(), std::inserter(notPresent, notPresent.end()));

      for(int i = 0; i < concurrentSet.size(); ++i) {
        std::string next_id = "next_" + concurrentSet[i];
        transition = land(transition, parenthesize(leq(next_id, conConsequent[i])));
      }

      for(auto id: notPresent) {
        std::string next_id = "next_" + id;
        transition = land(transition, parenthesize(leq(next_id, id)));
      }

      return parenthesize(transition);
    }

    void prepareConcurrentT() {
      for(int i = 0; i < transitions.size(); ++i) {
        std::string clause = transitions[i];
        for(int j = 0; j < transitions.size(); ++j) {
          if(j != i) {
           clause = land(clause, lnot(transitions[j]));
          }
        }
        clause = parenthesize(clause);
        if(i == 0) T = clause;
        else T = lor(T, clause);
      }
    }
};
