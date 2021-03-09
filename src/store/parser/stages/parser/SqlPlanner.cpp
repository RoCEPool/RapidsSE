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
#include "GraphValidator.hpp"
#include "GraphEditor.hpp"

#include "ShadowNode.hpp"
#include "store/parser/shadowNodes/planObjects/Statement.hpp"
#include "store/parser/shadowNodes/planObjects/FilterOp.hpp"
#include "store/parser/shadowNodes/planObjects/LimitOp.hpp"
#include "store/parser/shadowNodes/planObjects/ProjectOp.hpp"
#include "store/parser/shadowNodes/planObjects/AggregateOp.hpp"
#include "store/parser/shadowNodes/planObjects/dataSources/TableScanOp.hpp"

#include "store/parser/shadowNodes/compound/TableRef.hpp"

// Expressions
#include "ColumnRef.hpp"
#include "ColumnWildcard.hpp"
#include "store/parser/shadowNodes/expressions/literals/NullLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/StringLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/IntegerLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/BooleanLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/FloatLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/DateLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/TimeLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/TimestampLiteral.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/AdditionExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/SubtractionExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/MultiplicationExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/DivisionExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/ModulusExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/NegationExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/EqExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/NeExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/GtExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/GeExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/LtExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/LeExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/logic/AndExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/logic/OrExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/logic/XorExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/logic/NotExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/logic/IsNullExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/CountExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/SumExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/AvgExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/MinExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/MaxExpr.hpp"

#include "store/parser/shadowNodes/compound/ProjectedExpr.hpp"

#include "SqlException.hpp"

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
  m_pExprList(nullptr),
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

  if (m_pExprList) {
    while (m_pExprList->nItems() > 0) {
      QueryNode* pItem = m_pExprList->pPop();
      delete pItem;
    }
    delete m_pExprList;
    m_pExprList = nullptr;
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
PlanNode* SqlPlanner::plan(Stz* pStr, const PlanNode* pTemplate)
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
    //vTrace("%s\n"_Stz, errorListener.szErrorMsg()->azChar());
    SyntaxException::vThrow(errorListener.iLine(), errorListener.iColumn(), errorListener.szErrorMsg());
  }
  
  // Visit the parse tree that resulted from the above parsing.
  // May throw a SyntaxException or SemanticException.
  this->visitStatement(pStatementCtx);

  // Create a new plan graph using the template
  PlanNode* pRoot = GraphEditor::pNewFromTemplate(pTemplate);

  // Find the subpath where we will attach the parsed query plan
  PlanNode* pItem = GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRoot);
  ASSERT(pItem);
  ASSERT(pItem->eGetClassNo() == CN_QUERYNODE);
  QueryNode* pQueryNode = static_cast<QueryNode*>(pItem);

  // Attach the statement plan we created
  ASSERT(m_pStatementStack->nItems() == 1);
  QueryNode* pStatementNode = m_pStatementStack->pPop();
  pQueryNode->vAppendChild(pStatementNode);

  // Validate the plan subsection of the graph
  GraphValidator::vValidatePlan(pRoot);

  return pRoot;
}


