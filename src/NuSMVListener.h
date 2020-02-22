#pragma once

#include <set>
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
    }

    virtual void exitModule(NuSMVParser::ModuleContext *ctx) override {
      spec.setI(parenthesize(I)); spec.setT(parenthesize(T)); spec.setP(P); 
    }

    virtual void exitInit(NuSMVParser::InitContext *ctx) override { 
      std::string id = (ctx->id())->getText();
      assert(declaredSymbols.count(id));

      std::string clause = getClause(id);

      if(I == "") { I = clause; }
      else { I = land(I, clause); }
    }

    virtual void exitNext(NuSMVParser::NextContext *ctx) override {
      std::string id = (ctx->id())->getText();
      assert(declaredSymbols.count(id));

      id = "next_" + id;

      std::string clause = getClause(id);

      if(T == "") { T = clause; }
      else { T = land(T, clause); }
    }


    virtual void exitSafetySpecBlock(NuSMVParser::SafetySpecBlockContext *ctx) override { 
      P = curExpression; 
      curExpression = "";
    }

    virtual void enterSimpleExpression(NuSMVParser::SimpleExpressionContext *ctx) override {
      curExpression = (ctx->formula())->getText();
    }

    virtual void enterCaseExpression(NuSMVParser::CaseExpressionContext *ctx) override {
      isCase = true;
      antecedents.clear(); consequents.clear();
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
    ModelSpecification spec;

    bool isCase;

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
      } else {
        clause = leq(id, curExpression); 
        curExpression = "";
      }

      return clause;
    }
};
