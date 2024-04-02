
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/json5.g4 by ANTLR 4.13.1

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

  virtual std::any visitJson5(json5Parser::Json5Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitObj(json5Parser::ObjContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPair(json5Parser::PairContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKey(json5Parser::KeyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue(json5Parser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArr(json5Parser::ArrContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace Parser
