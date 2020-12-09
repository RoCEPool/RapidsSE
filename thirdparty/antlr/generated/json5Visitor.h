
// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/json5.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "json5Parser.h"


namespace Parser {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by json5Parser.
 */
class  json5Visitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by json5Parser.
   */
    virtual antlrcpp::Any visitJson5(json5Parser::Json5Context *context) = 0;

    virtual antlrcpp::Any visitObj(json5Parser::ObjContext *context) = 0;

    virtual antlrcpp::Any visitPair(json5Parser::PairContext *context) = 0;

    virtual antlrcpp::Any visitKey(json5Parser::KeyContext *context) = 0;

    virtual antlrcpp::Any visitValue(json5Parser::ValueContext *context) = 0;

    virtual antlrcpp::Any visitArr(json5Parser::ArrContext *context) = 0;


};

}  // namespace Parser
