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
#include "ShadowRoot.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/dml/SelectMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/dml/ExplainMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/SubqueryMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/MainSubquery.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/TableSubquery.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/ExprSubquery.hpp"
#include "store/parser/shadowNodes/planObjects/FilterOp.hpp"
#include "store/parser/shadowNodes/planObjects/LimitOp.hpp"
#include "store/parser/shadowNodes/planObjects/ProjectOp.hpp"
#include "store/parser/shadowNodes/planObjects/AggregateOp.hpp"
#include "store/parser/shadowNodes/planObjects/dataSources/TableScanOp.hpp"
#include "store/parser/shadowNodes/planObjects/UnionOp.hpp"
#include "store/parser/shadowNodes/planObjects/ExplainOp.hpp"
#include "store/parser/shadowNodes/planObjects/OrderOp.hpp"
#include "store/parser/shadowNodes/planObjects/joins/AbstractJoinOp.hpp"
#include "store/parser/shadowNodes/planObjects/dataSources/SubqueryRefOp.hpp"

#include "store/parser/shadowNodes/compound/TableRef.hpp"
#include "store/parser/shadowNodes/valueObjects/StringObject.hpp"
#include "store/parser/shadowNodes/valueObjects/IntegerObject.hpp"
#include "store/parser/shadowNodes/valueObjects/BooleanObject.hpp"
#include "store/parser/shadowNodes/valueObjects/UnknownObject.hpp"
#include "store/parser/shadowNodes/valueObjects/FloatObject.hpp"
#include "store/parser/shadowNodes/valueObjects/DecimalObject.hpp"
#include "store/parser/shadowNodes/lists/StringList.hpp"
#include "store/parser/shadowNodes/lists/IdList.hpp"
#include "store/parser/shadowNodes/lists/StageRangeList.hpp"
#include "store/parser/shadowNodes/compound/StageRange.hpp"
#include "store/parser/shadowNodes/compound/OrderItem.hpp"
#include "store/parser/shadowNodes/lists/OrderItemList.hpp"
#include "store/parser/shadowNodes/compound/Hint.hpp"
#include "store/parser/shadowNodes/lists/Hints.hpp"
#include "store/parser/shadowNodes/types/extractField/ExtractField.hpp"
#include "eJT.h"

#include "Text.hpp"
#include "NodeNamer.hpp"

