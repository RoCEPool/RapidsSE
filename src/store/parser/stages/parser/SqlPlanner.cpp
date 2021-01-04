/*============================================================================*/
/* SqlPlanner.cpp - Parses SQL statements & produces an initial knowledgebase */
/*============================================================================*/

#include "BASE.h"
#include "SqlPlanner.hpp"

#include "sqlLexer.h"
#include "sqlParser.h"
#include "sqlBaseVisitor.h"

#include "QueryNode.hpp"
#include "QueryNodeStack.hpp"
#include "QueryNodeList.hpp"

#include "PlanConstants.hpp"
#include "parser/common/ErrorListener.hpp"
#include "GraphNav.hpp"

#include "ShadowNode.hpp"
#include "store/parser/shadowNodes/planObjects/Statement.hpp"
#include "TableRef.hpp"

#include "ColumnRef.hpp"
#include "LiteralExpr.hpp"

#include "ProjectedExpr.hpp"

#include "ProjectOp.hpp"
#include "TableScanOp.hpp"

using namespace Parser;

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
  m_pStatementStack(new QueryNodeStack()),
  m_pTableRefStack(new QueryNodeStack()),
  m_pExprStack(nullptr),
  m_pSelectItems(nullptr)
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
  
  // Clean up the statement, tableref and expression stacks
  if (m_pStatementStack) {
    while (m_pStatementStack->nItems() > 0) {
      QueryNode* pItem = m_pStatementStack->pPop();
      delete pItem;
    }
    delete m_pStatementStack;
    m_pStatementStack = nullptr;
  }

  if (m_pTableRefStack) {
    while (m_pTableRefStack->nItems() > 0) {
      QueryNode* pItem = m_pTableRefStack->pPop();
      delete pItem;
    }
    delete m_pTableRefStack;
    m_pTableRefStack = nullptr;
  }

  if (m_pExprStack) {
    while (m_pExprStack->nItems() > 0) {
      QueryNode* pItem = m_pExprStack->pPop();
      delete pItem;
    }
    delete m_pExprStack;
    m_pExprStack = nullptr;
  }

  if (m_pSelectItems) {
    while (m_pSelectItems->nItems() > 0) {
      QueryNode* pItem = m_pSelectItems->pPop();
      delete pItem;
    }
    delete m_pSelectItems;
    m_pSelectItems = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Parse and create an initial plan for a SQL statement                       */
/*----------------------------------------------------------------------------*/
void SqlPlanner::plan(Stz* pStr, PlanNode* pTemplate)
{
  // We need to give the Lexer a CharStream, but that requires an STL-string...
  std::string sqlStr(pStr->azChar());
  antlr4::ANTLRInputStream stream(sqlStr);
  
  // Create the lexer and get the stream of tokens representing the input command.
  sqlLexer lexer(&stream);
  antlr4::CommonTokenStream tokens(&lexer);
  
  // The thing that gets told of parsing errors.
  ErrorListener errorListener;
  
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

  // Create a new plan graph using the template
  // TODO: Uncomment this when we have a list class with separate node pointers
  //PlanNode* pRoot = GraphEditor::pNewFromTemplate(pTemplate);
  PlanNode* pRoot = pTemplate;

  // Find the subpath where we will attach the parsed query plan
  PlanNode* pItem = GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRoot);
  ASSERT(pItem);
  ASSERT(pItem->eClass() == CN_QUERYNODE);
  QueryNode* pQueryNode = static_cast<QueryNode*>(pItem);

  // Attach the statement plan we created
  ASSERT(m_pStatementStack->nItems() == 1);
  QueryNode* pStatementNode = m_pStatementStack->pPop();
  pQueryNode->vAppendChild(pStatementNode);
}


