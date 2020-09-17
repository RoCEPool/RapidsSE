/*============================================================================*/
/* SqlPlanner.cpp -                                                            */
/*============================================================================*/

#include "Parser.h"
#include "sqlLexer.h"
#include "sqlParser.h"
#include "sqlBaseVisitor.h"
#include "SqlErrorListener.hpp"

//using namespace antlr4;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,SqlPlanner,CN_SQLPLANNER)
__aNewFunction(SqlPlanner,CN_SQLPLANNER)
__vDeleteFunction(SqlPlanner,CN_SQLPLANNER)
__aFooterFunction(SqlPlanner)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SqlPlanner::SqlPlanner(void)
: Object(CN_SQLPLANNER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SQLPLANNER,_FOOTER_);
  
  _checkInitialised_(CN_SQLPLANNER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SQLPLANNER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SqlPlanner::~SqlPlanner(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SQLPLANNER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void SqlPlanner::plan(Stz* pStr)
{
  // We need to give the Lexer a CharStream, but that requires an STL-string...
  std::string sqlStr(pStr->azChar());
  antlr4::ANTLRInputStream stream(sqlStr);
  
  // Create the lexer and get the stream of tokens representing the input command.
  sqlLexer lexer(&stream);
  antlr4::CommonTokenStream tokens(&lexer);
  
  // The thing that gets told of parsing errors.
  SqlErrorListener errorListener;
  
  // Create a parser from the stream of tokens.
  sqlParser parser(&tokens);
  
//  antlr4::ANTLRErrorStrategy errorHandler;
//  parser.setErrorHandler(errorHandler);
  parser.removeErrorListeners();
  parser.addErrorListener(&errorListener);
  
  // Parse the token stream.
  sqlParser::StatementContext* pStatementCtx = parser.statement();
  
  // Check for any syntax errors
  if (errorListener.bGotError()) {
    vTrace("%s\n"_Stz, errorListener.szErrorMsg());
    return;
  }
  
  // Visit the parse tree that resulted from the above parsing.
  vTrace("Statement parsed successfully. About to visit the parse tree.\n"_Stz);
  this->visitStatement(pStatementCtx);
  vTrace("Finished visiting the parse tree.\n"_Stz);  
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
