#pragma once

#include <set>
#include <algorithm>
#include "NuSMVLexer.h"
#include "NuSMVParser.h"
#include "NuSMVParserBaseListener.h"
#include "ModelSpecification.h"

#include "antlr4-runtime.h"

class NuSMVListener : public NuSMVParserBaseListener {
  public:

    virtual void enterVarDeclaration(NuSMVParser::VarDeclarationContext *ctx) override {
      std::string typeString = (ctx->type())->getText();
      std::string name = (ctx->id())->getText();

      type symbolType;
      if(typeString == "boolean") symbolType = bool_const;
      if(typeString == "integer") symbolType = int_const;

      Symbol symbol(symbolType, name);
      spec.addSymbol(symbol);
      declaredSymbols.insert(name);
    }

    virtual void enterModule(NuSMVParser::ModuleContext *ctx) override {
      I = T = P = curExpression = "";
      isCase = isSeq = isCon = false;
    }

    virtual void exitModule(NuSMVParser::ModuleContext *ctx) override {
      if(isCon) { prepareConcurrentT(); }
      spec.setI(parenthesize(I)); spec.setT(parenthesize(T)); spec.setP(P); 
      if(isLtl) { spec.setLtl(); spec.setBound(bound); }
    }

    virtual void exitInit(NuSMVParser::InitContext *ctx) override { 
      std::string id = (ctx->id())->getText();
      assert(declaredSymbols.count(id));

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
      assert(declaredSymbols.count(id));

      id = "next_" + id;

      std::string clause = getClause(id);

      if(T == "") { T = clause; }
      else { T = land(T, clause); }
    }
    
    virtual void enterConcurrentNext(NuSMVParser::ConcurrentNextContext *ctx) override { 
      isCon = true;
      assert(!isSeq);
    }

    virtual void exitConcurrentNext(NuSMVParser::ConcurrentNextContext *ctx) override {
      transitions.emplace_back(getTransition());
      concurrentSet.clear();
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
      if(concurrentSet.empty()) {
        concurrentSet = curSet;
      } else {
        conConsequent = curSet;
        assert(conConsequent.size() == concurrentSet.size());
      }
      insideSet = false;
    }

    virtual void enterSimpleExpression(NuSMVParser::SimpleExpressionContext *ctx) override {
      curExpression = (ctx->formula())->getText();
      if(insideSet) {
        curSet.emplace_back(curExpression);
      }
    }

    virtual void enterCaseExpression(NuSMVParser::CaseExpressionContext *ctx) override {
      isCase = true;
    }
    
    virtual void enterCaseSubExpression(NuSMVParser::CaseSubExpressionContext *ctx) override {
      antecedents.emplace_back(parenthesize((ctx->antecedent)->getText()));
      consequents.emplace_back(parenthesize((ctx->consequent)->getText()));
    }

    ModelSpecification getSpecification() { return spec; }

  private:

    std::string I, T;
    std::string P;
    std::string curExpression;
    std::set<std::string> declaredSymbols;
    std::vector<std::string> antecedents;
    std::vector<std::string> consequents;

    std::string conAntecedent;
    std::vector<std::string> concurrentSet;
    std::vector<std::string> curSet;
    std::vector<std::string> conConsequent;
    std::vector<std::string> transitions;
    
    ModelSpecification spec;

    bool isCase;
    bool isSeq;
    bool isCon;
    bool insideSet;
    bool isLtl;

    int bound;

    std::string getClause(std::string id) {
      std::string clause = "";

      if(isCase) {
        for(int i = 0; i < antecedents.size(); ++i) {
          std::string sub = limplies(antecedents[i], parenthesize(leq(id, consequents[i])));
          sub = parenthesize(sub);
          if(i == 0) clause = sub;
          else {
            clause = land(clause, sub);
          }
        }
        isCase = false;
        antecedents.clear(); consequents.clear();
      } else {
        clause = leq(id, curExpression); 
        curExpression = "";
      }

      return clause;
    }

    std::string getTransition() {
      std::set<std::string> present;
      std::set<std::string> notPresent;
      std::string transition = conAntecedent;
      
      for(auto id: concurrentSet) { present.insert(id); }

      assert(present.size() == concurrentSet.size());
      std::set_difference(declaredSymbols.begin(), declaredSymbols.end(), 
        present.begin(), present.end(), std::inserter(notPresent, notPresent.end()));

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