/*----------------------------------------------------------------------------*/
/* Visit functions for parse tree nodes                                       */
/*----------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Statement node (top-most level of the parse tree)                          */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitStatement(sqlParser::StatementContext *ctx)
{
  // Save the whole statement text
  antlr4::misc::Interval interval(ctx->start->getStartIndex(), ctx->stop->getStopIndex());
  std::string queryStlStr = ctx->start->getInputStream()->getText(interval);

  m_sQueryStr.vCopy((Stz*) queryStlStr.c_str());

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
  QueryNode* pStmt = Statement::pCreate(Statement::ST_SELECT, m_sQueryStr.szStz());
  m_pStatementStack->vPush(pStmt);

  visitChildren(ctx);
  return nullptr;
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

  QueryNode* pTableRef = TableRef::pCreate(pCatalogName, pSchemaName, pTableName);
  m_pTableRefStack->vPush(pTableRef);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select Expression node (for each select of each query/subquery)            */
/* Contains select list / FROM / WHERE / GROUP BY / HAVING                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectExpr(sqlParser::SelectExprContext* ctx)
{
  ASSERT(m_pStatementStack->nItems() > 0);
  ASSERT(!m_pSelectItems);
  m_pSelectItems = new QueryNodeList();

  // Visit the child parse tree nodes. The select list above will be populated.
  antlrcpp::Any any = visitChildren(ctx);

  ASSERT(m_pSelectItems->nItems() > 0);

  // Create a project operator and attach it to the statement object
  QueryNode* pProjectOp = ProjectOp::pCreate(m_pSelectItems);

  QueryNode* pStatement = m_pStatementStack->pPeek(0);
  ASSERT(Statement::bIsOfType(pStatement));
  pStatement->vAppendChild(pProjectOp);



  // Check for a tableRef. If one exists, add it to a TableScanOp
  // and attach the TableScanOp as a child of the ProjectOp
  if (m_pTableRefStack->nItems() > 0) {
    ASSERT(m_pTableRefStack->nItems() == 1);
    QueryNode* pTableRef = m_pTableRefStack->pPop();

    QueryNode* pTableScanOp = TableScanOp::pCreate(pTableRef);
    pProjectOp->vAppendChild(pTableScanOp);
  }



  ////////////////////////////////////////////////////////
//  if (m_pTableRefStack->nItems() > 0) {
//    vTrace("  /from\n"_Stz);
//
//    s2K sBuf;
//    QueryNode* pTableRef = m_pTableRefStack->pPop();
//    ShadowNode::vDescribe(pTableRef, sBuf);
//
//    vTrace("    /%s\n"_Stz, sBuf.szStz());
//    delete pTableRef;
//  }
//
//  ASSERT(m_pSelectItems->nItems() > 0);
//  vTrace("  /project\n"_Stz);
//
//  while (m_pSelectItems->nItems() > 0) {
//    QueryNode* pSelectItem = m_pSelectItems->pPop();
//    s2K buf;
//
//    buf.vCopy("    /"_Stz);
//    ShadowNode::vDescribe(pSelectItem, buf);
//
//    buf.vConcat("\n"_Stz);
//    vTrace(buf.szStz());
//  }
  ////////////////////////////////////////////////////////

  ASSERT(m_pSelectItems->nItems() == 0);
  delete m_pSelectItems;
  m_pSelectItems = nullptr;
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select List node                                                           */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectList(sqlParser::SelectListContext *ctx)
{
  ASSERT(m_pSelectItems);
  
  ASSERT(!m_pExprStack);
  m_pExprStack = new QueryNodeStack();
  
  visitChildren(ctx);
  
  ASSERT(m_pExprStack->nItems() == 0);
  delete m_pExprStack;
  m_pExprStack = nullptr;
  
  ASSERT(m_pSelectItems->nItems() > 0);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select List Item node                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectListItem(sqlParser::SelectListItemContext* ctx)
{
  visitChildren(ctx);
  
  // pop the expr stack and add it to the select list.
  ASSERT(m_pExprStack->nItems() == 1);
  
  QueryNode* pExprNode = m_pExprStack->pPop();

  // Is there an alias?
  Text* pAliasText = cleanId(ctx->as);

  QueryNode* pProjectedExpr = ProjectedExpr::pCreate(pExprNode, pAliasText);

  m_pSelectItems->vAppend(pProjectedExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Column Reference node                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitColumnRef(sqlParser::ColumnRefContext* ctx)
{
  ASSERT(m_pExprStack);

  visitChildren(ctx);
  
  Text* pCatalogName = ctx->catalogName() == nullptr ? nullptr : cleanId(ctx->catalogName()->id);
  
  Text* pSchemaName = ctx->schemaName() == nullptr ? nullptr : cleanId(ctx->schemaName()->id);
  
  Text* pTableName = ctx->tableName() == nullptr ? nullptr : cleanId(ctx->tableName()->id);
  
  Text* pColumnName = cleanId(ctx->columnName()->id);

  QueryNode* pColumnRef = ColumnRef::pCreate(pCatalogName, pSchemaName,
                                             pTableName, pColumnName);

  m_pExprStack->vPush(pColumnRef);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Clean up a name of an object. Returned string is dynamically allocated.    */
/*----------------------------------------------------------------------------*/
Text* SqlPlanner::cleanId(sqlParser::IdentifierContext *ctx)
{
  if (!ctx) {
    return nullptr;
  }

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
