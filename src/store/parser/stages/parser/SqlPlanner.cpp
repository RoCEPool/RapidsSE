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
#include "TableRef.hpp"

#include "ColumnRef.hpp"
#include "store/parser/shadowNodes/expressions/literals/LiteralExpr.hpp"
#include "store/parser/shadowNodes/expressions/literals/NullLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/StringLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/IntegerLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/BooleanLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/FloatLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/DateLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/TimeLiteral.hpp"
#include "store/parser/shadowNodes/expressions/literals/TimestampLiteral.hpp"

#include "ProjectedExpr.hpp"

#include "ProjectOp.hpp"
#include "store/parser/shadowNodes/planObjects/dataSources/TableScanOp.hpp"

#include "ParserException.hpp"

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
    vTrace("%s\n"_Stz, errorListener.szErrorMsg()->azChar());
    m_sLastError.vCopy(errorListener.szErrorMsg());
    return nullptr;
  }
  
  // Visit the parse tree that resulted from the above parsing.
  try {
    this->visitStatement(pStatementCtx);

  } catch (SyntaxException& e) {
    m_sLastError.vCopy(e.szError());
    return nullptr;

  } catch (SemanticException& e) {
    m_sLastError.vCopy(e.szError());
    return nullptr;
  }

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

  // Explain the statement
//  s48K sExplainBuf;
//  Statement::vExplain(pStatementNode, sExplainBuf, 0);
//  vTrace("%s\n\n"_Stz, sExplainBuf.szStz());

  // Validate the plan subsection of the graph
  GraphValidator::vValidatePlan(pRoot);

  return pRoot;
}

/*----------------------------------------------------------------------------*/
/* Returns the last error message encountered, or nullptr if there is none.   */
/*----------------------------------------------------------------------------*/
Stz* SqlPlanner::szLastError()
{
  if (m_sLastError.nLength() == 0) {
    return nullptr;
  }

  return m_sLastError.szStz();
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
  m_pSelectItems = new QueryNodeList();

  // Visit the child parse tree nodes. The select list above will be populated.
  antlrcpp::Any any = visitChildren(pCtx);

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
antlrcpp::Any SqlPlanner::visitSelectList(sqlParser::SelectListContext *pCtx)
{
  ASSERT(m_pSelectItems);
  
  ASSERT(!m_pExprStack);
  m_pExprStack = new QueryNodeStack();
  
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

  QueryNode* pProjectedExpr = ProjectedExpr::pCreate(pExprNode, pAliasText);

  m_pSelectItems->vAppend(pProjectedExpr);

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

  QueryNode* pColumnRef = ColumnRef::pCreate(pCatalogName, pSchemaName,
                                             pTableName, pColumnName);

  m_pExprStack->vPush(pColumnRef);
  
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

  m_pExprStack->vPush(pLiteral);
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

  i8 idLen = idStr.length();

  if (idLen == 0) {
    return;
  }

  // Ensure that the string is short enough to fit in the buffer
  RASSERT(idLen < sBuf.nSpace());

  // Remove single quotes
  if (idStr.at(0) == '\'' && idStr.at(idLen - 1) == '\'') {
    // Not great, substr() makes a another copy of the string.
    sBuf.vCopy((Stz*) idStr.substr(1, idLen - 2).c_str());

    // Unescape any two consecutive single quotes
    while (StrLib::bReplace(sBuf.azChar(), "''", "'")) { /*noop*/ }
    return;
  }

  // Remove double quotes
  if (idStr.at(0) == '"' && idStr.at(idLen - 1) == '"') {
    // Not great, substr() makes a another copy of the string.
    sBuf.vCopy((Stz*) idStr.substr(1, idLen - 2).c_str());

    // Unescape any two consecutive double quotes
    while (StrLib::bReplace(sBuf.azChar(), "\"\"", "\"")) { /*noop*/ }
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* Return an integer value from an integer literal token                      */
/*----------------------------------------------------------------------------*/
i8 SqlPlanner::iCleanInt(sqlParser::IntegerLiteralContext *pCtx)
{
  ASSERT(pCtx);

  std::string sStr = pCtx->getText();
  i8 iVal;

  try {
    iVal = std::stoll(sStr);

  } catch (std::invalid_argument &e) {
    // TODO: handle exceptions
    NOT_IMPLEMENTED_YET

  } catch (std::out_of_range &e) {
    // TODO: handle exceptions
    NOT_IMPLEMENTED_YET
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
    // TODO: handle exceptions
    NOT_IMPLEMENTED_YET

  } catch (std::out_of_range &e) {
    // TODO: handle exceptions
    NOT_IMPLEMENTED_YET
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
    throw SyntaxException(_szInvalidFormat);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  if (sBuf.nLength() != 10) {
    throw SyntaxException(_szInvalidFormat);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[4] != '-' || pcBuf[7] != '-') {
    throw SyntaxException(_szInvalidFormat);
  }

  if (!StrLib::bIsDigit(pcBuf[0]) ||
      !StrLib::bIsDigit(pcBuf[1]) ||
      !StrLib::bIsDigit(pcBuf[2]) ||
      !StrLib::bIsDigit(pcBuf[3]) ||
      !StrLib::bIsDigit(pcBuf[5]) ||
      !StrLib::bIsDigit(pcBuf[6]) ||
      !StrLib::bIsDigit(pcBuf[8]) ||
      !StrLib::bIsDigit(pcBuf[9]) ) {
    throw SyntaxException(_szInvalidFormat);
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
    throw SyntaxException(_szInvalidFormat);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  if (sBuf.nLength() != 8) {
    throw SyntaxException(_szInvalidFormat);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[2] != ':' || pcBuf[5] != ':') {
    throw SyntaxException(_szInvalidFormat);
  }

  if (!StrLib::bIsDigit(pcBuf[0]) ||
      !StrLib::bIsDigit(pcBuf[1]) ||
      !StrLib::bIsDigit(pcBuf[3]) ||
      !StrLib::bIsDigit(pcBuf[4]) ||
      !StrLib::bIsDigit(pcBuf[6]) ||
      !StrLib::bIsDigit(pcBuf[7]) ) {
    throw SyntaxException(_szInvalidFormat);
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
    throw SyntaxException(_szInvalidFormat);
  }

  // Read it into the local buffer. This will also strip off the outer quotes.
  s256 sBuf;
  vCleanString(pCtx, sBuf);

  i8 iLen = sBuf.nLength();
  if (iLen < 19) {
    throw SyntaxException(_szInvalidFormat);
  }

  char* pcBuf = sBuf.azChar();

  // Validate the format
  if (pcBuf[4] != '-' || pcBuf[7] != '-' || pcBuf[10] != ' ' || pcBuf[13] != ':' || pcBuf[16] != ':') {
    throw SyntaxException(_szInvalidFormat);
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
    throw SyntaxException(_szInvalidFormat);
  }

  // check for optional micros
  b1 bHasMicros = false;

  if (iLen > 19) {
    if (pcBuf[19] != '.') {
      throw SyntaxException(_szInvalidFormat);
    }

    for (i8 iIdx = 20; iIdx < iLen; iIdx++) {
      if (iIdx > 25) {
        // can only have 6 digits of microseconds
        throw SyntaxException(_szInvalidFormat);
      }

      if (!StrLib::bIsDigit(pcBuf[iIdx])) {
        throw SyntaxException(_szInvalidFormat);
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
