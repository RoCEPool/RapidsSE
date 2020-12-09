
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/json5.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "json5Visitor.h"


namespace Parser {

/**
 * This class provides an empty implementation of json5Visitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  json5BaseVisitor : public json5Visitor {
public:

  virtual antlrcpp::Any visitJson5(json5Parser::Json5Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObj(json5Parser::ObjContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPair(json5Parser::PairContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitKey(json5Parser::KeyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(json5Parser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArr(json5Parser::ArrContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace Parser