/*----------------------------------------------------------------------------*/
/* Visit functions for parse tree nodes                                       */
/*----------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Statement node (top-most level of the parse tree)                          */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitStatement(sqlParser::StatementContext *pCtx)
{
  // Save the whole statement text
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  m_sQueryStr.vCopy((Stz*) queryStlStr.c_str());

  return visitChildren(pCtx);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Query node (Called for the main query and each subquery)                   */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitQuery(sqlParser::QueryContext *pCtx)
{
   return visitChildren(pCtx);
 }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select node (for each select of each query/subquery)                       */
/* Contains WITH / ORDER BY / LIMIT                                           */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectQuery(sqlParser::SelectQueryContext *pCtx)
{
  QueryNode* pStmt = Statement::pCreate(Statement::ST_SELECT, m_sQueryStr.szStz());
  m_pStatementStack->vPush(pStmt);

  visitChildren(pCtx);

  if (pCtx->ORDER()) {
    // TODO

  }

  if (pCtx->LIMIT()) {
    i8 iLimit = iCleanInt(pCtx->limit);
    i8 iOffset = -1;

    if (iLimit < 0) {
      SemanticException::vThrow("LIMIT value must be >= 0"_Stz);
    }

    if (pCtx->OFFSET()) {
      iOffset = iCleanInt(pCtx->offset);

      if (iOffset < -1) {
        SemanticException::vThrow("OFFSET value must be >= 0"_Stz);
      }
    }

    // Create the limit op
    QueryNode* pLimitOp = LimitOp::pCreate(iLimit, iOffset);

    // Insert the limit op as the top-most child underneath the statement.
    // Need to make any current children of statement now children of limitOp.
    QueryNode* pStatement = m_pStatementStack->pPeek(0);
    pStatement->vInsertAsParentOfChildren(pLimitOp);
  }

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* (Table) Fetch node                                                         */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitFetch(sqlParser::FetchContext *pCtx)
{
  visitChildren(pCtx);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Table Reference node                                                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitTableReference(sqlParser::TableReferenceContext *pCtx)
{
  visitChildren(pCtx);
  Text* pCatalogName = nullptr;
  Text* pSchemaName = nullptr;
  Text* pTableName = nullptr;
  
  if (pCtx->catalogName() != nullptr) {
    pCatalogName = pCleanId(pCtx->catalogName()->id);
  }
  
  if (pCtx->schemaName() != nullptr) {
    pSchemaName = pCleanId(pCtx->schemaName()->id);
  }
  
  ASSERT(pCtx->tableName() != nullptr);
  pTableName = pCleanId(pCtx->tableName()->id);

  QueryNode* pTableRef = TableRef::pCreate(pCatalogName, pSchemaName, pTableName);
  m_pTableRefStack->vPush(pTableRef);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select Expression node (for each select of each query/subquery)            */
/* Contains select list / FROM / WHERE / GROUP BY / HAVING                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectExpr(sqlParser::SelectExprContext* pCtx)
{
  ASSERT(m_pStatementStack->nItems() > 0);
  ASSERT(!m_pSelectItems);

  // N.B. We don't call visitChildren() in this function. Instead we manually
  // visit the required nodes, so if the grammar changes then we also need to
  // change this. The reason for this is so that we can appropriately separate
  // out how expressions are stored. e.g. we don't want select list exprs in
  // the same stack at the same time as WHERE expressions.
  // We also parse them in a slightly different order than is written. This
  // helps us to form the graph more easily.

  // -----------------------
  // Parenthetic SELECT Expr
  // -----------------------
  if (pCtx->selectExpr()) {
    return pCtx->selectExpr()->accept(this);
  }

  // -----------------------
  // DISTINCT keyword
  // -----------------------
  if (pCtx->distinct) {
    // TODO: implement
  }

  // -----------------------
  // Select List (Project)
  // -----------------------
  QueryNode *pProjectOp = nullptr;
  if (!pCtx->groupBy) {
    // If we have a group by clause then we will get this selectList there
    m_pSelectItems = new QueryNodeList;

    // Visit the child parse tree nodes for the selectList rule. The select list above will be populated.
    pCtx->selectList()->accept(this);

    ASSERT(m_pSelectItems->nItems() > 0);

    // Create a project operator. We will attach it to the statement object
    // at the end of this function.
    pProjectOp = ProjectOp::pCreate(m_pSelectItems);

    ASSERT(m_pSelectItems->nItems() == 0);
    delete m_pSelectItems;
    m_pSelectItems = nullptr;
  }


  // -----------------------
  // GROUP BY clause
  // -----------------------
  QueryNode* pAggregateOp = nullptr;
  if (pCtx->groupBy) {
    // First gather the select list items
      m_pSelectItems = new QueryNodeList;

      // Visit the child parse tree nodes for the selectList rule. The select list above will be populated.
      pCtx->selectList()->accept(this);

      ASSERT(m_pSelectItems->nItems() > 0);


    // Now gather the group by clause list
      ASSERT(!m_pExprList);
      m_pExprList = new QueryNodeList;

      // Visit the children of the GROUP BY clause
      pCtx->groupBy->accept(this);
      ASSERT(m_pExprList->nItems() > 0);

    // Create the aggregate operator
      pAggregateOp = AggregateOp::pCreate(m_pSelectItems, m_pExprList);

    // Clean up
      ASSERT(m_pSelectItems->nItems() == 0);
      delete m_pSelectItems;
      m_pSelectItems = nullptr;

      ASSERT(m_pExprList->nItems() == 0);
      delete m_pExprList;
      m_pExprList = nullptr;
  }

  // -----------------------
  // HAVING clause
  // -----------------------
  QueryNode* pHavingFilterOp = nullptr;
  if (pCtx->having) {

    // Check that the having clause is valid.
    // The expressions in a having clause must either be the grouping column
    // in the group by list or columns that are functionally dependant on
    // the grouping column.
    // TODO
//    b1 bHasAggregate = (pAggregateOp || (pProjectOp && ProjectOp::bHasAnAggregate(pProjectOp)));
//    if (!bHasAggregate) {
//      SemanticException::vThrow("Cannot specify a HAVING clause with no aggregate expressions"_Stz);
//    }

    ASSERT(!m_pExprList);
    m_pExprList = new QueryNodeList();

    // Visit the children of the HAVING clause
    pCtx->where->accept(this);
    ASSERT(m_pExprList->nItems() == 1);

    QueryNode* pExpr = m_pExprList->pPop();
    pHavingFilterOp = FilterOp::pCreate(pExpr);

    ASSERT(m_pExprList->nItems() == 0);
    delete m_pExprList;
    m_pExprList = nullptr;
  }


  // -----------------------
  // WHERE clause
  // -----------------------
  QueryNode* pWhereFilterOp = nullptr;
  if (pCtx->where) {
    ASSERT(!m_pExprList);
    m_pExprList = new QueryNodeList;

    // Visit the children of the WHERE clause
    pCtx->where->accept(this);
    ASSERT(m_pExprList->nItems() == 1);

    QueryNode* pExpr = m_pExprList->pPop();
    pWhereFilterOp = FilterOp::pCreate(pExpr);

    ASSERT(m_pExprList->nItems() == 0);
    delete m_pExprList;
    m_pExprList = nullptr;
  }


  // -----------------------
  // Table References
  // -----------------------
  // Check for a tableRef. If one exists, add it to a TableScanOp
  // and attach the TableScanOp as a child of the ProjectOp
  QueryNode* pTableScanOp = nullptr;
  if (pCtx->tableExpr()) {
    // Visit the grammar rule for table expressions
    pCtx->tableExpr()->accept(this);

    ASSERT(m_pTableRefStack->nItems() == 1);
    pTableScanOp = TableScanOp::pCreate(m_pTableRefStack->pPop());

  } else {
    SemanticException::vThrow("No table source specified"_Stz);
  }


  // -----------------------
  // Final Plan Assembly
  // -----------------------

  QueryNode *pFirstOp = nullptr;
  QueryNode *pLastOp = nullptr;

  /// PROJECT
  if(pProjectOp) {
    if (!pFirstOp)  pFirstOp = pProjectOp;
    if (pLastOp)    pLastOp->vAppendChild(pProjectOp);

    pLastOp = pProjectOp;
  }

  /// HAVING
  if (pHavingFilterOp) {
    if (!pFirstOp)  pFirstOp = pHavingFilterOp;
    if (pLastOp)    pLastOp->vAppendChild(pHavingFilterOp);

    pLastOp = pHavingFilterOp;
  }

  /// GROUP BY
  if (pAggregateOp) {
    if (!pFirstOp)  pFirstOp = pAggregateOp;
    if (pLastOp)    pLastOp->vAppendChild(pAggregateOp);

    pLastOp = pAggregateOp;
  }

  /// WHERE
  if (pWhereFilterOp) {
    if (!pFirstOp)  pFirstOp = pWhereFilterOp;
    if (pLastOp)    pLastOp->vAppendChild(pWhereFilterOp);

    pLastOp = pWhereFilterOp;
  }

  /// TABLE REF
  if (pTableScanOp) {
    if (!pFirstOp)  pFirstOp = pTableScanOp;
    if (pLastOp)    pLastOp->vAppendChild(pTableScanOp);

    pLastOp = pTableScanOp;
  }

  // Now attach the sql operator chain to the statement object.
  QueryNode* pStatement = m_pStatementStack->pPeek(0);
  ASSERT(Statement::bIsOfType(pStatement));
  pStatement->vAppendChild(pFirstOp);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select List node                                                           */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectList(sqlParser::SelectListContext *pCtx)
{
  ASSERT(m_pSelectItems);
  
  ASSERT(!m_pExprList);
  m_pExprList = new QueryNodeList;
  
  visitChildren(pCtx);
  
  ASSERT(m_pExprList->nItems() == 0);
  delete m_pExprList;
  m_pExprList = nullptr;
  
  ASSERT(m_pSelectItems->nItems() > 0);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select List Item node                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectListItem(sqlParser::SelectListItemContext* pCtx)
{
  visitChildren(pCtx);
  
  // pop the expr stack and add it to the select list.
  ASSERT(m_pExprList->nItems() == 1);
  
  QueryNode* pExprNode = m_pExprList->pPop();

  // Is there an alias?
  Text* pAliasText = pCleanId(pCtx->as);

  QueryNode* pProjectedExpr = ProjectedExpr::pCreate(pExprNode, pAliasText);

  m_pSelectItems->vAppend(pProjectedExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Column Reference node                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitColumnRef(sqlParser::ColumnRefContext* pCtx)
{
  ASSERT(m_pExprList);

  visitChildren(pCtx);
  
  Text* pCatalogName = pCtx->catalogName() == nullptr ? nullptr : pCleanId(pCtx->catalogName()->id);
  Text* pSchemaName = pCtx->schemaName() == nullptr ? nullptr : pCleanId(pCtx->schemaName()->id);
  Text* pTableName = pCtx->tableName() == nullptr ? nullptr : pCleanId(pCtx->tableName()->id);
  Text* pColumnName = pCleanId(pCtx->columnName()->id);

  QueryNode* pColumnRef = ColumnRef::pCreate(pCatalogName, pSchemaName,
                                             pTableName, pColumnName);

  m_pExprList->vPush(pColumnRef);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Column Reference node                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitColumnWildcard(sqlParser::ColumnWildcardContext* pCtx)
{
  ASSERT(m_pExprList);

  visitChildren(pCtx);

  Text* pCatalogName = pCtx->catalogName() == nullptr ? nullptr : pCleanId(pCtx->catalogName()->id);
  Text* pSchemaName = pCtx->schemaName() == nullptr ? nullptr : pCleanId(pCtx->schemaName()->id);
  Text* pTableName = pCtx->tableName() == nullptr ? nullptr : pCleanId(pCtx->tableName()->id);

  QueryNode* pColumnWildcard = ColumnWildcard::pCreate(pCatalogName, pSchemaName, pTableName);

  m_pExprList->vPush(pColumnWildcard);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Expression Literal node                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitLiteral(sqlParser::LiteralContext* pCtx)
{
  ASSERT(m_pExprList);

  visitChildren(pCtx);

  QueryNode* pLiteral;

  if (pCtx->SQL_NULL()) {
    pLiteral = NullLiteral::pCreate();

  } else if (pCtx->stringLiteral()) {
    Text* pStr = pCleanString(pCtx->stringLiteral());
    pLiteral = StringLiteral::pCreate(pStr);

  } else if (pCtx->booleanLiteral()) {
    b1 bVal = pCtx->booleanLiteral()->SQL_TRUE() != nullptr;
    pLiteral = BooleanLiteral::pCreate(bVal);

  } else if (pCtx->integerLiteral()) {
    i8 iVal = iCleanInt(pCtx->integerLiteral());
    pLiteral = IntegerLiteral::pCreate(iVal);

//  } else if (pCtx->decimalLiteral()) {
//    NOT_IMPLEMENTED_YET
//
  } else if (pCtx->floatLiteral()) {
    d8 dVal = dCleanFloat(pCtx->floatLiteral());
    pLiteral = FloatLiteral::pCreate(dVal);

  } else if (pCtx->dateLiteral()) {
    Dte xVal = xCleanDate(pCtx->dateLiteral()->stringLiteral());
    pLiteral = DateLiteral::pCreate(xVal);

  } else if (pCtx->timeLiteral()) {
    Tme xVal = xCleanTime(pCtx->timeLiteral()->stringLiteral());
    pLiteral = TimeLiteral::pCreate(xVal);

  } else if (pCtx->timestampLiteral()) {
    Tsp xVal = xCleanTimestamp(pCtx->timestampLiteral()->stringLiteral());
    pLiteral = TimestampLiteral::pCreate(xVal);

  } else if (pCtx->intervalLiteral()) {
    NOT_IMPLEMENTED_YET

  } else {
    NEVER_REACH
  }

  m_pExprList->vPush(pLiteral);
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Unary Expression Operators                                                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitUnaryOp(sqlParser::UnaryOpContext *pCtx)
{
  visitChildren(pCtx);

  QueryNode* pOpExpr = nullptr;
  QueryNode* pExpr1 = m_pExprList->pPop();

  // Get the string of the operator
  s256 sOp;
  vCleanString(pCtx->op, sOp);

  if (sOp.bEqual("+"_Stz)) {
    // We don't create the identity operator, just push the expression
    // back onto the stack again.
    pOpExpr = pExpr1;

  } else if (sOp.bEqual("-"_Stz)) {
    // Negate the expression on the stack
    pOpExpr = NegationExpr::pCreate(pExpr1);

  } else {
    NEVER_REACH
  }

  // Put the operator back onto the expression stack
  m_pExprList->vPush(pOpExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Binary Expression Operators                                                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitBinaryOp(sqlParser::BinaryOpContext *pCtx)
{
  visitChildren(pCtx);

  QueryNode* pOpExpr = nullptr;
  QueryNode* pExpr2 = m_pExprList->pPop();
  QueryNode* pExpr1 = m_pExprList->pPop();

  // Get the string of the operator
  s256 sOp;
  vCleanString(pCtx->op, sOp);

  if (sOp.bEqual("+"_Stz)) {
    pOpExpr = AdditionExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("-"_Stz)) {
    pOpExpr = SubtractionExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("*"_Stz)) {
    pOpExpr = MultiplicationExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("/"_Stz)) {
    pOpExpr = DivisionExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("%"_Stz)) {
    pOpExpr = ModulusExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("<"_Stz)) {
    pOpExpr = LtExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("<="_Stz)) {
    pOpExpr = LeExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("="_Stz)) {
    pOpExpr = EqExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("!="_Stz)) {
    pOpExpr = NeExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("<>"_Stz)) {
    pOpExpr = NeExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual(">="_Stz)) {
    pOpExpr = GeExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual(">"_Stz)) {
    pOpExpr = GtExpr::pCreate(pExpr1, pExpr2);

//  } else if (sOp.bEqual("||"_Stz)) {                  // concatenation
//    pOpExpr = ConcatExpr::pCreate(pExpr1, pExpr2);

  } else {
    NEVER_REACH
  }

  // Put the operator back onto the expression stack
  m_pExprList->vPush(pOpExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Logic Expression Operators                                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitLogicOp(sqlParser::LogicOpContext *pCtx)
{
  visitChildren(pCtx);

  QueryNode* pOpExpr = nullptr;
  QueryNode* pExpr2;
  QueryNode* pExpr1;

  // Get the string of the operator
  s256 sOp;
  vCleanString(pCtx->op, sOp);
  sOp.vToUpper();

  if (sOp.bEqual("AND"_Stz)) {
    pExpr2 = m_pExprList->pPop();
    pExpr1 = m_pExprList->pPop();
    pOpExpr = AndExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("OR"_Stz)) {
    pExpr2 = m_pExprList->pPop();
    pExpr1 = m_pExprList->pPop();
    pOpExpr = OrExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("XOR"_Stz)) {
    pExpr2 = m_pExprList->pPop();
    pExpr1 = m_pExprList->pPop();
    pOpExpr = XorExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("NOT"_Stz)) {
    pExpr1 = m_pExprList->pPop();
    pOpExpr = NotExpr::pCreate(pExpr1);

  } else {
    NEVER_REACH
  }

  // Put the operator back onto the expression stack
  m_pExprList->vPush(pOpExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* IsNull Expression Operator                                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitIsNullOp(sqlParser::IsNullOpContext *pCtx)
{
  visitChildren(pCtx);

  QueryNode* pOpExpr = nullptr;
  QueryNode* pExpr1 = m_pExprList->pPop();

  pOpExpr = IsNullExpr::pCreate(pExpr1);

  // Turn "IS NOT NULL <expr>" into NOT(<expr> IS NULL)
  if (pCtx->NOT()) {
    pOpExpr = NotExpr::pCreate(pOpExpr);
  }

  // Put the operator back onto the expression stack
  m_pExprList->vPush(pOpExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* COUNT(*) Expression Operator                                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitCountAll(sqlParser::CountAllContext *pCtx)
{
  visitChildren(pCtx);

  QueryNode* pOpExpr = CountExpr::pCreate(true, false, nullptr);

  // Put the operator back onto the expression stack
  m_pExprList->vPush(pOpExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Generic Aggregate Expression Operator                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitAggFunc(sqlParser::AggFuncContext *pCtx)
{
  visitChildren(pCtx);

  b1 bIsDistinct = (pCtx->distinct != nullptr);

  ASSERT(m_pExprList->nItems() > 0);
  QueryNode* pChildExpr = m_pExprList->pPop();

  s256 sFunc;
  vCleanString(pCtx->func, sFunc);
  sFunc.vToUpper();

  QueryNode* pAggExpr = nullptr;

  if (sFunc.bEqual("COUNT"_Stz)) {
    pAggExpr = CountExpr::pCreate(false, bIsDistinct, pChildExpr);

  } else if (sFunc.bEqual("SUM"_Stz)) {
    pAggExpr = SumExpr::pCreate(bIsDistinct, pChildExpr);

  } else if (sFunc.bEqual("AVG"_Stz)) {
    pAggExpr = AvgExpr::pCreate(bIsDistinct, pChildExpr);

  } else if (sFunc.bEqual("MIN"_Stz)) {
    pAggExpr = MinExpr::pCreate(bIsDistinct, pChildExpr);

  } else if (sFunc.bEqual("MAX"_Stz)) {
    pAggExpr = MaxExpr::pCreate(bIsDistinct, pChildExpr);

  } else {
    SyntaxException::vThrow(pCtx->func->getLine(),
                            pCtx->func->getCharPositionInLine(),
                            "Unrecognised aggregate function: %s"_Stz, sFunc.szStz());
  }

  ASSERT(pAggExpr);

  // Put the operator back onto the expression stack
  m_pExprList->vPush(pAggExpr);

  return nullptr;
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Clean up a name of an object. Returned string is dynamically allocated.    */
/*----------------------------------------------------------------------------*/
Text* SqlPlanner::pCleanId(sqlParser::IdentifierContext *pCtx)
{
  if (!pCtx) {
    return nullptr;
  }

  std::string idStr = pCtx->getText();
  
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

/*----------------------------------------------------------------------------*/
/* Clean up a a string literal. Returned string is dynamically allocated.     */
/*----------------------------------------------------------------------------*/
Text* SqlPlanner::pCleanString(sqlParser::StringLiteralContext *pCtx)
{
  if (!pCtx) {
    return nullptr;
  }

  std::string idStr = pCtx->getText();

  i8 idLen = idStr.length();

  if (idLen == 0) {
    return new Text(""_Stz);
  }

  // Remove single quotes
  if (idStr.at(0) == '\'' && idStr.at(idLen - 1) == '\'') {
    // Not great, substr() makes a another copy of the string.
    Text* pText = new Text((Stz*) idStr.substr(1, idLen - 2).c_str());

    // Unescape any two consecutive single quotes
    while (StrLib::bReplace(pText->szGet()->azChar(), "''", "'")) { /*noop*/ }
    pText->vSizeText();

    return pText;
  }

  // Remove double quotes
  if (idStr.at(0) == '"' && idStr.at(idLen - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    Text* pText = new Text((Stz*) idStr.substr(1, idLen - 2).c_str());

    // Unescape any two consecutive double quotes
    while (StrLib::bReplace(pText->szGet()->azChar(), "\"\"", "\"")) { /*noop*/ }
    pText->vSizeText();

    return pText;
  }

  return new Text((Stz*) idStr.c_str());
}

/*----------------------------------------------------------------------------*/
/* Clean up a a string literal. Returned string is written to the given buffer*/
/*----------------------------------------------------------------------------*/
void SqlPlanner::vCleanString(sqlParser::StringLiteralContext *pCtx, s256& sBuf)
{
  if (!pCtx) {
    return;
  }

  std::string idStr = pCtx->getText();
  vCleanString(idStr, sBuf);
}

/*----------------------------------------------------------------------------*/
/* Clean up a a string literal. Returned string is written to the given buffer*/
/*----------------------------------------------------------------------------*/
void SqlPlanner::vCleanString(sqlParser::IdentifierContext *pCtx, s256& sBuf)
{
  if (!pCtx) {
    return;
  }

  std::string idStr = pCtx->getText();
  vCleanString(idStr, sBuf);
}

/*----------------------------------------------------------------------------*/
/* Clean up a a string literal. Returned string is written to the given buffer*/
/*----------------------------------------------------------------------------*/
void SqlPlanner::vCleanString(antlr4::Token* pToken, s256& sBuf)
{
  if (!pToken) {
    return;
  }

  std::string sStr = pToken->getText();
  vCleanString(sStr, sBuf);
}

/*----------------------------------------------------------------------------*/
/* Clean up a a string literal. Returned string is written to the given buffer*/
/*----------------------------------------------------------------------------*/
void SqlPlanner::vCleanString(std::string& sStr, s256& sBuf)
{
  i8 len = sStr.length();

  if (len == 0) {
    return;
  }

  // Ensure that the string is short enough to fit in the buffer
  RASSERT(len < sBuf.nSpace());

  // Remove single quotes
  if (sStr.at(0) == '\'' && sStr.at(len - 1) == '\'') {
    // Not great, substr() makes a another copy of the string.
    sBuf.vCopy((Stz*) sStr.substr(1, len - 2).c_str());

    // Unescape any two consecutive single quotes
    while (StrLib::bReplace(sBuf.azChar(), "''", "'")) { /*noop*/ }
    return;
  }

  // Remove double quotes
  else if (sStr.at(0) == '"' && sStr.at(len - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    sBuf.vCopy((Stz*) sStr.substr(1, len - 2).c_str());

    // Unescape any two consecutive double quotes
    while (StrLib::bReplace(sBuf.azChar(), "\"\"", "\"")) { /*noop*/ }
    return;


  } else {
    // No quotes. Just copy the string
    sBuf.vCopy((Stz*) sStr.c_str());
  }
}

/*----------------------------------------------------------------------------*/
/* Return an integer value from an integer literal token                      */
/*----------------------------------------------------------------------------*/
i8 SqlPlanner::iCleanInt(sqlParser::IntegerLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  return iCleanInt(sStr);
}

/*----------------------------------------------------------------------------*/
/* Return an integer value from an integer literal token                      */
/*----------------------------------------------------------------------------*/
i8 SqlPlanner::iCleanInt(antlr4::Token* pToken)
{
  ASSERT(pToken);

  std::string sStr = pToken->getText();
  return iCleanInt(sStr);
}

/*----------------------------------------------------------------------------*/
/* Return an integer value from an integer literal token                      */
/*----------------------------------------------------------------------------*/
i8 SqlPlanner::iCleanInt(std::string& sStr)
{
  i8 iVal;

  try {
    iVal = std::stoll(sStr);

  } catch (std::invalid_argument &e) {
    SemanticException::vThrow("Cannot convert value\"%s\" to an integer"_Stz, sStr.c_str());

  } catch (std::out_of_range &e) {
    SemanticException::vThrow("Cannot convert value\"%s\" to an integer as it is out of range"_Stz, sStr.c_str());
  }

  return iVal;
}

/*----------------------------------------------------------------------------*/
/* Return a floating point value (approximate numeric) from a float token     */
/*----------------------------------------------------------------------------*/
d8 SqlPlanner::dCleanFloat(sqlParser::FloatLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  d8 dVal;

  try {
    dVal = std::stod(sStr);

  } catch (std::invalid_argument &e) {
    SemanticException::vThrow("Cannot convert value\"%s\" to a float"_Stz, sStr.c_str());

  } catch (std::out_of_range &e) {
    SemanticException::vThrow("Cannot convert value\"%s\" to a float as it is out of range"_Stz, sStr.c_str());
  }

  return dVal;
}

/*----------------------------------------------------------------------------*/
/* Return a decimal value (exact numeric) from a decimal literal token        */
/*----------------------------------------------------------------------------*/
//?? SqlPlanner::cleanDecimal(sqlParser::DecimalLiteralContext *pCtx)
//{
//  ASSERT(pCtx);
//
//  std::string sStr = pCtx->getText();
//  ????
//}

/*----------------------------------------------------------------------------*/
/* Return a date value from a date token                                      */
/*----------------------------------------------------------------------------*/
Dte SqlPlanner::xCleanDate(sqlParser::StringLiteralContext *pCtx)
{
  ASSERT(pCtx);

  static const Stz* _szInvalidFormat =
      "Invalid date string format. Must be of the form: 'YYYY-MM-DD'"_Stz;

  // First check that it can fit into the 256 buffer
  if (pCtx->getText().length() > 255) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  if (sBuf.nLength() != 10) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[4] != '-' || pcBuf[7] != '-') {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  if (!StrLib::bIsDigit(pcBuf[0]) ||
      !StrLib::bIsDigit(pcBuf[1]) ||
      !StrLib::bIsDigit(pcBuf[2]) ||
      !StrLib::bIsDigit(pcBuf[3]) ||
      !StrLib::bIsDigit(pcBuf[5]) ||
      !StrLib::bIsDigit(pcBuf[6]) ||
      !StrLib::bIsDigit(pcBuf[8]) ||
      !StrLib::bIsDigit(pcBuf[9]) ) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  // Parse out the fields
  i8 iYear  = 0;
  i8 iMonth = 0;
  i8 iDay   = 0;

  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[0]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[1]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[2]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[3]);

  iMonth *= 10; iMonth += StrLib::iDigitToI8(pcBuf[5]);
  iMonth *= 10; iMonth += StrLib::iDigitToI8(pcBuf[6]);

  iDay *= 10; iDay += StrLib::iDigitToI8(pcBuf[8]);
  iDay *= 10; iDay += StrLib::iDigitToI8(pcBuf[9]);

  // Now check the values
  s256 sErrorMsg;
  if (!Dte::bIsValid(iYear, iMonth, iDay, sErrorMsg)) {
    throw SemanticException(sErrorMsg.szStz());
  }

  Dte xVal;
  xVal.vSet(iYear, iMonth, iDay);
  return xVal;
}

/*----------------------------------------------------------------------------*/
/* Return a time value from a time token                                      */
/*----------------------------------------------------------------------------*/
Tme SqlPlanner::xCleanTime(sqlParser::StringLiteralContext *pCtx)
{
  ASSERT(pCtx);

  static const Stz* _szInvalidFormat =
      "Invalid time string format. Must be of the form: 'hh:mm:ss'"_Stz;

  // First check that it can fit into the 256 buffer
  if (pCtx->getText().length() > 255) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  if (sBuf.nLength() != 8) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[2] != ':' || pcBuf[5] != ':') {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  if (!StrLib::bIsDigit(pcBuf[0]) ||
      !StrLib::bIsDigit(pcBuf[1]) ||
      !StrLib::bIsDigit(pcBuf[3]) ||
      !StrLib::bIsDigit(pcBuf[4]) ||
      !StrLib::bIsDigit(pcBuf[6]) ||
      !StrLib::bIsDigit(pcBuf[7]) ) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  // Parse out the fields
  i8 iHour = 0;
  i8 iMin  = 0;
  i8 iSec  = 0;

  iHour *= 10; iHour += StrLib::iDigitToI8(pcBuf[0]);
  iHour *= 10; iHour += StrLib::iDigitToI8(pcBuf[1]);

  iMin *= 10; iMin += StrLib::iDigitToI8(pcBuf[3]);
  iMin *= 10; iMin += StrLib::iDigitToI8(pcBuf[4]);

  iSec *= 10; iSec += StrLib::iDigitToI8(pcBuf[6]);
  iSec *= 10; iSec += StrLib::iDigitToI8(pcBuf[7]);

  // Now check the values
  s256 sErrorMsg;
  if (!Tme::bIsValid(iHour, iMin, iSec, sErrorMsg)) {
    throw SemanticException(sErrorMsg.szStz());
  }

  Tme xVal;
  xVal.vSet(iHour, iMin, iSec, 0);
  return xVal;
}

/*----------------------------------------------------------------------------*/
/* Return a timestamp value from a timestamp token                            */
/*----------------------------------------------------------------------------*/
Tsp SqlPlanner::xCleanTimestamp(sqlParser::StringLiteralContext *pCtx)
{
  ASSERT(pCtx);

  static const Stz* _szInvalidFormat =
      "Invalid timestamp string format. Must be of the form: 'YYYY-MM-DD hh:mm:ss[.ffffff]'"_Stz;

  // First check that it can fit into the 256 buffer
  if (pCtx->getText().length() > 255) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  i8 iLen = sBuf.nLength();
  if (iLen < 19) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[4] != '-' || pcBuf[7] != '-' || pcBuf[10] != ' ' || pcBuf[13] != ':' || pcBuf[16] != ':') {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  if (!StrLib::bIsDigit(pcBuf[0])  ||
      !StrLib::bIsDigit(pcBuf[1])  ||
      !StrLib::bIsDigit(pcBuf[2])  ||
      !StrLib::bIsDigit(pcBuf[3])  ||
      !StrLib::bIsDigit(pcBuf[5])  ||
      !StrLib::bIsDigit(pcBuf[6])  ||
      !StrLib::bIsDigit(pcBuf[8])  ||
      !StrLib::bIsDigit(pcBuf[9])  ||
      !StrLib::bIsDigit(pcBuf[11]) ||
      !StrLib::bIsDigit(pcBuf[12]) ||
      !StrLib::bIsDigit(pcBuf[14]) ||
      !StrLib::bIsDigit(pcBuf[15]) ||
      !StrLib::bIsDigit(pcBuf[17]) ||
      !StrLib::bIsDigit(pcBuf[18]) ) {
    throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
  }

  // check for optional micros
  b1 bHasMicros = false;

  if (iLen > 19) {
    if (pcBuf[19] != '.') {
      throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
    }

    for (i8 iIdx = 20; iIdx < iLen; iIdx++) {
      if (iIdx > 25) {
        // can only have 6 digits of microseconds
        throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
      }

      if (!StrLib::bIsDigit(pcBuf[iIdx])) {
        throw SyntaxException(pCtx->start->getLine(), pCtx->start->getCharPositionInLine(), _szInvalidFormat);
      }
    }

    bHasMicros = true;
  }

  // Parse out the fields
  i8 iYear  = 0;
  i8 iMonth = 0;
  i8 iDay   = 0;

  i8 iHour = 0;
  i8 iMin  = 0;
  i8 iSec  = 0;
  i8 iMicro = 0;

  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[0]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[1]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[2]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[3]);

  iMonth *= 10; iMonth += StrLib::iDigitToI8(pcBuf[5]);
  iMonth *= 10; iMonth += StrLib::iDigitToI8(pcBuf[6]);

  iDay *= 10; iDay += StrLib::iDigitToI8(pcBuf[8]);
  iDay *= 10; iDay += StrLib::iDigitToI8(pcBuf[9]);

  iHour *= 10; iHour += StrLib::iDigitToI8(pcBuf[11]);
  iHour *= 10; iHour += StrLib::iDigitToI8(pcBuf[12]);

  iMin *= 10; iMin += StrLib::iDigitToI8(pcBuf[14]);
  iMin *= 10; iMin += StrLib::iDigitToI8(pcBuf[15]);

  iSec *= 10; iSec += StrLib::iDigitToI8(pcBuf[17]);
  iSec *= 10; iSec += StrLib::iDigitToI8(pcBuf[18]);

  if (bHasMicros) {
    for (i8 iIdx = 20; iIdx < iLen; iIdx++) {
      iMicro *= 10;
      iMicro += StrLib::iDigitToI8(pcBuf[iIdx]);
    }
  }

  // Now check the values
  s256 sErrorMsg;
  if (!Tsp::bIsValid(iYear, iMonth, iDay, iHour, iMin, iSec, iMicro, sErrorMsg)) {
    throw SemanticException(sErrorMsg.szStz());
  }

  Tsp xVal;
  xVal.vSet(iYear, iMonth, iDay, iHour, iMin, iSec, iMicro);
  return xVal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
