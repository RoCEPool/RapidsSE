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
#include "store/parser/shadowNodes/planObjects/statementMarkers/SelectMarker.hpp"
#include "store/parser/shadowNodes/planObjects/statementMarkers/ExplainMarker.hpp"
#include "store/parser/shadowNodes/planObjects/FilterOp.hpp"
#include "store/parser/shadowNodes/planObjects/LimitOp.hpp"
#include "store/parser/shadowNodes/planObjects/ProjectOp.hpp"
#include "store/parser/shadowNodes/planObjects/AggregateOp.hpp"
#include "store/parser/shadowNodes/planObjects/dataSources/TableScanOp.hpp"
#include "store/parser/shadowNodes/planObjects/UnionOp.hpp"
#include "store/parser/shadowNodes/planObjects/ExplainOp.hpp"
#include "store/parser/shadowNodes/planObjects/OrderOp.hpp"

#include "store/parser/shadowNodes/compound/TableRef.hpp"
#include "store/parser/shadowNodes/valueObjects/StringObject.hpp"
#include "store/parser/shadowNodes/valueObjects/IntegerObject.hpp"
#include "store/parser/shadowNodes/lists/StringList.hpp"
#include "store/parser/shadowNodes/lists/StageRangeList.hpp"
#include "store/parser/shadowNodes/compound/StageRange.hpp"
#include "store/parser/shadowNodes/compound/OrderExpr.hpp"
#include "store/parser/shadowNodes/lists/OrderExprList.hpp"

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

