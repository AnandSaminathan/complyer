#pragma once

#include <set>
#include <algorithm>
#include "NuSMVLexer.h"
#include "NuSMVParser.h"
#include "NuSMVParserBaseListener.h"
#include "complyer/nusmv/NuSMV.hpp"

#include "antlr4-runtime.h"

class NuSMVListener : public NuSMVParserBaseListener {
  public:

    void enterModule(NuSMVParser::ModuleContext *ctx) override { 
      module.setName((ctx->name)->getText());
    }
    void exitModule(NuSMVParser::ModuleContext *ctx) override { 
      nusmv.addModule(module);
      module = Module();
    }

    void enterSafetySpec(NuSMVParser::SafetySpecContext *ctx) override {
      Specification spec(SAFETYSPEC, (ctx->formula())->getText());

      auto boundCtx = ctx->bound();
      if(boundCtx != nullptr) {
        spec.setBound((boundCtx->val)->getText());
      } 

      nusmv.addSpecification(spec);
    }

    void enterLtlSpec(NuSMVParser::LtlSpecContext *ctx) override {
      Specification spec(LTLSPEC, (ctx->formula())->getText()); 

      auto boundCtx = ctx->bound();
      if(boundCtx != nullptr) {
        spec.setBound((boundCtx->val)->getText());
      } 

      nusmv.addSpecification(spec);
    }

    void enterParameters(NuSMVParser::ParametersContext *ctx) override { 
      auto idCtxs = ctx->id();
      std::vector<std::string> ids;
      for(auto idCtx: idCtxs) {
        ids.emplace_back(idCtx->getText());
      }
      module.setParameters(ids);
    }

    void enterDeclaration(NuSMVParser::DeclarationContext *ctx) override { 
      std::string name = (ctx->id())->getText();
      std::string typeStr = (ctx->type())->getText();

      type symbolType = (typeStr == "boolean") ? bool_const : int_const;
      module.addSymbol(Symbol(symbolType, name));
    }

    void exitAssignment(NuSMVParser::AssignmentContext *ctx) override { 
      module.setAssignment(assignment);
      assignment = Assignment();
    }

    void enterInit(NuSMVParser::InitContext *ctx) override {
      module.setInit((ctx->formula())->getText());
    }

    void enterTrans(NuSMVParser::TransContext *ctx) override {
      module.setTrans((ctx->formula())->getText());
    }

    void exitInitAssignment(NuSMVParser::InitAssignmentContext *ctx) override {
      Init init((ctx->id())->getText(), seqExpression); 
      assignment.addInit(init);
    }

    void exitSeqNextAssignment(NuSMVParser::SeqNextAssignmentContext *ctx) override {
      SeqNext seqNext((ctx->id())->getText(), seqExpression);
      assignment.addSeqNext(seqNext);
    }

    void exitConNextAssignment(NuSMVParser::ConNextAssignmentContext *ctx) override { 
      auto idCtxs = (ctx->set())->id();
      std::vector<std::string> ids;
      for(auto idCtx: idCtxs) {
        ids.emplace_back(idCtx->getText());
      }
      ConNext conNext(ids, conExpression);
      assignment.addConNext(conNext);
    }

    void enterSeqSimpleExpr(NuSMVParser::SeqSimpleExprContext *ctx) override {
      SeqSimpleExpr seqSimpleExpr((ctx->formula())->getText());
      seqExpression = seqSimpleExpr;
    }

    void exitSeqCaseExpr(NuSMVParser::SeqCaseExprContext *ctx) override { 
      seqExpression = seqCaseExpr;
      seqCaseExpr = SeqCaseExpr();
    }

    void exitSeqIntervalExpr(NuSMVParser::SeqIntervalExprContext *ctx) override { 
      SeqIntervalExpr seqIntervalExpr((ctx->from)->getText(), (ctx->to)->getText());
      seqExpression = seqIntervalExpr;
    }

    void enterSeqSetExpr(NuSMVParser::SeqSetExprContext *ctx) override {
      seqSetExpr = SeqSetExpr();
    }

    void exitSeqSetExpr(NuSMVParser::SeqSetExprContext *ctx) override {
      seqExpression = seqSetExpr;
    }

    void exitSeqSetSubExpr(NuSMVParser::SeqSetSubExprContext *ctx) override {
      seqSetExpr.addSubExpression(seqExpression);
    }

    void exitSeqCaseSubExpr(NuSMVParser::SeqCaseSubExprContext *ctx) override { 
      SeqCaseSubExpr seqCaseSubExpr((ctx->formula())->getText(), seqExpression);
      seqCaseExpr.addSubExpression(seqCaseSubExpr);
    }

    void exitConExpression(NuSMVParser::ConExpressionContext *ctx) override { 
      std::string antecedent = (ctx->formula())->getText();
      std::string label = ((ctx->label())->id())->getText();
      conExpression = ConExpression(antecedent, label, seqSetExpr);
    }

    void exitDefinition(NuSMVParser::DefinitionContext *ctx) override {
      Definition definition((ctx->id())->getText(), (ctx->formula())->getText());
      module.addDefinition(definition);
    }

    NuSMV getNuSMV() { return nusmv; }

  private:

    NuSMV nusmv;
    Module module;
    Assignment assignment;
    SeqExpression seqExpression;
    ConExpression conExpression;
    SeqCaseExpr seqCaseExpr;
    SeqSetExpr seqSetExpr;

};
