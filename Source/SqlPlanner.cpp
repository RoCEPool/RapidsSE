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
: Object(CN_SQLPLANNER),
  m_pTableRefStack(new Stack(KK_PARSER_STACK))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SQLPLANNER,_FOOTER_);
  
  _checkInitialised_(CN_SQLPLANNER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SQLPLANNER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SqlPlanner::~SqlPlanner(void) noexcept
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SQLPLANNER,_FOOTER_);
   
  // Clean up the stack of TableRefs
  while (m_pTableRefStack->nValues() > 0) {
    TableRef* pTableRef = _pTableRef(m_pTableRefStack->pPopPtr(__func__));
    delete pTableRef;
  }
  delete m_pTableRefStack;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Parse and create an initial plan for a SQL statement                       */
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
  
  // Add our own error listener so that we can be notified if there is a parsing error.
  parser.removeErrorListeners();
  parser.addErrorListener(&errorListener);
  
  // Parse the token stream.
  sqlParser::StatementContext* pStatementCtx = parser.statement();
  
  // Check for any syntax errors
  if (errorListener.bGotError()) {
    vTrace("%s\n"_Stz, errorListener.szErrorMsg()->azChar());
    return;
  }
  
  // Visit the parse tree that resulted from the above parsing.
  this->visitStatement(pStatementCtx);
  //vTrace("Finished visiting the parse tree.\n"_Stz);  
}


/*----------------------------------------------------------------------------*/
/* Visit functions for parse tree nodes                                       */
/*----------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Statement node (top-most level of the parse tree)                          */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitStatement(sqlParser::StatementContext *ctx)
{
  return visitChildren(ctx);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Query node (Called for the main query and each subquery)                   */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitQuery(sqlParser::QueryContext *ctx)
{
   return visitChildren(ctx);
 }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select node (for each select of each query/subquery)                       */
/* Contains WITH / ORDER BY / LIMIT                                           */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectQuery(sqlParser::SelectQueryContext *ctx)
{
  antlrcpp::Any any = visitChildren(ctx);
  
  if (m_pTableRefStack->nValues() > 0) {
    vTrace("  /from\n"_Stz);
    
    s256 ref;
    TableRef* pTableRef = _pTableRef(m_pTableRefStack->pPopPtr(__func__));
    pTableRef->vPrintInto(ref);
    
    vTrace("    /%s\n"_Stz, ref.szStz());
    delete pTableRef;
  }
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select Expression node (for each select of each query/subquery)            */
/* Contains select list / FROM / WHERE / GROUP BY / HAVING                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectExpr(sqlParser::SelectExprContext *ctx)
{
  return visitChildren(ctx);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* (Table) Fetch node                                                         */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitFetch(sqlParser::FetchContext *ctx)
{
  visitChildren(ctx);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Table Reference node                                                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitTableReference(sqlParser::TableReferenceContext *ctx)
{
  visitChildren(ctx);
  Text* pCatalogName = nullptr;
  Text* pSchemaName = nullptr;
  Text* pTableName = nullptr;
  
  if (ctx->catalogName() != nullptr) {
    pCatalogName = cleanId(ctx->catalogName()->id);
  }
  
  if (ctx->schemaName() != nullptr) {
    pSchemaName = cleanId(ctx->schemaName()->id);
  }
  
  ASSERT(ctx->tableName() != nullptr);
  pTableName = cleanId(ctx->tableName()->id);

  TableRef* pTableRef = new TableRef(pCatalogName, pSchemaName, pTableName);
  m_pTableRefStack->vPushPtr(pTableRef);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select List node                                                           */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectList(sqlParser::SelectListContext *ctx)
{
  return visitChildren(ctx);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Clean up a name of an object. Returned string is dynamically allocated.    */
/*----------------------------------------------------------------------------*/
Text* SqlPlanner::cleanId(sqlParser::IdentifierContext *ctx)
{
  std::string idStr = ctx->getText();
  
  i8 idLen = idStr.length();
  
  if (idLen == 0) {
    return new Text(""_Stz);
  }
      
  // Remove double quotes
  if (idStr.at(0) == '"' && idStr.at(idLen - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    return new Text((Stz*) idStr.substr(1, idLen - 2).c_str());
  }
  
  // Remove backticks
  if (idStr.at(0) == '`' && idStr.at(idLen - 1) == '`') {
    // Not great, substr() makes a another copy of the string.
    return new Text((Stz*) idStr.substr(1, idLen - 2).c_str());
  }
  
  return new Text((Stz*) idStr.c_str());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