#include "store/parser/shadowNodes/compound/ProjectedColumn.hpp"

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
  m_pSelectItems(nullptr),
  m_pOrderExprs(nullptr)
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

  if (m_pOrderExprs) {
    while (m_pOrderExprs->nItems() > 0) {
      QueryNode* pItem = m_pOrderExprs->pPop();
      delete pItem;
    }
    delete m_pOrderExprs;
    m_pOrderExprs = nullptr;
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
    SyntaxException::vThrowAntlr(errorListener.iLine(), errorListener.iColumn(), errorListener.szErrorMsg());
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
  QueryNode* pQueryNode = _pQueryNode(pItem);

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
/* Explain node                                                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitExplainStatement(sqlParser::ExplainStatementContext *pCtx)
{
  // N.B. Manual visitation of children used at this level.

  // Get the statement string for this part of the query
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  // Create an explain StatementMarker and add it to the statement stack
  QueryNode* pExplainMarker = ExplainMarker::pCreate((Stz*)(queryStlStr.c_str()));
  m_pStatementStack->vPush(pExplainMarker);

  Text* pFormat = nullptr;
  QueryNode* pStageList = nullptr;

  for (sqlParser::ExplainOptionsContext* pOptionsCtx : pCtx->explainOptions()) {
    if (pOptionsCtx->explainFormat()) {

      if (pFormat) {
        delete pFormat;
        pFormat = nullptr;
      }

      pFormat = pOptionsCtx->explainFormat()->accept(this).as<Text*>();

    } else if (pOptionsCtx->explainStageList()) {

      if (pStageList) {
        delete pStageList;
        pStageList = nullptr;
      }

      pStageList = pOptionsCtx->explainStageList()->accept(this).as<QueryNode*>();
      ASSERT(StageRangeList::bIsOfType(pStageList));

    } else {
      NEVER_REACH
    }
  }

  if (!pStageList) {
    // Create a state list with the stage range 'last'
    pStageList = StageRangeList::pCreate();

    QueryNode* pRange = StageRange::pCreateLastOnly();
    pStageList->vAppendChild(pRange);
  }

  // Create an ExplainOp and add it as a child of the ExplainMarker
  QueryNode* pExplainOp = ExplainOp::pCreate(pStageList, pFormat == nullptr ? nullptr : pFormat->szGet());
  pExplainMarker->vAppendChild(pExplainOp);
  delete pFormat;

  // Visit the statement choice node. This will add a new statementMarker to the statement list.
  // We can then pop it off and add it as a child of the ExplainOp.
  pCtx->statementChoice()->accept(this);

  ASSERT(m_pStatementStack->nItems() >= 2);
  QueryNode* pExplainableStatement = m_pStatementStack->pPop();
  ASSERT(m_pStatementStack->pTop() == pExplainMarker);
  pExplainOp->vAppendChild(pExplainableStatement);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Explain Format                                                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitExplainFormat(sqlParser::ExplainFormatContext *pCtx)
{
  if (pCtx->PRETTY()) {
    return new Text("PRETTY"_Stz);

  } else if (pCtx->ROW()) {
    return new Text("ROW"_Stz);
  } else {
    NEVER_REACH
  }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Explain StageRangeList                                                     */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitExplainStageList(sqlParser::ExplainStageListContext *pCtx)
{
  // N.B. Manual visitation of children used at this level.

  QueryNode* pStageList = StageRangeList::pCreate();

  for (sqlParser::ExplainRangeContext* pRangeCtx : pCtx->explainRange()) {
    // Visit each explain range and add it to the list
    QueryNode* pRange = pRangeCtx->accept(this).as<QueryNode*>();
    ASSERT(StageRange::bIsOfType(pRange));

    pStageList->vAppendChild(pRange);
  }

  return pStageList;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Explain Stage Range                                                        */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitExplainRange(sqlParser::ExplainRangeContext *pCtx)
{
  // N.B. Manual visitation of children used at this level.

  QueryNode* pBegin;
  QueryNode* pEnd = nullptr;

  pBegin = pCtx->begin->accept(this).as<QueryNode*>();
  ASSERT(ValueObject::bIsOfType(pBegin));

  if (pCtx->end) {
    pEnd = pCtx->end->accept(this).as<QueryNode*>();
    ASSERT(ValueObject::bIsOfType(pEnd));
  }

  QueryNode* pRange = StageRange::pCreate(pBegin, pEnd);
  return pRange;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Explain Marker                                                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitExplainRangeChoice(sqlParser::ExplainRangeChoiceContext *pCtx)
{
  // N.B. Manual visitation of children used at this level.

  QueryNode* pMarker;

  if (pCtx->all) {
    pMarker = StringObject::pCreate(new Text(StageRange::C_RANGE_ALL));

  } else if (pCtx->id) {
    Text* pText = pCleanId(pCtx->id);
    pMarker = StringObject::pCreate(pText);

  } else if (pCtx->num) {
    i8 iVal = iCleanInt(pCtx->num);
    pMarker = IntegerObject::pCreate(iVal);

  } else {
    NEVER_REACH
  }

  return pMarker;
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
  // Get the statement string for this part of the query
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  QueryNode* pStmt = SelectMarker::pCreate((Stz*)(queryStlStr.c_str()));
  m_pStatementStack->vPush(pStmt);

  if (pCtx->ORDER()) {
    ASSERT(!m_pOrderExprs);
    m_pOrderExprs = new QueryNodeList();
  }

  visitChildren(pCtx);

  if (pCtx->ORDER()) {
    ASSERT(m_pOrderExprs);
    ASSERT(m_pOrderExprs->nItems() > 0);

    // Convert the QueryNodeList to a OrderExprList
    QueryNode* pOrderExprList = OrderExprList::pCreate();
    QueryNode* pOrderExpr;
    while ((pOrderExpr = m_pOrderExprs->pPop()) != nullptr) {
      pOrderExprList->vAppendChild(pOrderExpr);
    }

    // Create the OrderOp
    QueryNode* pOrderOp = OrderOp::pCreate(pOrderExprList);

    // Insert the orderOp as the top-most child underneath the statement.
    // Need to make any current children of statement now children of orderOp.
    QueryNode* pStatement = m_pStatementStack->pTop();
    pStatement->vInsertAsParentOfChildren(pOrderOp);

    ASSERT(m_pOrderExprs->nItems() == 0);
    delete m_pOrderExprs;
    m_pOrderExprs = nullptr;
  }

  if (pCtx->LIMIT()) {
    i8 iLimit = iCleanInt(pCtx->limit);
    i8 iOffset = -1;

    if (iLimit < 0) {
      SemanticException::vThrow(EC_INVALID_LIMIT_VALUE);
    }

    if (pCtx->OFFSET()) {
      iOffset = iCleanInt(pCtx->offset);

      if (iOffset < -1) {
        SemanticException::vThrow(EC_INVALID_OFFSET_VALUE);
      }
    }

    // Create the limit op
    QueryNode* pLimitOp = LimitOp::pCreate(iLimit, iOffset);

    // Insert the limit op as the top-most child underneath the statement.
    // Need to make any current children of statement now children of limitOp.
    QueryNode* pStatement = m_pStatementStack->pTop();
    pStatement->vInsertAsParentOfChildren(pLimitOp);
  }

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Set operations (union, intersect, except) over select queries              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSetQuery(sqlParser::SetQueryContext *pCtx)
{
  if (pCtx->ORDER()) {
    ASSERT(!m_pOrderExprs);
    m_pOrderExprs = new QueryNodeList();
  }

  visitChildren(pCtx);

  ASSERT(SelectMarker::bIsOfType(m_pStatementStack->pTop()));
  ASSERT(m_pStatementStack->nItems() >= 1);

  if (pCtx->ORDER() || pCtx->LIMIT()) {
    // We need to stick another statementOp on top of the one currently on the stack
    QueryNode* pSubStatement = m_pStatementStack->pPop();
    ASSERT(pSubStatement);

    // Get the statement string for this part of the query
    antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
    std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

    QueryNode *pStatement = SelectMarker::pCreate((Stz *) (queryStlStr.c_str()));
    pStatement->vAppendChild(pSubStatement);
    m_pStatementStack->vPush(pStatement);
  }

  if (pCtx->ORDER()) {
    ASSERT(m_pOrderExprs);
    ASSERT(m_pOrderExprs->nItems() > 0);

    // Convert the QueryNodeList to a OrderExprList
    QueryNode* pOrderExprList = OrderExprList::pCreate();
    QueryNode* pOrderExpr;
    while ((pOrderExpr = m_pOrderExprs->pPop()) != nullptr) {
      pOrderExprList->vAppendChild(pOrderExpr);
    }

    // Create the OrderOp
    QueryNode* pOrderOp = OrderOp::pCreate(pOrderExprList);

    // Insert the orderOp as the top-most child underneath the statement.
    // Need to make any current children of statement now children of orderOp.
    QueryNode* pStatement = m_pStatementStack->pTop();
    pStatement->vInsertAsParentOfChildren(pOrderOp);

    ASSERT(m_pOrderExprs->nItems() == 0);
    delete m_pOrderExprs;
    m_pOrderExprs = nullptr;
  }

  if (pCtx->LIMIT()) {
    i8 iLimit = iCleanInt(pCtx->limit);
    i8 iOffset = -1;

    if (iLimit < 0) {
      SemanticException::vThrow(EC_INVALID_LIMIT_VALUE);
    }

    if (pCtx->OFFSET()) {
      iOffset = iCleanInt(pCtx->offset);

      if (iOffset < -1) {
        SemanticException::vThrow(EC_INVALID_OFFSET_VALUE);
      }
    }

    // Create the limit op
    QueryNode* pLimitOp = LimitOp::pCreate(iLimit, iOffset);

    // Insert the limit op as the top-most child underneath the statement.
    // Need to make any current children of statement now children of limitOp.
    QueryNode* pStatement = m_pStatementStack->pTop();
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
/* Set Expression node                                                        */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSetExpr(sqlParser::SetExprContext* pCtx)
{
  ASSERT(!m_pSelectItems);

  if (pCtx->selectExpr()) {
    // Add a SelectMarker to the statement stack. After visiting children
    // then we can pop the entire logical subgraph off the statement stack
    // and add them as children to a union/intersect/except operator.

    // Get the statement string for this part of the query
    antlr4::misc::Interval interval(pCtx->selectExpr()->start->getStartIndex(),
                                    pCtx->selectExpr()->stop->getStopIndex());
    std::string queryStlStr = pCtx->selectExpr()->start->getInputStream()->getText(interval);

    QueryNode *pStatement = SelectMarker::pCreate((Stz *) (queryStlStr.c_str()));
    m_pStatementStack->vPush(pStatement);
  }

  visitChildren(pCtx);

  if (pCtx->op == nullptr) {
    // Nothing more to do here, we're nested
    return nullptr;
  }

  // Got a set operation.
  // First get any properties of it.

  b1 bIsDistinct = (pCtx->all == nullptr);

  if (bIsDistinct) {
    s256 sBuf;
    sBuf.vCatSprint("%s DISTINCT"_Stz, pCtx->op->getText().c_str());
    __NOT_IMPL2(sBuf.szStz())
  }

  QueryNode* pCorrespondingByList = nullptr;

  if (pCtx->corrClause()) {
    pCorrespondingByList = StringList::pCreate();

    // The vector of corresponding by list items
    if (pCtx->corrClause()->corrList()) {
      std::vector<sqlParser::CorrListItemContext *> xItemCtxVector = pCtx->corrClause()->corrList()->corrListItem();

      // Iterate the item contexts, extract the column names, create nodes to hold
      // these string values and add them to the correspondingByList
      for (sqlParser::CorrListItemContext *pItemCtx : xItemCtxVector) {
        Text *pColumnName = pCleanId(pItemCtx->id);
        QueryNode *pColumnNode = StringObject::pCreate(pColumnName);
        pCorrespondingByList->vAppendChild(pColumnNode);
      }
    }
  }

  // Get the actual set operation type and create it.
  QueryNode* pSetOp;

  switch (pCtx->op->getType()) {

    case sqlLexer::UNION:
      pSetOp = UnionOp::pCreate(bIsDistinct, pCorrespondingByList);
      break;

    case sqlLexer::INTERSECT:
      __NOT_IMPL("INTERSECT")
      break;

    case sqlLexer::EXCEPT:
      __NOT_IMPL("EXCEPT")
      break;

    default:
      NEVER_REACH
  }

  // Add the children. They should have been added to the statement list
  ASSERT(m_pStatementStack->nItems() >= 2);
  QueryNode* pChild2 = m_pStatementStack->pPop();
  QueryNode* pChild1 = m_pStatementStack->pPop();

  pSetOp->vAppendChild(pChild1);
  pSetOp->vAppendChild(pChild2);

  // Create a SelectMarker and add this set operator to it
  // Get the statement string for this part of the query
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  QueryNode *pStatement = SelectMarker::pCreate((Stz *) (queryStlStr.c_str()));
  pStatement->vAppendChild(pSetOp);
  m_pStatementStack->vPush(pStatement);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select Expression node (for each select of each query/subquery)            */
/* Contains select list / FROM / WHERE / GROUP BY / HAVING                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectExpr(sqlParser::SelectExprContext* pCtx)
{
  ASSERT(m_pStatementStack->nItems() > 0);
  ASSERT(SelectMarker::bIsOfType(m_pStatementStack->pTop()));
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
    __NOT_IMPL("DISTINCT")
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

    ASSERT(!m_pExprList);
    m_pExprList = new QueryNodeList();

    // Visit the children of the HAVING clause
    pCtx->having->accept(this);
    ASSERT(m_pExprList->nItems() == 1);

    QueryNode* pExpr = m_pExprList->pPop();
    pHavingFilterOp = FilterOp::pCreate(pExpr);

    ASSERT(m_pExprList->nItems() == 0);
    delete m_pExprList;
    m_pExprList = nullptr;

    // Check that the having clause is valid.
    // The expressions in a having clause must either be the grouping column
    // in the group by list or columns that are functionally dependant on
    // the grouping column.
    if (!Expr::bContainsAnAggregate(pExpr) && !pAggregateOp) {
      // There's no aggregate op, so the having clause must contain an aggregate
      SemanticException::vThrow(EC_HAVING_EXPR_NOT_AGG_OR_IN_GROUPBY);
    }
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

    // Check if this expression is an aggregate
    if (Expr::bContainsAnAggregate(pExpr)) {
      SemanticException::vThrow(EC_AGG_EXPR_IN_WHERE_CLAUSE);
    }
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
    SyntaxException::vThrowNotImplemented(pCtx->start->getLine(),
                                          pCtx->start->getCharPositionInLine(),
                                          "No table source specified"_Stz);
  }


  // -----------------------
  // Final Plan Assembly
  // -----------------------

  QueryNode *pFirstOp = nullptr;
  QueryNode *pLastOp = nullptr;

  /// HAVING
  if (pHavingFilterOp) {
    if (!pFirstOp)  pFirstOp = pHavingFilterOp;
    if (pLastOp)    pLastOp->vAppendChild(pHavingFilterOp);

    pLastOp = pHavingFilterOp;
  }

  /// PROJECT
  if(pProjectOp) {
    if (!pFirstOp)  pFirstOp = pProjectOp;
    if (pLastOp)    pLastOp->vAppendChild(pProjectOp);

    pLastOp = pProjectOp;
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
  QueryNode* pStatement = m_pStatementStack->pTop();
  ASSERT(StatementMarker::bIsOfType(pStatement));
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

  QueryNode* pProjectedColumn = ProjectedColumn::pCreate(pExprNode, pAliasText);

  m_pSelectItems->vAppend(pProjectedColumn);

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
    NEVER_REACH
  }

  ASSERT(pAggExpr);

  // Put the operator back onto the expression stack
  m_pExprList->vPush(pAggExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Order by list                                                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitOrderByList(sqlParser::OrderByListContext *pCtx)
{
  ASSERT(!m_pExprList);

  m_pExprList = new QueryNodeList();

  visitChildren(pCtx);

  ASSERT(m_pExprList->nItems() == 0);
  delete m_pExprList;
  m_pExprList = nullptr;

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Order by expression items                                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitOrderByListItem(sqlParser::OrderByListItemContext *pCtx)
{
  ASSERT(m_pOrderExprs);
  ASSERT(m_pExprList);

  visitChildren(pCtx);

  ASSERT(m_pExprList->nItems() > 0);
  QueryNode* pExpr = m_pExprList->pPop();

  b1 bIsAscending = pCtx->DESC() == nullptr;

  QueryNode* pOrderExpr = OrderExpr::pCreate(pExpr, bIsAscending);
  m_pOrderExprs->vAppend(pOrderExpr);

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
    SemanticException::vThrow(EC_INVALID_INTEGER_FORMAT,
                              ": \"%s\""_Stz,
                              sStr.c_str());

  } catch (std::out_of_range &e) {
    SemanticException::vThrow(EC_INVALID_INTEGER_FORMAT,
                              ": Value of out range: \"%s\""_Stz,
                              sStr.c_str());
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
    SemanticException::vThrow(EC_INVALID_FLOAT_FORMAT,
                              ": \"%s\""_Stz,
                              sStr.c_str());

  } catch (std::out_of_range &e) {
    SemanticException::vThrow(EC_INVALID_FLOAT_FORMAT,
                              ": Value of out range: \"%s\""_Stz,
                              sStr.c_str());
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

  // First check that it can fit into the 256 buffer
  if (pCtx->getText().length() > 255) {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_DATE_FORMAT,
                            ": date string is too large"_Stz);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  if (sBuf.nLength() != 10) {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_DATE_FORMAT);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[4] != '-' || pcBuf[7] != '-') {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_DATE_FORMAT);
  }

  if (!StrLib::bIsDigit(pcBuf[0]) ||
      !StrLib::bIsDigit(pcBuf[1]) ||
      !StrLib::bIsDigit(pcBuf[2]) ||
      !StrLib::bIsDigit(pcBuf[3]) ||
      !StrLib::bIsDigit(pcBuf[5]) ||
      !StrLib::bIsDigit(pcBuf[6]) ||
      !StrLib::bIsDigit(pcBuf[8]) ||
      !StrLib::bIsDigit(pcBuf[9]) ) {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_DATE_FORMAT);
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
    SemanticException::vThrow(EC_INVALID_DATE_FORMAT, ": %s"_Stz, sErrorMsg.szStz());
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

  // First check that it can fit into the 256 buffer
  if (pCtx->getText().length() > 255) {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_TIME_FORMAT,
                            ": time string is too large"_Stz);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  if (sBuf.nLength() != 8) {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_TIME_FORMAT);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[2] != ':' || pcBuf[5] != ':') {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_TIME_FORMAT);
  }

  if (!StrLib::bIsDigit(pcBuf[0]) ||
      !StrLib::bIsDigit(pcBuf[1]) ||
      !StrLib::bIsDigit(pcBuf[3]) ||
      !StrLib::bIsDigit(pcBuf[4]) ||
      !StrLib::bIsDigit(pcBuf[6]) ||
      !StrLib::bIsDigit(pcBuf[7]) ) {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_TIME_FORMAT);
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
    SemanticException::vThrow(EC_INVALID_TIME_FORMAT, ": %s"_Stz, sErrorMsg.szStz());
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

  // First check that it can fit into the 256 buffer
  if (pCtx->getText().length() > 255) {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_TIMESTAMP_FORMAT,
                            ": timestamp string is too large"_Stz);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  i8 iLen = sBuf.nLength();
  if (iLen < 19) {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_TIMESTAMP_FORMAT);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[4] != '-' || pcBuf[7] != '-' || pcBuf[10] != ' ' || pcBuf[13] != ':' || pcBuf[16] != ':') {
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_TIMESTAMP_FORMAT);
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
    SyntaxException::vThrow(pCtx->start->getLine(),
                            pCtx->start->getCharPositionInLine(),
                            EC_INVALID_TIMESTAMP_FORMAT);
  }

  // check for optional micros
  b1 bHasMicros = false;

  if (iLen > 19) {
    if (pcBuf[19] != '.') {
      SyntaxException::vThrow(pCtx->start->getLine(),
                              pCtx->start->getCharPositionInLine(),
                              EC_INVALID_TIMESTAMP_FORMAT);
    }

    for (i8 iIdx = 20; iIdx < iLen; iIdx++) {
      if (iIdx > 25) {
        // can only have 6 digits of microseconds
        SyntaxException::vThrow(pCtx->start->getLine(),
                                pCtx->start->getCharPositionInLine(),
                                EC_INVALID_TIMESTAMP_FORMAT);
      }

      if (!StrLib::bIsDigit(pcBuf[iIdx])) {
        SyntaxException::vThrow(pCtx->start->getLine(),
                                pCtx->start->getCharPositionInLine(),
                                EC_INVALID_TIMESTAMP_FORMAT);
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
    SemanticException::vThrow(EC_INVALID_TIMESTAMP_FORMAT, ": %s"_Stz, sErrorMsg.szStz());
  }

  Tsp xVal;
  xVal.vSet(iYear, iMonth, iDay, iHour, iMin, iSec, iMicro);
  return xVal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