// Expressions
#include "store/parser/shadowNodes/expressions/ColumnRef.hpp"
#include "store/parser/shadowNodes/expressions/ColumnWildcard.hpp"
#include "store/parser/shadowNodes/expressions/SubqueryRefExpr.hpp"
#include "store/parser/shadowNodes/expressions/literals/NullLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/StringLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/IntegerLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/BooleanLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/FloatLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/DecimalLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/DateLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/TimeLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/TimestampLiteral.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/AdditionExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/SubtractionExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/MultiplicationExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/DivisionExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/ModulusExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/NegationExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/mathematical/ExtractExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/EqExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/NeExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/GtExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/GeExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/LtExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/LeExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/InExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/ExistsExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/LikeExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/comparison/BetweenExpr.hpp"
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
#include "store/parser/shadowNodes/expressions/operators/stringFunction/SubstringExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/conditional/ElseExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/conditional/WhenExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/conditional/CaseExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/misc/CastExpr.hpp"

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
  m_iSubqueryCounter(0),
  m_pQueryHints(nullptr),
  m_pStatementStack(new QueryNodeStack()),
  m_pTableExprStack(new QueryNodeStack()),
  m_pTableSubqueryList(nullptr),
  m_pExprSubqueryList(nullptr),
  m_pExprStack(nullptr),
  m_pSelectItems(nullptr),
  m_pOrderItems(nullptr)
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

  if (m_pQueryHints) {
    delete m_pQueryHints;
    m_pQueryHints = nullptr;
  }

  // Clean up the statement, tableref and expression stacks
  if (m_pStatementStack) {
    while (m_pStatementStack->nItems() > 0) {
      QueryNode* pItem = m_pStatementStack->pPop();
      delete pItem;
    }
    delete m_pStatementStack;
    m_pStatementStack = nullptr;
  }

  if (m_pTableExprStack) {
    while (m_pTableExprStack->nItems() > 0) {
      QueryNode* pItem = m_pTableExprStack->pPop();
      delete pItem;
    }
    delete m_pTableExprStack;
    m_pTableExprStack = nullptr;
  }

  if (m_pTableSubqueryList) {
    while (m_pTableSubqueryList->nItems() > 0) {
      QueryNode* pItem = m_pTableSubqueryList->pPop();
      delete pItem;
    }
    delete m_pTableSubqueryList;
    m_pTableSubqueryList = nullptr;
  }

  if (m_pExprSubqueryList) {
    while (m_pExprSubqueryList->nItems() > 0) {
      QueryNode* pItem = m_pExprSubqueryList->pPop();
      delete pItem;
    }
    delete m_pExprSubqueryList;
    m_pExprSubqueryList = nullptr;
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

  if (m_pOrderItems) {
    while (m_pOrderItems->nItems() > 0) {
      QueryNode* pItem = m_pOrderItems->pPop();
      delete pItem;
    }
    delete m_pOrderItems;
    m_pOrderItems = nullptr;
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
  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRoot);

  // Attach the statement plan we created
  ASSERT(m_pStatementStack->nItems() >= 1);

  b1 bGotMainSubquery = false;
  QueryNode* pChildNode;
  while ((pChildNode = m_pStatementStack->pPop()) != nullptr) {
    ASSERT(SubqueryMarker::bIsOfType(pChildNode));

    // There can only be one MainSubquery
    if (MainSubquery::bIsOfType(pChildNode)) {
      RASSERT(!bGotMainSubquery);
      bGotMainSubquery = true;
    }

    pPlanRoot->vPushChild(pChildNode);
  }

  // There must be a MainSubquery
  RASSERT(bGotMainSubquery);

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
/* Statement choice node                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitStatementChoice(sqlParser::StatementChoiceContext *pCtx)
{
  visitChildren(pCtx);

  return nullptr;
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

  // Start with a MainSubquery and add it to the statement stack
  // Make up a unique table alias
  s256 sName;
  vGenerateSubqueryName(sName);
  QueryNode* pMainSubquery = MainSubquery::pCreate((Stz*)(queryStlStr.c_str()), sName.szStz());
  m_pStatementStack->vPush(pMainSubquery);

  // Create an explainMarker as a child of the MainSubquery
  QueryNode* pExplainMarker = ExplainMarker::pCreate((Stz*)(queryStlStr.c_str()));
  pMainSubquery->vAppendChild(pExplainMarker);

  // Add any hints
  for (auto pHintCtx : pCtx->queryHint()) {
    pHintCtx->accept(this);
  }

  if (m_pQueryHints) {
    PlanObject::vAddHints(pExplainMarker, m_pQueryHints);
    m_pQueryHints = nullptr;
  }

  // Extract the fields for the ExplainOp we need to create
  Text* pFormat = nullptr;
  QueryNode* pStageList = nullptr;
  b1 bWithNames = false;
  b1 bWithTypes = false;

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

      pStageList = pOptionsCtx->explainStageList()->accept(this).as<QueryNode *>();
      ASSERT(StageRangeList::bIsOfType(pStageList));

    } else if (pOptionsCtx->WITH_NAMES()) {
      bWithNames = true;

    } else if (pOptionsCtx->WITH_TYPES()) {
      bWithTypes = true;

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
  QueryNode* pExplainOp = ExplainOp::pCreate(pStageList, pFormat == nullptr ? nullptr : pFormat->szGet(), bWithNames, bWithTypes);
  pExplainMarker->vAppendChild(pExplainOp);
  delete pFormat;

  QueryNodeStack* pOrigStatementStack = m_pStatementStack;
  m_pStatementStack = new QueryNodeStack();

  // Visit the statement choice node. This will add a new SubqueryMarker to the statement list.
  // We can then pop it off and add it as a child of the ExplainOp.
  try {
    pCtx->statementChoice()->accept(this);

  } catch (SqlException& e) {
    // clean up the temporary m_pStatementStack
    while (m_pStatementStack->nItems() > 0) {
      QueryNode* pNode = m_pStatementStack->pPop();
      delete pNode;
    }

    delete m_pStatementStack;
    m_pStatementStack = pOrigStatementStack;

    throw;
  }

  // Any nodes that have been added to the statement stack should now be made children
  // of the ExplainOp.
  QueryNode* pChild;
  while ((pChild = m_pStatementStack->pPop()) != nullptr) {
    pExplainOp->vPushChild(pChild);
  }

  // cleanup and restore the original statement stack
  ASSERT(m_pStatementStack->nItems() == 0);
  delete m_pStatementStack;
  m_pStatementStack = pOrigStatementStack;
  pOrigStatementStack = nullptr;

  ASSERT(m_pStatementStack->pTop() == pMainSubquery);

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
  ASSERT(!m_pQueryHints);

  visitChildren(pCtx);

  ASSERT(!m_pQueryHints);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Query Hint node                                                            */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitQueryHint(sqlParser::QueryHintContext *pCtx)
{
  if(!m_pQueryHints) {
    m_pQueryHints = Hints::pCreate();
  }

  visitChildren(pCtx);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Hint node                                                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitHint(sqlParser::HintContext *pCtx)
{
  ASSERT(m_pQueryHints);
  ASSERT(Hints::bIsOfType(m_pQueryHints));

  Text* pHintKey;

  if (pCtx->name) {
    pHintKey = pCleanId(pCtx->name->identifier());

  } else if (pCtx->exprName) {
    pHintKey = pCleanString(pCtx->exprName->start);

  } else {
    NEVER_REACH
  }

  QueryNode* pHint = Hint::pCreate(pHintKey);

  // Extract out any values. A hint value could be a simple value like
  // a string or a literal, or it could be an expression.
  QueryNodeList* pOldExprStack = m_pExprStack;
  m_pExprStack = new QueryNodeList;
  ASSERT(!pOldExprStack || pOldExprStack->nItems() == 0);

  std::vector<sqlParser::ExprContext*> exprValues = pCtx->expr();
  std::vector<sqlParser::HintValueContext*> values = pCtx->hintValue();

  try {
    for (sqlParser::ExprContext* pExprCtx: exprValues) {
      pExprCtx->accept(this);

      ASSERT(m_pExprStack->nItems() == 1);
      ASSERT(Expr::bIsOfType(m_pExprStack->pHead()));

      Hint::vAddHintValue(pHint, m_pExprStack->pPop());
      ASSERT(m_pExprStack->nItems() == 0);
    }

    for (sqlParser::HintValueContext* pValueCtx: values) {

      QueryNode* pHintVal;

      if (pValueCtx->identifier()) {
        pHintVal = StringObject::pCreate(pCleanId(pValueCtx->identifier()));

      } else if (pValueCtx->expr()) {
        ASSERT(m_pExprStack->nItems() == 0);
        pValueCtx->expr()->accept(this);
        ASSERT(m_pExprStack->nItems() == 1);
        ASSERT(Expr::bIsOfType(m_pExprStack->pHead()));

        pHintVal = m_pExprStack->pPop();
        ASSERT(m_pExprStack->nItems() == 0);

      } else if (pValueCtx->simpleLiteral()->SQL_NULL()) {
        pHintVal = UnknownObject::pCreate();

      } else if (pValueCtx->simpleLiteral()->booleanLiteral()) {
        pHintVal = BooleanObject::pCreate(pValueCtx->simpleLiteral()->booleanLiteral()->SQL_TRUE() != nullptr);

      } else if (pValueCtx->simpleLiteral()->signedIntegerLiteral()) {
        pHintVal = IntegerObject::pCreate(iCleanInt(pValueCtx->simpleLiteral()->signedIntegerLiteral()));

      } else if (pValueCtx->simpleLiteral()->signedFloatLiteral()) {
        pHintVal = FloatObject::pCreate(dCleanFloat(pValueCtx->simpleLiteral()->signedFloatLiteral()));

      } else if (pValueCtx->simpleLiteral()->signedDecimalLiteral()) {
        pHintVal = DecimalObject::pCreate(xCleanDecimal(pValueCtx->simpleLiteral()->signedDecimalLiteral()));

      } else if (pValueCtx->simpleLiteral()->stringLiteral()) {
        pHintVal = StringObject::pCreate(pCleanString(pValueCtx->simpleLiteral()->stringLiteral()));

      } else {
        NEVER_REACH
      }

      Hint::vAddHintValue(pHint, pHintVal);
    }

  } catch (Exception &e) {
    pOldExprStack->vDeleteAll();
    delete pOldExprStack;
    throw;
  }

  ASSERT(m_pExprStack->nItems() == 0);
  delete m_pExprStack;
  m_pExprStack = pOldExprStack;
  ASSERT(!pOldExprStack || pOldExprStack->nItems() == 0);


  Hints::vAddHint(m_pQueryHints, pHint);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Select node (for each select of each query/subquery)                       */
/* Contains WITH / ORDER BY / LIMIT                                           */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSelectQuery(sqlParser::SelectQueryContext *pCtx)
{
  // We don't call visitChildren() in this function because we need to control
  // the order of execution. Specifically, we need to place a SelectMarker on
  // the m_pStatementStack before we descend into the selectExpr rule (we cannot
  // do that inside the selectExpr rule because it is reused in multiple places),
  // but before we can do that we need to visit any WITH queries first.
  // Calling visitChildren() doesn't allow us to do this.

  // Call each WITH subquery first. The main subquery should be last.
  for (sqlParser::WithQueryContext* pWithCtx : pCtx->withQuery()) {
    pWithCtx->accept(this);
  }

  // Get the statement string for this part of the query. Ignore any WITH
  // clauses at the start though.
  antlr4::misc::Interval interval(pCtx->selectExpr()->start->getStartIndex(), pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  // Create the select marker, but don't put it on the statement stack just yet.
  QueryNode* pStmt = SelectMarker::pCreate((Stz*)(queryStlStr.c_str()));

  // Add any hints
  if (m_pQueryHints) {
    PlanObject::vAddHints(pStmt, m_pQueryHints);
    m_pQueryHints = nullptr;
  }

  // Save the current table and expr subquery list (if any) and create a temporary new one
  PlannerContext xContext = xSaveMemberContext();
  ASSERT(m_pTableSubqueryList);
  ASSERT(m_pExprSubqueryList);

  // Now put the select marker on the new stack
  m_pStatementStack->vPush(pStmt);


  try {
    pCtx->selectExpr()->accept(this);

  } catch (SqlException& e) {

    // Cleanup temporary variables.
    vRestoreMemberContext(&xContext);
    throw;
  }

  // Remove the statementMarker. We still have it in pStmt above.
  ASSERT(m_pStatementStack->pTop() == pStmt);
  m_pStatementStack->pPop();

  // Move any table subqueries encountered to the statement stack,
  // before the select marker that is currently on top of this stack.
  if (m_pTableSubqueryList->nItems() > 0) {

    while (m_pTableSubqueryList->nItems() > 0) {
      QueryNode *pTableSubquery = m_pTableSubqueryList->pPop();
      ASSERT(TableSubquery::bIsOfType(pTableSubquery));
      xContext.pOldStatementStack->vPush(pTableSubquery);
    }
  }


  // Move any expr subqueries encountered to the statement stack,
  // before the main subquery marker that is currently on top of this stack.
  // This is because an exprSubquery that appears in a SELECT list needs
  // to be validated (and have a CteDef) before the MainSubquery is validated
  // as the MainSubquery will look for this CteDef of the exprSubquery
  if (m_pExprSubqueryList->nItems() > 0) {

    while (m_pExprSubqueryList->nItems() > 0) {
      QueryNode *pExprSubquery = m_pExprSubqueryList->pPop();
      ASSERT(ExprSubquery::bIsOfType(pExprSubquery));
      xContext.pOldStatementStack->vPush(pExprSubquery);
    }
  }

  // Move anything else on the statement stack
  if (m_pStatementStack->nItems() > 0) {
    while (m_pStatementStack->nItems() > 0) {
      QueryNode *pPlanObject = m_pStatementStack->pPop();
      ASSERT(PlanObject::bIsOfType(pPlanObject));
      xContext.pOldStatementStack->vPush(pPlanObject);
    }
  }

  // Restore the original member variables
  ASSERT(m_pStatementStack->nItems() == 0);
  ASSERT(m_pTableSubqueryList->nItems() == 0);
  ASSERT(m_pExprSubqueryList->nItems() == 0);
  vRestoreMemberContext(&xContext);

  // Add a MainSubquery to the statement stack
  // Make up a unique table alias
  s256 sName;
  vGenerateSubqueryName(sName);

  QueryNode* pMainSubquery = MainSubquery::pCreate((Stz*)(queryStlStr.c_str()), sName.szStz());
  pMainSubquery->vAppendChild(pStmt);
  m_pStatementStack->vPush(pMainSubquery);

  if (pCtx->ORDER()) {
    ASSERT(!m_pOrderItems);
    m_pOrderItems = new QueryNodeList();

    // Call the ORDER BY children
    pCtx->orderBy->accept(this);

    ASSERT(m_pOrderItems);
    ASSERT(m_pOrderItems->nItems() > 0);

    // Convert the QueryNodeList to a OrderItemList
    QueryNode* pOrderItemList = OrderItemList::pCreate();
    QueryNode* pOrderItem;
    while ((pOrderItem = m_pOrderItems->pPop()) != nullptr) {
      pOrderItemList->vAppendChild(pOrderItem);
    }

    // Create the OrderOp
    QueryNode* pOrderOp = OrderOp::pCreate(pOrderItemList);

    // Insert the orderOp as the top-most child underneath the statement.
    // Need to make any current children of statement now children of orderOp.
    pStmt->vInsertAsParentOfChildren(pOrderOp);

    ASSERT(m_pOrderItems->nItems() == 0);
    delete m_pOrderItems;
    m_pOrderItems = nullptr;
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
    pStmt->vInsertAsParentOfChildren(pLimitOp);
  }

  // The last item on the stack is the main subquery
  ASSERT(m_pStatementStack->nItems() > 0);
  ASSERT(SubqueryMarker::bIsOfType(m_pStatementStack->pTop()));

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* WITH query item (for each table subquery)                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitWithQuery(sqlParser::WithQueryContext *pCtx)
{
  // Get the statement string for this part of the query
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(),
                                  pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  QueryNodeStack* pOrigStatementStack = m_pStatementStack;
  m_pStatementStack = new QueryNodeStack();

  try {
    visitChildren(pCtx);

  } catch (SqlException& e) {
    // clean up the temporary m_pStatementStack
    while (m_pStatementStack->nItems() > 0) {
      QueryNode* pNode = m_pStatementStack->pPop();
      delete pNode;
    }

    delete m_pStatementStack;
    m_pStatementStack = pOrigStatementStack;

    throw;
  }

  if (pCtx->RECURSIVE()) {
    __NOT_IMPL("RECURSIVE common table expression")
  }

  // Extract the mandatory table alias name
  Text* pTblAlias = pCleanId(pCtx->name->id);
  ASSERT(pTblAlias);

  // Extract the optional list of column aliases
  QueryNode* pIdList = IdList::pCreate();
  if (pCtx->colAliases()) {
    for (sqlParser::ColAliasListItemContext* pColAliasCtx: pCtx->colAliases()->colAliasList()->colAliasListItem()) {
      Text* pColName = pCleanId(pColAliasCtx->id);
      QueryNode* pColNameNode = StringObject::pCreate(pColName);
      pIdList->vAppendChild(pColNameNode);
    }
  }

  // The last item on the stack is the table subquery. We need to add
  // a TableSubquery marker to the top of it and then put it back
  // onto the statement stack.
  ASSERT(m_pStatementStack->nItems() > 0);

  QueryNode* pTableSubquery = TableSubquery::pCreate(pTblAlias, (Stz*)(queryStlStr.c_str()), pIdList, pCtx->RECURSIVE() != nullptr);

  QueryNode* pNode;
  while ((pNode = m_pStatementStack->pPop()) != nullptr) {
    ASSERT(SubqueryMarker::bIsOfType(pNode));

    pTableSubquery->vPushChild(pNode);
  }

  // Restore the original statement stack
  ASSERT(m_pStatementStack->nItems() == 0);
  delete m_pStatementStack;
  m_pStatementStack = pOrigStatementStack;

  // And put this table subquery on the original statement stack
  m_pStatementStack->vPush(pTableSubquery);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Set operations (union, intersect, except) over select queries              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSetQuery(sqlParser::SetQueryContext *pCtx)
{

  // We don't call visitChildren() in this function because we need to control
  // the order of execution.

  // Call each WITH subquery first. The main subquery should be last.
  for (sqlParser::WithQueryContext* pWithCtx : pCtx->withQuery()) {
    pWithCtx->accept(this);
  }

  // Get the statement string for this part of the query, ignore WITH clauses
  antlr4::misc::Interval interval(pCtx->setExpr()->start->getStartIndex(),
                                  pCtx->stop->getStopIndex());
  std::string mainQueryStlStr = pCtx->start->getInputStream()->getText(interval);


  // Create a new statement stack before we descend into setExpr
  PlannerContext xContext = xSaveMemberContext();
  ASSERT(m_pTableSubqueryList);
  ASSERT(m_pExprSubqueryList);


  // Visit the setExpr node. This will add a single SelectMarker to the statement list.
  // We can then pop it off and add it as a child of the MainSubquery.
  try {
    pCtx->setExpr()->accept(this);

  } catch (SqlException& e) {
    // clean up the temporary m_pStatementStack
    vRestoreMemberContext(&xContext);

    throw;
  }

  ASSERT(m_pStatementStack->nItems() == 1);
  QueryNode* pSetExprSelectMarker = m_pStatementStack->pPop();
  ASSERT(SelectMarker::bIsOfType(pSetExprSelectMarker));

  // Before we put the statement marker on the original statement stack,
  // let's see if there are any table subqueries from m_pTableExprStack
  // that need to be added first.
  while (m_pTableSubqueryList->nItems() > 0) {
    QueryNode* pTableSubquery = m_pTableSubqueryList->pPop();
    ASSERT(TableSubquery::bIsOfType(pTableSubquery));
    xContext.pOldStatementStack->vPush(pTableSubquery);
  }

  // Add any expr subqueries onto the statement stack. These need to go
  // on before the MainSubquery because in the case of an ExprSubquery
  // in a SELECT list we need to validate the subquery and create its
  // CteDef before the MainSubquery is processed.
  while (m_pExprSubqueryList->nItems() > 0) {
    QueryNode* pExprSubquery = m_pExprSubqueryList->pPop();
    ASSERT(ExprSubquery::bIsOfType(pExprSubquery));
    xContext.pOldStatementStack->vPush(pExprSubquery);
  }

  // cleanup and restore the original member variables
  ASSERT(m_pStatementStack->nItems() == 0);
  ASSERT(m_pTableSubqueryList->nItems() == 0);
  ASSERT(m_pExprSubqueryList->nItems() == 0);
  vRestoreMemberContext(&xContext);

  // Now put the statement marker on the original statement stack
  m_pStatementStack->vPush(pSetExprSelectMarker);


  if (pCtx->ORDER() || pCtx->LIMIT()) {
    // We need to stick another statementOp on top of the one from setExpr
    QueryNode* pSubStatement = m_pStatementStack->pPop();
    ASSERT(pSubStatement);

    // Get the statement string for this part of the query
    antlr4::misc::Interval interval(pCtx->start->getStartIndex(), pCtx->stop->getStopIndex());
    std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

    QueryNode *pOuterSelectStatement = SelectMarker::pCreate((Stz *) (queryStlStr.c_str()));
    pOuterSelectStatement->vAppendChild(pSubStatement);
    m_pStatementStack->vPush(pOuterSelectStatement);


    if (pCtx->ORDER()) {
      ASSERT(!m_pOrderItems);
      m_pOrderItems = new QueryNodeList();

      // Descend down into the order by children
      pCtx->orderBy->accept(this);

      ASSERT(m_pOrderItems);
      ASSERT(m_pOrderItems->nItems() > 0);

      // Convert the QueryNodeList to a OrderItemList
      QueryNode* pOrderItemList = OrderItemList::pCreate();
      QueryNode* pOrderItem;
      while ((pOrderItem = m_pOrderItems->pPop()) != nullptr) {
        pOrderItemList->vAppendChild(pOrderItem);
      }

      // Create the OrderOp
      QueryNode* pOrderOp = OrderOp::pCreate(pOrderItemList);

      // Insert the orderOp as the top-most child underneath the statement.
      // Need to make any current children of statement now children of orderOp.
      pOuterSelectStatement->vInsertAsParentOfChildren(pOrderOp);

      ASSERT(m_pOrderItems->nItems() == 0);
      delete m_pOrderItems;
      m_pOrderItems = nullptr;
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
      pOuterSelectStatement->vInsertAsParentOfChildren(pLimitOp);
    }
  }


  // Finally, add a MainSubquery on top of the last thing on the statement stack
  ASSERT(m_pStatementStack->nItems() >= 1);
  ASSERT(SelectMarker::bIsOfType(m_pStatementStack->pTop()));

  // Make up a unique table alias
  s256 sName;
  vGenerateSubqueryName(sName);

  QueryNode* pMainSubquery = MainSubquery::pCreate((Stz*)(mainQueryStlStr.c_str()), sName.szStz());
  pMainSubquery->vAppendChild(m_pStatementStack->pPop());
  m_pStatementStack->vPush(pMainSubquery);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* (Table) Fetch node                                                         */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitFetch(sqlParser::FetchContext *pCtx)
{
  visitChildren(pCtx);

  if (pCtx->tableAlias()) {
    Text* pAlias = pCleanId(pCtx->tableAlias()->identifier());

    QueryNode* pTableScanOp = m_pTableExprStack->pTop();
    ASSERT(TableScanOp::bIsOfType(pTableScanOp));
    ASSERT(!TableScanOp::bHasAlias(pTableScanOp));

    TableScanOp::vSetAlias(pTableScanOp, pAlias);
  }
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* (Table) Join On node                                                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitJoinOn(sqlParser::JoinOnContext *pCtx)
{
  // Setup for join predicate
  b1 bFreeExprStack = false;
  if (!m_pExprStack) {
    m_pExprStack = new QueryNodeList;
    bFreeExprStack = true;
  }

  eJT eJoinType;
  QueryNode* pOldQueryHints = m_pQueryHints;
  m_pQueryHints = nullptr;

  try {
    visitChildren(pCtx);

    // Extract join type. It can also throw an exception
    eJoinType = eExtractJoinType(pCtx->extendedJoinType());

  } catch (SqlException& e) {

    if (bFreeExprStack) {
      m_pExprStack->vDeleteAll();
      delete m_pExprStack;
      m_pExprStack = nullptr;
    }

    delete pOldQueryHints;
    throw;
  }

  // Restore the query hints
  QueryNode* pJoinHints = m_pQueryHints;
  m_pQueryHints = pOldQueryHints;

  // Extract the join table expressions
  ASSERT(m_pTableExprStack->nItems() >=2);
  QueryNode* pTableExpr2 = m_pTableExprStack->pPop();
  QueryNode* pTableExpr1 = m_pTableExprStack->pPop();

  // Extract join predicate
  ASSERT(m_pExprStack->nItems() >= 1);
  QueryNode* pJoinPredicate = m_pExprStack->pPop();

  if (bFreeExprStack) {
    ASSERT(m_pExprStack->nItems() == 0);
    delete m_pExprStack;
    m_pExprStack = nullptr;
  }

  QueryNode* pJoinOp = AbstractJoinOp::pCreate(eJoinType, pJoinPredicate, nullptr);
  pJoinOp->vAppendChild(pTableExpr1);
  pJoinOp->vAppendChild(pTableExpr2);

  if (pJoinHints) {
    PlanObject::vAddHints(pJoinOp, pJoinHints);
  }

  // Put it back on the table expr stack so that visitSelectExpr() can pop it off
  m_pTableExprStack->vPush(pJoinOp);
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* (Table) Join Using node                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitJoinUsing(sqlParser::JoinUsingContext *pCtx)
{
  QueryNode* pOldQueryHints = m_pQueryHints;
  m_pQueryHints = nullptr;

  try {
    visitChildren(pCtx);

  } catch (SqlException& e) {
    delete pOldQueryHints;
    throw;
  }

  // Restore the query hints
  QueryNode* pJoinHints = m_pQueryHints;
  m_pQueryHints = pOldQueryHints;

  // Extract join type. It can also throw an exception
  eJT eJoinType = eExtractJoinType(pCtx->extendedJoinType());


  // Extract the join table expressions
  ASSERT(m_pTableExprStack->nItems() >=2);
  QueryNode* pTableExpr2 = m_pTableExprStack->pPop();
  QueryNode* pTableExpr1 = m_pTableExprStack->pPop();

  // Extract the USING column identifiers into a StringList node
  QueryNode* pUsingList = StringList::pCreate();

  for (sqlParser::JoinUsingListItemContext* pItemCtx : pCtx->joinUsingList()->joinUsingListItem()) {
    Text* pColumnName = pCleanId(pItemCtx->identifier());

    QueryNode* pColumnNode = StringObject::pCreate(pColumnName);
    pUsingList->vAppendChild(pColumnNode);
  }

  ASSERT(pUsingList->nChildren() > 0);

  QueryNode* pJoinOp = AbstractJoinOp::pCreate(eJoinType, nullptr, pUsingList);
  pJoinOp->vAppendChild(pTableExpr1);
  pJoinOp->vAppendChild(pTableExpr2);

  if (pJoinHints) {
    PlanObject::vAddHints(pJoinOp, pJoinHints);
  }

  // Put it back on the table expr stack so that visitSelectExpr() can pop it off
  m_pTableExprStack->vPush(pJoinOp);
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* (Table) Cross Join node                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitCrossJoin(sqlParser::CrossJoinContext *pCtx)
{
  visitChildren(pCtx);

  // Extract the join table expressions
  ASSERT(m_pTableExprStack->nItems() >=2);
  QueryNode* pTableExpr2 = m_pTableExprStack->pPop();
  QueryNode* pTableExpr1 = m_pTableExprStack->pPop();

  // Join type
  eJT eJoinType = JT_CROSS;

  // A cross join is the same as an inner join with join predicate of TRUE.
  QueryNode* pJoinPredicate = BooleanLiteral::pCreate(true);

  QueryNode* pJoinOp = AbstractJoinOp::pCreate(eJoinType, pJoinPredicate, nullptr);
  pJoinOp->vAppendChild(pTableExpr1);
  pJoinOp->vAppendChild(pTableExpr2);

  // Put it back on the table expr stack so that visitSelectExpr() can pop it off
  m_pTableExprStack->vPush(pJoinOp);
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* (Table) Natural join node                                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitNaturalJoin(sqlParser::NaturalJoinContext *pCtx)
{
  __NOT_IMPL("NATURAL JOIN")
  visitChildren(pCtx);

  // Extract join type. It can also throw an exception
  eJT eJoinType = eExtractJoinType(pCtx->joinType());


  // Extract the join table expressions
  ASSERT(m_pTableExprStack->nItems() >=2);
  QueryNode* pTableExpr2 = m_pTableExprStack->pPop();
  QueryNode* pTableExpr1 = m_pTableExprStack->pPop();

  UNUSED(eJoinType);   // for now...
  UNUSED(pTableExpr1); // for now...
  UNUSED(pTableExpr2); // for now...

  // TODO: Extract join hint

  // TODO: create a join predicate
//  QueryNode* pJoinPredicate = ????
//
//
//  QueryNode* pJoinOp = NestedLoopJoinOp::pCreate(eJoinType, pJoinPredicate);
//  pJoinOp->vAppendChild(pTableExpr1);
//  pJoinOp->vAppendChild(pTableExpr2);
//
//  // Put it back on the table expr stack so that visitSelectExpr() can pop it off
//  m_pTableExprStack->vPush(pJoinOp);
//  return nullptr;
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
  QueryNode* pTableScanOp = TableScanOp::pCreate(pTableRef);
  m_pTableExprStack->vPush(pTableScanOp);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Table Subquery node                                                        */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSubquery(sqlParser::SubqueryContext *pCtx)
{
  ASSERT(m_pTableExprStack);

  // Get the statement string for this part of the query
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(),
                                  pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  // Save the context of the member variables
  // Visit the subquery. First we need to save and clear the member variables
  ASSERT(!m_pSelectItems);
  ASSERT(!m_pOrderItems);
  PlannerContext xContext = xSaveMemberContext();

  visitChildren(pCtx);

  // Any subordinate table and expression subqueries found here
  // need to be added as children of pTableSubquery below. So save
  // the lists so we can restore the context and still deal with
  // these lists later on.
  QueryNodeList* pSubordinateTableSubqueries = m_pTableSubqueryList;
  QueryNodeList* pSubordinateExprSubqueries  = m_pExprSubqueryList;
  m_pTableSubqueryList = nullptr;
  m_pExprSubqueryList = nullptr;


  // We should have precisely one main subquery on the statement stack
  // and possibly some table and expr subqueries too.
  ASSERT(m_pStatementStack->nItems() >= 1);

  QueryNode* pMainSubqueryNode = nullptr;

  while (m_pStatementStack->nItems() > 0) {
    QueryNode* pItem = m_pStatementStack->pPop();

    if (TableSubquery::bIsOfType(pItem)) {
      pSubordinateTableSubqueries->vAppend(pItem);

    } else if (MainSubquery::bIsOfType(pItem)) {
      pMainSubqueryNode = pItem;

    } else if (ExprSubquery::bIsOfType(pItem)) {
      pSubordinateExprSubqueries->vAppend(pItem);

    } else {
      NEVER_REACH
    }
  }


  // Restore the context of the member variables
  ASSERT(m_pStatementStack->nItems() == 0);
  ASSERT(m_pTableExprStack->nItems() == 0);
  ASSERT(!m_pTableSubqueryList || m_pTableSubqueryList->nItems() == 0);
  ASSERT(!m_pExprSubqueryList || m_pExprSubqueryList->nItems() == 0);
  ASSERT(!m_pExprStack);
  ASSERT(!m_pSelectItems);
  ASSERT(!m_pOrderItems);
  vRestoreMemberContext(&xContext);


  // Extract the table alias, if any
  Text* pTableAlias;
  QueryNode* pColumnNames = IdList::pCreate();


  if (pCtx->tableAlias()) {
    pTableAlias = pCleanId(pCtx->tableAlias()->identifier());

    // Extract the optional list of column aliases
    if (pCtx->colAliases()) {
      for (sqlParser::ColAliasListItemContext* pColAliasCtx: pCtx->colAliases()->colAliasList()->colAliasListItem()) {
        Text* pColName = pCleanId(pColAliasCtx->id);
        QueryNode* pColNameNode = StringObject::pCreate(pColName);
        pColumnNames->vAppendChild(pColNameNode);
      }
    }

  } else {
    // Make up a unique table alias
    s256 sName;
    vGenerateSubqueryName(sName);
    pTableAlias = new Text(sName);
  }

  // Create a new TableSubquery, add pMainSubquery to it as a child and add this
  // table subquery to the m_pTableSubqueryList so it can be handled upstream.
  QueryNode* pTableSubquery = TableSubquery::pCreate(new Text(pTableAlias->szGet()), (Stz*)(queryStlStr.c_str()), pColumnNames, false);

  while (pSubordinateTableSubqueries && pSubordinateTableSubqueries->nItems() > 0) {
    QueryNode* pTableSubqueryNode = pSubordinateTableSubqueries->pPop();
    ASSERT(TableSubquery::bIsOfType(pTableSubqueryNode));

    pTableSubquery->vAppendChild(pTableSubqueryNode);
  }

  while (pSubordinateExprSubqueries && pSubordinateExprSubqueries->nItems() > 0) {
    QueryNode* pExprSubqueryNode = pSubordinateExprSubqueries->pPop();
    ASSERT(ExprSubquery::bIsOfType(pExprSubqueryNode));

    pTableSubquery->vAppendChild(pExprSubqueryNode);
  }

  ASSERT(!pSubordinateTableSubqueries || pSubordinateTableSubqueries->nItems() == 0);
  ASSERT(!pSubordinateExprSubqueries  || pSubordinateExprSubqueries->nItems() == 0);
  delete pSubordinateTableSubqueries;
  delete pSubordinateExprSubqueries;

  // Now that we've added any subordinate table/expr subqueries
  // we can add the main subquery to pTableSubquery
  pTableSubquery->vAppendChild(pMainSubqueryNode);
  m_pTableSubqueryList->vAppend(pTableSubquery);

  // Add a SubqueryRefOp to m_pTableExprStack that references the table subquery we just created.
  QueryNode* pSubqueryRefOp = SubqueryRefOp::pCreate(pTableAlias);
  m_pTableExprStack->vPush(pSubqueryRefOp);

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
      ASSERT(!m_pExprStack);
      m_pExprStack = new QueryNodeList;

      // Visit the children of the GROUP BY clause
      try {
        pCtx->groupBy->accept(this);

      } catch (SqlException& e) {
        if (pProjectOp)       delete pProjectOp;
        throw;
      }

      ASSERT(m_pExprStack->nItems() > 0);

    // Create the aggregate operator
      pAggregateOp = AggregateOp::pCreate(m_pSelectItems, m_pExprStack);

    // Clean up
      ASSERT(m_pSelectItems->nItems() == 0);
      delete m_pSelectItems;
      m_pSelectItems = nullptr;

      ASSERT(m_pExprStack->nItems() == 0);
      delete m_pExprStack;
      m_pExprStack = nullptr;
  }

  // -----------------------
  // HAVING clause
  // -----------------------
  QueryNode* pHavingFilterOp = nullptr;
  if (pCtx->having) {

    ASSERT(!m_pExprStack);
    m_pExprStack = new QueryNodeList();

    // Visit the children of the HAVING clause
    try {
      pCtx->having->accept(this);

    } catch (SqlException& e) {
      if (pProjectOp)       delete pProjectOp;
      if (pAggregateOp)     delete pAggregateOp;
      throw;
    }

    ASSERT(m_pExprStack->nItems() == 1);

    QueryNode* pExpr = m_pExprStack->pPop();
    pHavingFilterOp = FilterOp::pCreate(pExpr);

    ASSERT(m_pExprStack->nItems() == 0);
    delete m_pExprStack;
    m_pExprStack = nullptr;
  }


  // -----------------------
  // WHERE clause
  // -----------------------
  QueryNode* pWhereFilterOp = nullptr;
  if (pCtx->where) {
    ASSERT(!m_pExprStack);
    m_pExprStack = new QueryNodeList;

    // Visit the children of the WHERE clause
    try {
      pCtx->where->accept(this);

    } catch (SqlException& e) {
      if (pProjectOp)       delete pProjectOp;
      if (pAggregateOp)     delete pAggregateOp;
      if (pHavingFilterOp)  delete pHavingFilterOp;
      throw;
    }

    ASSERT(m_pExprStack->nItems() == 1);

    QueryNode* pExpr = m_pExprStack->pPop();
    pWhereFilterOp = FilterOp::pCreate(pExpr);

    ASSERT(m_pExprStack->nItems() == 0);
    delete m_pExprStack;
    m_pExprStack = nullptr;

    // Check if this expression is an aggregate
    if (Expr::bContainsAnAggregate(pExpr)) {

      if (pProjectOp)       delete pProjectOp;
      if (pAggregateOp)     delete pAggregateOp;
      if (pHavingFilterOp)  delete pHavingFilterOp;
      if (pWhereFilterOp)   delete pWhereFilterOp;

      SemanticException::vThrow(EC_AGG_EXPR_IN_WHERE_CLAUSE);
    }
  }


  // -----------------------
  // Table Expressions
  // (These include TableScanOps, JoinOps, table subqueries, etc.
  // -----------------------
  // Check for a table expression. If one exists, attach it as a child of the ProjectOp
  QueryNode* pTableExprNode = nullptr;
  if (pCtx->tableExpr()) {
    // Visit the grammar rule for table expressions
    try {
      pCtx->tableExpr()->accept(this);

    } catch (SqlException& e) {
      if (pProjectOp)       delete pProjectOp;
      if (pAggregateOp)     delete pAggregateOp;
      if (pHavingFilterOp)  delete pHavingFilterOp;
      if (pWhereFilterOp)   delete pWhereFilterOp;
      throw;
    }

    ASSERT(m_pTableExprStack->nItems() == 1);
    pTableExprNode = m_pTableExprStack->pPop();
    ASSERT(DataSource::bIsOfType(pTableExprNode) ||
                    JoinOp::bIsOfType(pTableExprNode) ||
                    SubqueryRefOp::bIsOfType(pTableExprNode) );


  } else {

    if (pProjectOp)       delete pProjectOp;
    if (pAggregateOp)     delete pAggregateOp;
    if (pHavingFilterOp)  delete pHavingFilterOp;
    if (pWhereFilterOp)   delete pWhereFilterOp;

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
  if (pTableExprNode) {
    if (!pFirstOp)  pFirstOp = pTableExprNode;
    if (pLastOp)    pLastOp->vAppendChild(pTableExprNode);

    pLastOp = pTableExprNode;
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
  
  ASSERT(!m_pExprStack);
  m_pExprStack = new QueryNodeList;
  
  visitChildren(pCtx);
  
  ASSERT(m_pExprStack->nItems() == 0);
  delete m_pExprStack;
  m_pExprStack = nullptr;
  
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
  ASSERT(m_pExprStack->nItems() == 1);
  
  QueryNode* pExprNode = m_pExprStack->pPop();

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
  ASSERT(m_pExprStack);

  visitChildren(pCtx);
  
  Text* pCatalogName = pCtx->catalogName() == nullptr ? nullptr : pCleanId(pCtx->catalogName()->id);
  Text* pSchemaName = pCtx->schemaName() == nullptr ? nullptr : pCleanId(pCtx->schemaName()->id);
  Text* pTableName = pCtx->tableName() == nullptr ? nullptr : pCleanId(pCtx->tableName()->id);
  Text* pColumnName = pCleanId(pCtx->columnName()->id);

  QueryNode* pColumnRef = ColumnRef::pCreate(nullptr,
                                             pCatalogName, pSchemaName,
                                             pTableName, pColumnName);

  m_pExprStack->vPush(pColumnRef);
  
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Column Reference node                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitColumnWildcard(sqlParser::ColumnWildcardContext* pCtx)
{
  ASSERT(m_pExprStack);

  visitChildren(pCtx);

  Text* pCatalogName = pCtx->catalogName() == nullptr ? nullptr : pCleanId(pCtx->catalogName()->id);
  Text* pSchemaName = pCtx->schemaName() == nullptr ? nullptr : pCleanId(pCtx->schemaName()->id);
  Text* pTableName = pCtx->tableName() == nullptr ? nullptr : pCleanId(pCtx->tableName()->id);

  QueryNode* pColumnWildcard = ColumnWildcard::pCreate(pCatalogName, pSchemaName, pTableName);

  m_pExprStack->vPush(pColumnWildcard);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Expression Literal node                                                    */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitLiteral(sqlParser::LiteralContext* pCtx)
{
  ASSERT(m_pExprStack);

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

  } else if (pCtx->signedIntegerLiteral()) {
    i8 iVal = iCleanInt(pCtx->signedIntegerLiteral());
    pLiteral = IntegerLiteral::pCreate(iVal);

  } else if (pCtx->signedDecimalLiteral()) {
    Bcd xBcd = xCleanDecimal(pCtx->signedDecimalLiteral());
    pLiteral = DecimalLiteral::pCreate(xBcd);

  } else if (pCtx->signedFloatLiteral()) {
    d8 dVal = dCleanFloat(pCtx->signedFloatLiteral());
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
    __NOT_IMPL("<interval literal>")

  } else {
    NEVER_REACH
  }

  m_pExprStack->vPush(pLiteral);
  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Unary Expression Operators                                                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitUnaryOp(sqlParser::UnaryOpContext *pCtx)
{
  visitChildren(pCtx);

  QueryNode* pOpExpr = nullptr;
  QueryNode* pExpr1 = m_pExprStack->pPop();

  // Get the string of the operator
  s256 sOp;
  vCleanString(pCtx->op, sOp);

  if (sOp.bEqual("+"_Stz)) {
    // We don't create the identity operator, just push the expression
    // back onto the stack again.
    pOpExpr = pExpr1;

  } else if (sOp.bEqual("-"_Stz)) {

    // Optimisation for negating simple numerics
    if (IntegerLiteral::bIsOfType(pExpr1) && !IntegerLiteral::bIsNull(pExpr1)) {
      i8 iVal = IntegerLiteral::iValue(pExpr1);
      IntegerLiteral::vSetValue(pExpr1, -iVal);
      pOpExpr = pExpr1;

    } else if (FloatLiteral::bIsOfType(pExpr1) && !FloatLiteral::bIsNull(pExpr1)) {
      d8 dVal = FloatLiteral::dValue(pExpr1);
      FloatLiteral::vSetValue(pExpr1, -dVal);
      pOpExpr = pExpr1;

    } else if (DecimalLiteral::bIsOfType(pExpr1) && !DecimalLiteral::bIsNull(pExpr1)) {
      Bcd xVal = DecimalLiteral::xValue(pExpr1);
      xVal.T.m_iMantissa = -xVal.T.m_iMantissa;
      DecimalLiteral::vSetValue(pExpr1, xVal);
      pOpExpr = pExpr1;

    } else {
      // Simply negate the expression on the stack
      pOpExpr = NegationExpr::pCreate(pExpr1);
    }

  } else {
    NEVER_REACH
  }

  // Put the operator back onto the expression stack
  m_pExprStack->vPush(pOpExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Binary Expression Operators                                                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitBinaryOp(sqlParser::BinaryOpContext *pCtx)
{
  visitChildren(pCtx);

  QueryNode* pOpExpr = nullptr;
  QueryNode* pExpr2 = m_pExprStack->pPop();
  QueryNode* pExpr1 = m_pExprStack->pPop();

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
  m_pExprStack->vPush(pOpExpr);

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
    pExpr2 = m_pExprStack->pPop();
    pExpr1 = m_pExprStack->pPop();
    pOpExpr = AndExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("OR"_Stz)) {
    pExpr2 = m_pExprStack->pPop();
    pExpr1 = m_pExprStack->pPop();
    pOpExpr = OrExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("XOR"_Stz)) {
    pExpr2 = m_pExprStack->pPop();
    pExpr1 = m_pExprStack->pPop();
    pOpExpr = XorExpr::pCreate(pExpr1, pExpr2);

  } else if (sOp.bEqual("NOT"_Stz)) {
    pExpr1 = m_pExprStack->pPop();
    pOpExpr = NotExpr::pCreate(pExpr1);

  } else {
    NEVER_REACH
  }

  // Put the operator back onto the expression stack
  m_pExprStack->vPush(pOpExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* IsNull Expression Operator                                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitIsNullOp(sqlParser::IsNullOpContext *pCtx)
{
  visitChildren(pCtx);

  QueryNode* pOpExpr = nullptr;
  QueryNode* pExpr1 = m_pExprStack->pPop();

  pOpExpr = IsNullExpr::pCreate(pExpr1);

  // Turn "IS NOT NULL <expr>" into NOT(<expr> IS NULL)
  if (pCtx->NOT()) {
    pOpExpr = NotExpr::pCreate(pOpExpr);
  }

  // Put the operator back onto the expression stack
  m_pExprStack->vPush(pOpExpr);

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
  m_pExprStack->vPush(pOpExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Generic Aggregate Expression Operator                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitAggFunc(sqlParser::AggFuncContext *pCtx)
{
  visitChildren(pCtx);

  b1 bIsDistinct = (pCtx->distinct != nullptr);

  ASSERT(m_pExprStack->nItems() > 0);
  QueryNode* pChildExpr = m_pExprStack->pPop();

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
  m_pExprStack->vPush(pAggExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Expression subqueries                                                      */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSubqueryExpr(sqlParser::SubqueryExprContext *pCtx)
{
  // Get the statement string for this part of the query
  antlr4::misc::Interval interval(pCtx->start->getStartIndex(),
                                  pCtx->stop->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  PlannerContext xContext = xSaveMemberContext();

  try {
    visitChildren(pCtx);

  } catch (SqlException& e) {
    // Cleanup current member variables and restore all of the saved stacks/lists.
    vRestoreMemberContext(&xContext);
    throw;
  }

  // Create a ExprSubquery node
  // First make up a unique name for its table alias
  s256 sSubqueryName;
  vGenerateSubqueryName(sSubqueryName);

  QueryNode* pExprSubquery = ExprSubquery::pCreate(sSubqueryName.szStz(), (Stz*)(queryStlStr.c_str()));

  // We expect there to be one or more nodes in m_pStatementStack,
  // corresponding to a MainSubquery and zero or more TableSubquery nodes.
  // Pop them off and add them as children of the ExprSubquery node

  ASSERT(m_pStatementStack);
  while (m_pStatementStack->nItems() > 0) {
    QueryNode* pSubqueryMarker = m_pStatementStack->pPop();
    ASSERT(SubqueryMarker::bIsOfType(pSubqueryMarker));

    pExprSubquery->vPushChild(pSubqueryMarker);
  }

  // Restore the context of the member variables
  ASSERT(m_pStatementStack->nItems() == 0);
  ASSERT(m_pTableExprStack->nItems() == 0);
  ASSERT(!m_pTableSubqueryList || m_pTableSubqueryList->nItems() == 0);
  ASSERT(!m_pExprSubqueryList || m_pExprSubqueryList->nItems() == 0);
  ASSERT(!m_pExprStack || m_pExprStack->nItems() == 0);
  ASSERT(!m_pSelectItems || m_pSelectItems->nItems() == 0);
  ASSERT(!m_pOrderItems || m_pOrderItems->nItems() == 0);
  vRestoreMemberContext(&xContext);

  // Put the expression subquery marker onto the expression subquery list
  // so that it will be picked up upstream.
  m_pExprSubqueryList->vAppend(pExprSubquery);

  // Now create a SubqueryRefExpr node and reference the ExprSubquery
  QueryNode* pSubqueryRefExpr = SubqueryRefExpr::pCreate(ExprSubquery::szTableAlias(pExprSubquery));

  // Put the SubqueryRefExpr onto the exprStack
  m_pExprStack->vPush(pSubqueryRefExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* IN Expression Operator with a subquery child                               */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSubqueryInOp(sqlParser::SubqueryInOpContext *pCtx)
{
  // Get the statement string for this part of the query
  antlr4::misc::Interval interval(pCtx->query()->getStart()->getStartIndex(),
                                  pCtx->query()->getStop()->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);

  // Visit the left expression
  pCtx->expr()->accept(this);
  ASSERT(m_pExprStack->nItems() > 0);
  QueryNode* pLeftExpr = m_pExprStack->pPop();

  b1 bInvert = (pCtx->NOT() != nullptr);

  // Visit the subquery. First we need to save and clear the member variables
  PlannerContext xContext = xSaveMemberContext();

  try {
    pCtx->query()->accept(this);

  } catch (SqlException& e) {
    // Cleanup current member variables and restore all of the saved stacks/lists.
    vRestoreMemberContext(&xContext);
    delete pLeftExpr;
    throw;
  }

  // Create a ExprSubquery node
  // First make up a unique name for its table alias
  s256 sSubqueryName;
  vGenerateSubqueryName(sSubqueryName);

  QueryNode* pExprSubquery = ExprSubquery::pCreate(sSubqueryName.szStz(), (Stz*)(queryStlStr.c_str()));

  // We expect there to be one or more nodes in m_pStatementStack,
  // corresponding to a MainSubquery and zero or more TableSubquery nodes.
  // Pop them off and add them as children of the ExprSubquery node

  ASSERT(m_pStatementStack);
  while (m_pStatementStack->nItems() > 0) {
    QueryNode* pSubqueryMarker = m_pStatementStack->pPop();
    ASSERT(SubqueryMarker::bIsOfType(pSubqueryMarker));

    pExprSubquery->vPushChild(pSubqueryMarker);
  }

  // Restore the member variables
  ASSERT(m_pStatementStack->nItems() == 0);
  ASSERT(m_pTableExprStack->nItems() == 0);
  ASSERT(!m_pTableSubqueryList || m_pTableSubqueryList->nItems() == 0);
  ASSERT(!m_pExprSubqueryList || m_pExprSubqueryList->nItems() == 0);
  ASSERT(!m_pExprStack || m_pExprStack->nItems() == 0);
  ASSERT(!m_pSelectItems || m_pSelectItems->nItems() == 0);
  ASSERT(!m_pOrderItems || m_pOrderItems->nItems() == 0);
  vRestoreMemberContext(&xContext);

  // Put the expression subquery marker onto the expression subquery list
  // so that it will be picked up upstream.
  m_pExprSubqueryList->vAppend(pExprSubquery);

  // Now create a SubqueryRefExpr node and reference the ExprSubquery
  QueryNode* pSubqueryRefExpr = SubqueryRefExpr::pCreate(ExprSubquery::szTableAlias(pExprSubquery));

  // Form the IN expression and add the subquery to it
  QueryNode* pInExpr = InExpr::pCreate(pLeftExpr);
  pInExpr->vAppendChild(pSubqueryRefExpr);

  // Put the IN operator back onto the expression stack
  if (bInvert) {
    QueryNode* pNotExpr = NotExpr::pCreate(pInExpr);
    m_pExprStack->vPush(pNotExpr);

  } else {
    m_pExprStack->vPush(pInExpr);
  }

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* IN Expression Operator (NO subquery child)                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitInOp(sqlParser::InOpContext *pCtx)
{
  // Save and clear the member variables in case we hit any subqueries
  PlannerContext xContext = xSaveMemberContext();

  ASSERT(!m_pExprStack);
  m_pExprStack = new QueryNodeList;

  ASSERT(m_pTableSubqueryList);
  ASSERT(m_pExprSubqueryList);

  try {
    visitChildren(pCtx);

  } catch (SqlException& e) {
    // Cleanup current member variables and restore all of the saved stacks/lists.
    vRestoreMemberContext(&xContext);
    throw;
  }

  while (m_pTableSubqueryList && m_pTableSubqueryList->nItems() > 0) {
    QueryNode* pTableSubqueryNode = m_pTableSubqueryList->pPop();
    ASSERT(TableSubquery::bIsOfType(pTableSubqueryNode));

    xContext.pOldTableSubqueryList->vAppend(pTableSubqueryNode);
  }

  while (m_pExprSubqueryList && m_pExprSubqueryList->nItems() > 0) {
    QueryNode* pExprSubqueryNode = m_pExprSubqueryList->pPop();
    ASSERT(ExprSubquery::bIsOfType(pExprSubqueryNode));

    xContext.pOldExprSubqueryList->vAppend(pExprSubqueryNode);
  }

  if (m_pExprStack->nItems() < 2) {
    vRestoreMemberContext(&xContext);
    SyntaxException::vThrow(-1, -1, EC_INSUFFICIENT_EXPRS, ": %s"_Stz, "IN clause");
  }

  ASSERT(m_pExprStack->nItems() >= 2);

  QueryNode* pLeftExpr = m_pExprStack->pPopTail();
  ASSERT(pLeftExpr);
  ASSERT(Expr::bIsOfType(pLeftExpr));

  // Create the InExpr
  QueryNode* pInExpr = InExpr::pCreate(pLeftExpr);

  // Pop the expressions of the stack. The last one is the left side expression
  while (m_pExprStack->nItems() > 0) {
    QueryNode* pExpr = m_pExprStack->pPopTail();
    pInExpr->vAppendChild(pExpr);
  }

  // Restore the member variables
  ASSERT(m_pStatementStack->nItems() == 0);
  ASSERT(m_pTableExprStack->nItems() == 0);
  ASSERT(!m_pTableSubqueryList || m_pTableSubqueryList->nItems() == 0);
  ASSERT(!m_pExprSubqueryList || m_pExprSubqueryList->nItems() == 0);
  ASSERT(!m_pExprStack || m_pExprStack->nItems() == 0);
  ASSERT(!m_pSelectItems || m_pSelectItems->nItems() == 0);
  ASSERT(!m_pOrderItems || m_pOrderItems->nItems() == 0);
  vRestoreMemberContext(&xContext);

  // Put the IN operator back onto the expression stack
  b1 bInvert = (pCtx->NOT() != nullptr);

  if (bInvert) {
    QueryNode* pNotExpr = NotExpr::pCreate(pInExpr);
    m_pExprStack->vPush(pNotExpr);

  } else {
    m_pExprStack->vPush(pInExpr);
  }

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* EXISTS Expression Operator                                                 */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSubqueryExistsOp(sqlParser::SubqueryExistsOpContext *pCtx)
{
  // Get the statement string for this part of the query
  antlr4::misc::Interval interval(pCtx->query()->getStart()->getStartIndex(),
                                  pCtx->query()->getStop()->getStopIndex());
  std::string queryStlStr = pCtx->start->getInputStream()->getText(interval);


  // Save and clear the member variables in case we hit any subqueries
  PlannerContext xContext = xSaveMemberContext();

  try {
    visitChildren(pCtx);

  } catch (SqlException& e) {
    // Cleanup current member variables and restore all of the saved stacks/lists.
    vRestoreMemberContext(&xContext);
    throw;
  }

  // Create a ExprSubquery node
  // First make up a unique name for its table alias
  s256 sSubqueryName;
  vGenerateSubqueryName(sSubqueryName);

  QueryNode* pExprSubquery = ExprSubquery::pCreate(sSubqueryName.szStz(), (Stz*)(queryStlStr.c_str()));

  // We expect there to be one or more nodes in m_pStatementStack,
  // corresponding to a MainSubquery and zero or more TableSubquery nodes.
  // Pop them off and add them as children of the ExprSubquery node

  ASSERT(m_pStatementStack);
  while (m_pStatementStack->nItems() > 0) {
    QueryNode* pSubqueryMarker = m_pStatementStack->pPop();
    ASSERT(SubqueryMarker::bIsOfType(pSubqueryMarker));

    pExprSubquery->vPushChild(pSubqueryMarker);
  }

  // Restore the member variables
  ASSERT(m_pStatementStack->nItems() == 0);
  ASSERT(m_pTableExprStack->nItems() == 0);
  ASSERT(!m_pTableSubqueryList || m_pTableSubqueryList->nItems() == 0);
  ASSERT(!m_pExprSubqueryList || m_pExprSubqueryList->nItems() == 0);
  ASSERT(!m_pExprStack || m_pExprStack->nItems() == 0);
  ASSERT(!m_pSelectItems || m_pSelectItems->nItems() == 0);
  ASSERT(!m_pOrderItems || m_pOrderItems->nItems() == 0);
  vRestoreMemberContext(&xContext);

  // Put the expression subquery marker onto the expression subquery list
  // so that it will be picked up upstream.
  m_pExprSubqueryList->vAppend(pExprSubquery);

  // Now create a SubqueryRefExpr node and reference the ExprSubquery
  QueryNode* pSubqueryRef = SubqueryRefExpr::pCreate(ExprSubquery::szTableAlias(pExprSubquery));

  // Create the ExistsExpr
  QueryNode* pExistsExpr = ExistsExpr::pCreate(pSubqueryRef);

  // Put the EXISTS operator back onto the expression stack
  b1 bInvert = (pCtx->NOT() != nullptr);

  if (bInvert) {
    QueryNode* pNotExpr = NotExpr::pCreate(pExistsExpr);
    m_pExprStack->vPush(pNotExpr);

  } else {
    m_pExprStack->vPush(pExistsExpr);
  }

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* LIKE Expression Operator without ESCAPE clause                             */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitLikeOp(sqlParser::LikeOpContext *pCtx)
{
  ASSERT(m_pExprStack);

  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() >= 2);

  QueryNode* pPatternExpr   = m_pExprStack->pPop();
  QueryNode* pPredicandExpr = m_pExprStack->pPop();

  QueryNode* pLikeExpr = LikeExpr::pCreate(pPredicandExpr, pPatternExpr, nullptr);

  // Put the LIKE operator back onto the expression stack
  b1 bInvert = (pCtx->NOT() != nullptr);

  if (bInvert) {
    QueryNode* pNotExpr = NotExpr::pCreate(pLikeExpr);
    m_pExprStack->vPush(pNotExpr);

  } else {
    m_pExprStack->vPush(pLikeExpr);
  }

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* LIKE Expression Operator with ESCAPE clause                                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitLikeEscapeOp(sqlParser::LikeEscapeOpContext *pCtx)
{
  ASSERT(m_pExprStack);

  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() >= 3);

  QueryNode* pEscapeExpr    = m_pExprStack->pPop();
  QueryNode* pPatternExpr   = m_pExprStack->pPop();
  QueryNode* pPredicandExpr = m_pExprStack->pPop();

  if (pEscapeExpr &&
      Expr::bIsFullyLiteral(pEscapeExpr) &&
      StringLiteral::bIsOfType(pEscapeExpr)) {

    Stz* szEscape = StringLiteral::szValue(pEscapeExpr);
    if (szEscape->nLength() > 1) {
      s2K sEscapeBuf;
      sEscapeBuf.vCopy(StringLiteral::szValue(pEscapeExpr));

      delete pEscapeExpr;
      delete pPatternExpr;
      delete pPredicandExpr;

      SemanticException::vThrow(EC_ESCAPE_SINGLE_CHAR, ": '%s'"_Stz, sEscapeBuf.szStz());
    }
  }

  QueryNode* pLikeExpr = LikeExpr::pCreate(pPredicandExpr, pPatternExpr, pEscapeExpr);

  // Put the LIKE operator back onto the expression stack
  b1 bInvert = (pCtx->NOT() != nullptr);

  if (bInvert) {
    QueryNode* pNotExpr = NotExpr::pCreate(pLikeExpr);
    m_pExprStack->vPush(pNotExpr);

  } else {
    m_pExprStack->vPush(pLikeExpr);
  }

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* BETWEEN Expression Operator                                                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitBetweenOp(sqlParser::BetweenOpContext *pCtx)
{
  ASSERT(m_pExprStack);

  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() >= 3);

  QueryNode* pUpperBound    = m_pExprStack->pPop();
  QueryNode* pLowerBound    = m_pExprStack->pPop();
  QueryNode* pPredicandExpr = m_pExprStack->pPop();

  QueryNode* pBetweenExpr = BetweenExpr::pCreate(pPredicandExpr, pLowerBound, pUpperBound);

  // Put the BETWEEN operator back onto the expression stack
  b1 bInvert = (pCtx->NOT() != nullptr);

  if (bInvert) {
    QueryNode* pNotExpr = NotExpr::pCreate(pBetweenExpr);
    m_pExprStack->vPush(pNotExpr);

  } else {
    m_pExprStack->vPush(pBetweenExpr);
  }

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* EXTRACT Expression Operator                                                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitExtractFunc(sqlParser::ExtractFuncContext* pCtx)
{
  ASSERT(m_pExprStack);

  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() >= 1);

  QueryNode* pSourceExpr = m_pExprStack->pPop();
  ASSERT(Expr::bIsOfType(pSourceExpr));

  QueryNode* pExtractField = nullptr;
  if      (pCtx->YEAR())   pExtractField = ExtractField::pCreate(ExtractField::EF_YEAR);
  else if (pCtx->MONTH())  pExtractField = ExtractField::pCreate(ExtractField::EF_MONTH);
  else if (pCtx->DAY())    pExtractField = ExtractField::pCreate(ExtractField::EF_DAY);
  else if (pCtx->HOUR())   pExtractField = ExtractField::pCreate(ExtractField::EF_HOUR);
  else if (pCtx->MINUTE()) pExtractField = ExtractField::pCreate(ExtractField::EF_MINUTE);
  else if (pCtx->SECOND()) pExtractField = ExtractField::pCreate(ExtractField::EF_SECOND);
  else    NEVER_REACH

  QueryNode* pBetweenExpr = ExtractExpr::pCreate(pExtractField, pSourceExpr);

  // Put the EXTRACT operator back onto the expression stack
  m_pExprStack->vPush(pBetweenExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SUBSTRING Expression Operator                                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitSubStringFunc(sqlParser::SubStringFuncContext* pCtx)
{
  ASSERT(m_pExprStack);

  PlannerContext xPlannerContext = xSaveMemberContext();
  ASSERT(!m_pExprStack);
  m_pExprStack = new QueryNodeList();

  try {
    visitChildren(pCtx);

  } catch (SqlException& e) {
    vRestoreMemberContext(&xPlannerContext);
    throw;
  }

  ASSERT(m_pExprStack->nItems() >= 2);

  QueryNode* pLengthExpr = nullptr;

  if (m_pExprStack->nItems() == 3) {
    pLengthExpr = m_pExprStack->pPop();
  }

  QueryNode* pFromExpr = m_pExprStack->pPop();
  QueryNode* pSrcExpr  = m_pExprStack->pPop();

  ASSERT(pSrcExpr);
  ASSERT(Expr::bIsOfType(pSrcExpr));
  ASSERT(pFromExpr);
  ASSERT(Expr::bIsOfType(pFromExpr));
  ASSERT(!pLengthExpr || Expr::bIsOfType(pLengthExpr));

  vRestoreMemberContext(&xPlannerContext);

  if (IntegerLiteral::bIsOfType(pFromExpr) && IntegerLiteral::iValue(pFromExpr) < 0) {
    i8 iVal = IntegerLiteral::iValue(pFromExpr);
    delete pLengthExpr;
    delete pFromExpr;
    delete pSrcExpr;
    vRestoreMemberContext(&xPlannerContext);
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, ": SUBSTRING FROM value: %lld"_Stz, iVal);
  }

  if (pLengthExpr && IntegerLiteral::bIsOfType(pLengthExpr) && IntegerLiteral::iValue(pLengthExpr) < 0) {
    i8 iVal = IntegerLiteral::iValue(pLengthExpr);
    delete pLengthExpr;
    delete pFromExpr;
    delete pSrcExpr;
    vRestoreMemberContext(&xPlannerContext);
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, ": SUBSTRING LENGTH value: %lld"_Stz, iVal);
  }

  QueryNode* pSubstringExpr = SubstringExpr::pCreate(pSrcExpr, pFromExpr, pLengthExpr);

  // Put the SUBSTRING operator back onto the expression stack
  m_pExprStack->vPush(pSubstringExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CASE Expression Operator                                                   */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitCaseOp(sqlParser::CaseOpContext* pCtx)
{
  PlannerContext xPlannerContext = xSaveMemberContext();
  ASSERT(!m_pExprStack);
  m_pExprStack = new QueryNodeList();

  try {
    visitChildren(pCtx);

  } catch (SqlException& e) {
    vRestoreMemberContext(&xPlannerContext);
    throw;
  }

  ASSERT(m_pExprStack->nItems() >= 2);

  QueryNode* pElseVal = (pCtx->elseExpr ? m_pExprStack->pPop() : NullLiteral::pCreate());
  QueryNode* pElseExpr = ElseExpr::pCreate(pElseVal);

  QueryNode* pCaseExpr = CaseExpr::pCreate(pElseExpr);

  if (pCtx->caseExpr) {
    // We have a "simple case" with a separate value and comparison operand.
    // Need to form a proper boolean predicate expression out of them.

    ASSERT(m_pExprStack->nItems() >= 3);
    QueryNode* pCaseVal = m_pExprStack->pPopTail();

    while (m_pExprStack->nItems() > 0) {
      ASSERT(m_pExprStack->nItems() >= 2);
      QueryNode* pResultExpr = m_pExprStack->pPop();
      QueryNode* pOperandExpr = m_pExprStack->pPop();

      // Form a predicate
      QueryNode* pPredicateExpr = EqExpr::pCreate(pCaseVal->pDeepCopy(true), pOperandExpr);

      QueryNode* pWhenExpr = WhenExpr::pCreate(pPredicateExpr, pResultExpr);
      CaseExpr::vPushWhenExpr(pCaseExpr, pWhenExpr);
    }

    delete pCaseVal;

  } else {
    // We have a "searched case" wth the predicate condition already
    // formed for each WHEN clause

    while (m_pExprStack->nItems() > 0) {
      ASSERT(m_pExprStack->nItems() >= 2);
      QueryNode* pResultExpr = m_pExprStack->pPop();
      QueryNode* pPredicateExpr = m_pExprStack->pPop();

      QueryNode* pWhenExpr = WhenExpr::pCreate(pPredicateExpr, pResultExpr);
      CaseExpr::vPushWhenExpr(pCaseExpr, pWhenExpr);
    }
  }

  vRestoreMemberContext(&xPlannerContext);

  m_pExprStack->vPush(pCaseExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CAST Expression Operator                                                   */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitCastOp(sqlParser::CastOpContext* pCtx)
{
  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() > 0);
  QueryNode* pTargetExpr = m_pExprStack->pPop();
  ASSERT(Expr::bIsOfType(pTargetExpr));

  eCT eDataType = eExtractDataType(pCtx->dataType());

  QueryNode* pCastExpr = CastExpr::pCreate(pTargetExpr, eDataType);
  m_pExprStack->vPush(pCastExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* CAST Expression Operator                                                   */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitInlineCast(sqlParser::InlineCastContext* pCtx)
{
  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() > 0);
  QueryNode* pTargetExpr = m_pExprStack->pPop();
  ASSERT(Expr::bIsOfType(pTargetExpr));

  eCT eDataType = eExtractDataType(pCtx->dataType());

  QueryNode* pCastExpr = CastExpr::pCreate(pTargetExpr, eDataType);
  m_pExprStack->vPush(pCastExpr);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Order by list                                                              */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitOrderByList(sqlParser::OrderByListContext *pCtx)
{
  ASSERT(!m_pExprStack);

  m_pExprStack = new QueryNodeList();

  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() == 0);
  delete m_pExprStack;
  m_pExprStack = nullptr;

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Order by expression items                                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitOrderByListItem(sqlParser::OrderByListItemContext *pCtx)
{
  ASSERT(m_pOrderItems);
  ASSERT(m_pExprStack);

  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() > 0);
  QueryNode* pExpr = m_pExprStack->pPop();

  b1 bIsAscending = pCtx->DESC() == nullptr;

  QueryNode* pOrderItem = OrderItem::pCreate(pExpr, bIsAscending);
  m_pOrderItems->vAppend(pOrderItem);

  return nullptr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Group by expression items                                                  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
antlrcpp::Any SqlPlanner::visitGroupByListItem(sqlParser::GroupByListItemContext *pCtx)
{
  ASSERT(m_pExprStack);

  // We use the expression stack to collect the list of expressions,
  // however it arranges them as a stack (last in first out). We will
  // easily reverse it here for convenience.

  visitChildren(pCtx);

  ASSERT(m_pExprStack->nItems() > 0);
  QueryNode* pExpr = m_pExprStack->pPop();
  m_pExprStack->vAppend(pExpr);

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
/* Clean up a string token. Returned string is dynamically allocated.         */
/*----------------------------------------------------------------------------*/
Text* SqlPlanner::pCleanString(antlr4::Token* pToken)
{
  if (!pToken) {
    return nullptr;
  }

  return new Text((Stz*) pToken->getText().c_str());
}


/*----------------------------------------------------------------------------*/
/* Clean up a string literal. Returned string is written to the given buffer  */
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
/* Clean up a string literal. Returned string is written to the given buffer  */
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
/* Clean up a string literal. Returned string is written to the given buffer  */
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
/* Clean up a string literal. Returned string is written to the given buffer  */
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
i8 SqlPlanner::iCleanInt(sqlParser::SignedIntegerLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  return iCleanInt(sStr);
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
d8 SqlPlanner::dCleanFloat(sqlParser::SignedFloatLiteralContext *pCtx)
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
Bcd SqlPlanner::xCleanDecimal(sqlParser::DecimalLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  return xCleanDecimal((Stz*) sStr.c_str(), pCtx->getStart()->getLine(), pCtx->getStart()->getCharPositionInLine());
}

/*----------------------------------------------------------------------------*/
/* Return a decimal value (exact numeric) from a decimal literal token        */
/*----------------------------------------------------------------------------*/
Bcd SqlPlanner::xCleanDecimal(sqlParser::SignedDecimalLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  return xCleanDecimal((Stz*) sStr.c_str(), pCtx->getStart()->getLine(), pCtx->getStart()->getCharPositionInLine());
}

/*----------------------------------------------------------------------------*/
/* Return a decimal value (exact numeric) from a decimal literal token        */
/*----------------------------------------------------------------------------*/
Bcd SqlPlanner::xCleanDecimal(Stz* szStr, i8 iLine, i8 iColumn)
{
  ASSERT(szStr);

  i8 nLen = szStr->nLength();
  ASSERT(szStr->nLength() > 0);

  i8 iSign = (szStr->azChar()[0] == '-' || szStr->azChar()[0] == '+') ? 1 : 0;

  i8 iPointPos = StrLib::iFind(szStr->azChar(), '.');
  ASSERT(iPointPos >= 0);  // decimal literals must always contain one decimal point

  i8 iScale = nLen - iPointPos - 1;
  i8 iPrecision = nLen - iSign - 1;

  i8 iMantissa = 0;
  char* pcCrs = szStr->azChar();
  bool bResult = StrLib::bExtractDecimal(&pcCrs, iMantissa, iPrecision + iScale, iScale);

  if (!bResult) {
    SyntaxException::vThrow(iLine, iColumn,
                            EC_INVALID_VALUE,
                            ": Unable to parse decimal literal '%s'"_Stz, szStr);
  }

  // Check for overflows. This is dependent on the size of components of the Bcd_T structure
  if (iScale > 127 || iScale < -128) {
    SyntaxException::vThrow(iLine, iColumn,
                            EC_VALUE_OUT_OF_RANGE,
                            ": Max/min decimal scale supported is [-128, 127] : %s"_Stz, szStr);
  }

  if (iMantissa > 140737488355327 || iMantissa < -140737488355328) {
    // iDecimal > (2^47 - 1) || iDecimal < (-2^47)
    SyntaxException::vThrow(iLine, iColumn,
                            EC_VALUE_OUT_OF_RANGE,
                            ": Max/min decimal precision supported is [2^47, (2^47) - 1] : %s"_Stz, szStr);
  }



  ASSERT(sizeof(Bcd) == C_BYTES_PER_WORD);

  Bcd xBcd;
  xBcd.vSet(iMantissa, iScale);
  return xBcd;
}

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

  ASSERT(sizeof(Dte) == C_BYTES_PER_WORD);

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

  ASSERT(sizeof(Dte) == C_BYTES_PER_WORD);

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
    for (i8 iIdx = 20; iIdx < 26; iIdx++) {
      iMicro *= 10;
      if (iIdx < iLen) {
        iMicro += StrLib::iDigitToI8(pcBuf[iIdx]);
      }
    }
  }

  // Now check the values
  s256 sErrorMsg;
  if (!Tsp::bIsValid(iYear, iMonth, iDay, iHour, iMin, iSec, iMicro, sErrorMsg)) {
    SemanticException::vThrow(EC_INVALID_TIMESTAMP_FORMAT, ": %s"_Stz, sErrorMsg.szStz());
  }

  ASSERT(sizeof(Dte) == C_BYTES_PER_WORD);

  Tsp xVal;
  xVal.vSet(iYear, iMonth, iDay, iHour, iMin, iSec, iMicro);
  return xVal;
}

/*----------------------------------------------------------------------------*/
/* Extract the join type enumeration                                          */
/*----------------------------------------------------------------------------*/
eJT SqlPlanner::eExtractJoinType(sqlParser::JoinTypeContext *pCtx)
{
  // The null/default case
  if (!pCtx || pCtx->INNER()) {
    return JT_INNER;
  }

  if (pCtx->outer) {
    if (pCtx->LEFT())   return JT_LEFT_OUTER;
    if (pCtx->RIGHT())  return JT_RIGHT_OUTER;
    if (pCtx->FULL())   return JT_FULL_OUTER;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Extract the join type enumeration                                          */
/*----------------------------------------------------------------------------*/
eJT SqlPlanner::eExtractJoinType(sqlParser::ExtendedJoinTypeContext *pCtx)
{
  // The null/default case
  if (!pCtx) {
    return JT_INNER;
  }

  if (pCtx->joinType()) {
    return eExtractJoinType(pCtx->joinType());
  }

  if (pCtx->SEMI()) {
//    __NOT_IMPL3("semi-join");
    return JT_SEMI;
  }

  if (pCtx->ANTI()) {
//    __NOT_IMPL3("anti-join");
    return JT_ANTI;
  }

  NEVER_REACH
}

eCT SqlPlanner::eExtractDataType(sqlParser::DataTypeContext* pCtx)
{
  ASSERT(pCtx);
  sqlParser::SqlDataTypeContext* pSqlCtx = pCtx->sqlDataType();

  eCT eDataType = eCT::CT_NONE;

  if      (pSqlCtx->BOOLEAN())            { eDataType = eCT::CT_BOOLEAN;     }
  else if (pSqlCtx->INTEGER())            { eDataType = eCT::CT_INTEGER;     }
  else if (pSqlCtx->DECIMAL())            { eDataType = eCT::CT_DECIMAL;     }
  else if (pSqlCtx->FLOAT())              { eDataType = eCT::CT_FLOAT;       }
  else if (pSqlCtx->DATE())               { eDataType = eCT::CT_DATE;        }
  else if (pSqlCtx->TIME())               { eDataType = eCT::CT_TIME;        }
  else if (pSqlCtx->TIMESTAMP())          { eDataType = eCT::eCT::CT_TIMESTAMP;   }
//TODO:  else if (pSqlCtx->INTERVAL())           { eDataType = eCT::CT_INTERVAL_YM; }
  else if (pSqlCtx->fixedLenChar())       { eDataType = eCT::CT_CHAR;        }
  else if (pSqlCtx->varyingLenChar())     { eDataType = eCT::CT_VARCHAR;     }
  else if (pSqlCtx->BINARY())             {  __NOT_IMPL3("BINARY data type") }
  else if (pSqlCtx->VARBINARY())          {  __NOT_IMPL3("VARBINARY data type") }
  else if (pSqlCtx->BLOB())               {  __NOT_IMPL3("BLOB data type") }



  return eDataType;
}

/*----------------------------------------------------------------------------*/
/* Generate a unique name for an unnamed subquery                             */
/*----------------------------------------------------------------------------*/
void SqlPlanner::vGenerateSubqueryName(s256& sBuf)
{
  sBuf.vSprint("%s%04lld"_Stz, C_SUBQUERY_NAME_PREFIX, ++m_iSubqueryCounter);
}


/*----------------------------------------------------------------------------*/
/* Save & reset the member variables. Returns a context structure by value    */
/*----------------------------------------------------------------------------*/
SqlPlanner::PlannerContext SqlPlanner::xSaveMemberContext()
{
  PlannerContext xPlannerContext;

  // Save the context of the member variables
  xPlannerContext.pOldQueryHints = m_pQueryHints;
  m_pQueryHints = nullptr;

  xPlannerContext.pOldStatementStack = m_pStatementStack;
  m_pStatementStack = new QueryNodeStack();

  xPlannerContext.pOldTableExprStack = m_pTableExprStack;
  m_pTableExprStack = new QueryNodeStack();

  xPlannerContext.pOldTableSubqueryList = m_pTableSubqueryList;
  m_pTableSubqueryList = new QueryNodeList();

  xPlannerContext.pOldExprSubqueryList = m_pExprSubqueryList;
  m_pExprSubqueryList = new QueryNodeList();

  xPlannerContext.pOldExprStack = m_pExprStack;
  m_pExprStack = nullptr;

  xPlannerContext.pOldSelectItems = m_pSelectItems;
  m_pSelectItems = nullptr;

  xPlannerContext.pOldOrderItems = m_pOrderItems;
  m_pOrderItems = nullptr;

  // Return by value
  return xPlannerContext;
}

/*----------------------------------------------------------------------------*/
/* Restores the member variables from a previously saved context              */
/*----------------------------------------------------------------------------*/
void SqlPlanner::vRestoreMemberContext(PlannerContext* pxPlannerContext)
{
  if (m_pQueryHints) {
    delete m_pQueryHints;
    m_pQueryHints = nullptr;
  }
  m_pQueryHints = pxPlannerContext->pOldQueryHints;
  pxPlannerContext->pOldQueryHints = nullptr;

  if (m_pStatementStack) {
    while (m_pStatementStack->nItems() > 0) {
      QueryNode* pItem = m_pStatementStack->pPop();
      delete pItem;
    }
    delete m_pStatementStack;
    m_pStatementStack = nullptr;
  }
  m_pStatementStack = pxPlannerContext->pOldStatementStack;
  pxPlannerContext->pOldStatementStack = nullptr;

  if (m_pTableExprStack) {
    while (m_pTableExprStack->nItems() > 0) {
      QueryNode* pItem = m_pTableExprStack->pPop();
      delete pItem;
    }
    delete m_pTableExprStack;
    m_pTableExprStack = nullptr;
  }
  m_pTableExprStack = pxPlannerContext->pOldTableExprStack;
  pxPlannerContext->pOldTableExprStack = nullptr;

  if (m_pTableSubqueryList) {
    while (m_pTableSubqueryList->nItems() > 0) {
      QueryNode* pItem = m_pTableSubqueryList->pPop();
      // Move any outstanding table subqueries to the prior list
      if (!pxPlannerContext->pOldTableSubqueryList) {
        pxPlannerContext->pOldTableSubqueryList = new QueryNodeList();
      }
      ASSERT(pxPlannerContext->pOldTableSubqueryList);
      pxPlannerContext->pOldTableSubqueryList->vAppend(pItem);
    }
    delete m_pTableSubqueryList;
    m_pTableSubqueryList = nullptr;
  }
  m_pTableSubqueryList = pxPlannerContext->pOldTableSubqueryList;
  pxPlannerContext->pOldTableSubqueryList = nullptr;

  if (m_pExprSubqueryList) {
    while (m_pExprSubqueryList->nItems() > 0) {
      QueryNode* pItem = m_pExprSubqueryList->pPop();
      // Move any outstanding expression subqueries to the prior list
      if (!pxPlannerContext->pOldExprSubqueryList) {
        pxPlannerContext->pOldExprSubqueryList = new QueryNodeList();
      }
      ASSERT(pxPlannerContext->pOldExprSubqueryList);
      pxPlannerContext->pOldExprSubqueryList->vAppend(pItem);
    }
    delete m_pExprSubqueryList;
    m_pExprSubqueryList = nullptr;
  }
  m_pExprSubqueryList = pxPlannerContext->pOldExprSubqueryList;
  pxPlannerContext->pOldExprSubqueryList = nullptr;

  if (m_pExprStack) {
    while (m_pExprStack->nItems() > 0) {
      QueryNode* pItem = m_pExprStack->pPop();
      delete pItem;
    }
    delete m_pExprStack;
    m_pExprStack = nullptr;
  }
  m_pExprStack = pxPlannerContext->pOldExprStack;
  pxPlannerContext->pOldExprStack = nullptr;

  if (m_pSelectItems) {
    while (m_pSelectItems->nItems() > 0) {
      QueryNode* pItem = m_pSelectItems->pPop();
      delete pItem;
    }
    delete m_pSelectItems;
    m_pSelectItems = nullptr;
  }
  m_pSelectItems = pxPlannerContext->pOldSelectItems;
  pxPlannerContext->pOldSelectItems = nullptr;

  if (m_pOrderItems) {
    while (m_pOrderItems->nItems() > 0) {
      QueryNode* pItem = m_pOrderItems->pPop();
      delete pItem;
    }
    delete m_pOrderItems;
    m_pOrderItems = nullptr;
  }
  m_pOrderItems = pxPlannerContext->pOldOrderItems;
  pxPlannerContext->pOldOrderItems = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
