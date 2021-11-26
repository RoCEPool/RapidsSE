/*============================================================================*/
/* LiteralExpr.cpp - a literal expression                                     */
/*============================================================================*/

#include "BASE.h"
#include "LiteralExpr.hpp"
#include "StringLiteral.hpp"
#include "IntegerLiteral.hpp"
#include "FloatLiteral.hpp"
#include "BooleanLiteral.hpp"
#include "DateLiteral.hpp"
#include "TimeLiteral.hpp"
#include "TimestampLiteral.hpp"
#include "NullLiteral.hpp"
#include "eCT.h"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LITERAL_NAME_PREFIX = "ExprLiteral"_Stz;
static Stz* C_LITERAL_DEFN = "/__root__/definitions/object/expr/literal"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void LiteralExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (   StringLiteral::bIsOfType(pNode))  {    StringLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (  BooleanLiteral::bIsOfType(pNode))  {   BooleanLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (  IntegerLiteral::bIsOfType(pNode))  {   IntegerLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (    FloatLiteral::bIsOfType(pNode))  {     FloatLiteral::vDescribe(pNode, sBuf, pRootNode); }
//else if (DecimalLiteral::bIsOfType(pNode))    {   DecimalLiteral::vDescribe(pNode, sBuf, pRootNode);    }
  else if (     DateLiteral::bIsOfType(pNode))  {      DateLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (     TimeLiteral::bIsOfType(pNode))  {      TimeLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (TimestampLiteral::bIsOfType(pNode))  { TimestampLiteral::vDescribe(pNode, sBuf, pRootNode); }
//else if (IntervalLiteral::bIsOfType(pNode))   {  IntervalLiteral::vDescribe(pNode, sBuf, pRootNode);    }
  else if (     NullLiteral::bIsOfType(pNode))  {      NullLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns true if the node has a null value. NB: this is disinct from        */
/* the node representing a NULL literal.                                      */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsNull(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  return pNode->bIsNullValue();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT LiteralExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pRootNode);

  if      (      StringLiteral::bIsOfType(pExprNode))  { return CT_VARCHAR;     }
  else if (     BooleanLiteral::bIsOfType(pExprNode))  { return CT_BOOLEAN;     }
  else if (     IntegerLiteral::bIsOfType(pExprNode))  { return CT_INTEGER;     }
  else if (       FloatLiteral::bIsOfType(pExprNode))  { return CT_FLOAT;       }
//  else if    (DecimalLiteral::bIsOfType(pExprNode))  { return CT_DECIMAL;     }
  else if (        DateLiteral::bIsOfType(pExprNode))  { return CT_DATE;        }
  else if (        TimeLiteral::bIsOfType(pExprNode))  { return CT_TIME;        }
  else if (   TimestampLiteral::bIsOfType(pExprNode))  { return CT_TIMESTAMP;   }
//  else if (IntervalYMLiteral::bIsOfType(pExprNode))  { return CT_INTERVAL_YM; }
//  else if (IntervalDTLiteral::bIsOfType(pExprNode))  { return CT_INTERVAL_DT; }
  else if (      NullLiteral::bIsOfType(pExprNode))    { return CT_UNKNOWN;     }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns true if an expr can be implicitly coerced to a given data type     */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsCoercibleTo(eCT eTarget, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(LiteralExpr::bIsOfType(pExpr));

  // Note: An implicit type coercion is one that is done by the system automatically.
  //       A conversion that is not handled here doesn't mean that it isn't
  //       supported, it's just that the system isn't doing it automatically and
  //       therefore it is up to the user to *explicitly* apply a cast expression.

  eCT eCurrentType = LiteralExpr::eType(pExpr, pRootNode);

  // We can always coerce the current type to itself
  if (eCurrentType == eTarget)                              { return true; }

  Stz* szStr;

  switch (eCurrentType) {

    case CT_INTEGER:
      if (eTarget == CT_DECIMAL || eTarget == CT_FLOAT)     { return true; }
      break;

    case CT_DECIMAL:
      if (eTarget == CT_FLOAT)                              { return true; }
      break;

    // Note: The string conversions are dependent on value of the string itself.
    case CT_VARCHAR:  // fall through
    case CT_CHAR:
      szStr = StringLiteral::szValue(pExpr);

      if (eTarget == CT_INTEGER   && StrLib::bIsNumeric(szStr->azChar()))         { return true; }
      if (eTarget == CT_DECIMAL   && StrLib::bIsDecimalNo(szStr->azChar()))       { return true; }
      if (eTarget == CT_FLOAT     && StrLib::bIsFloatingPoint(szStr->azChar()))   { return true; }
      if (eTarget == CT_DATE      && DateLiteral::bIsDateStr(szStr))              { return true; }
      if (eTarget == CT_TIME      && TimeLiteral::bIsTimeStr(szStr))              { return true; }
      if (eTarget == CT_TIMESTAMP && TimestampLiteral::bIsTimestampStr(szStr))    { return true; }
      break;

    case CT_DATE:
      if (eTarget == CT_TIMESTAMP)                          { return true; }
      break;

    case CT_BOOLEAN:
      if (eTarget == CT_VARCHAR || eTarget == CT_CHAR)      { return true; }
      break;

    default:
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns a new expression object which represents pExpr coerced to eTarget  */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pCoerceTo(eCT eTarget, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(eTarget > CT_NONE);
  ASSERT(eTarget < CT_ENUMS);
  ASSERT(pExpr);
  ASSERT(LiteralExpr::bIsOfType(pExpr));
  ASSERT(LiteralExpr::eType(pExpr, pRootNode) != CT_NONE);

  QueryNode* pCoercedLiteral = nullptr;
  Stz* szStr;
  b1 bParseResult;
  char* pEndPtr;
  eCT eCurrentType = Expr::eType(pExpr, pRootNode);

  // Are we coercing the type to itself?
  if (eCurrentType == eTarget) {
    // Return a new copy as that is what is being expected
    return pExpr->pDeepCopy(true);
  }


  switch (eCurrentType) {

    case CT_INTEGER:
      if (eTarget == CT_DECIMAL) {
        // TODO: Implement when decimals are fully supported
//      return DecimalLiteral::pCreate((??) pExpr->iValue());
        NOT_IMPLEMENTED_YET
      } else if (eTarget == CT_FLOAT) {
        return FloatLiteral::pCreate((d8) pExpr->iValue());
      }
      NEVER_REACH

    case CT_DECIMAL:
      if (eTarget == CT_FLOAT) {
        // TODO: Implement when decimals are fully supported
//      return FloatLiteral::pCreate((d8) pExpr->??Value());
        NOT_IMPLEMENTED_YET
      }
      NEVER_REACH

      // Note: The string conversions are dependent on value of the string itself.
    case CT_VARCHAR:  // fall through
    case CT_CHAR:
      szStr = StringLiteral::szValue(pExpr);

      if (eTarget == CT_INTEGER) {
        return IntegerLiteral::pCreate(StrLib::iToI8(szStr->azChar()));

      } else if (eTarget == CT_DECIMAL) {
        // TODO: Implement when decimals are fully supported
//      return DecimalLiteral::pCreate(StrLib::??ToDecimal(szStr->azChar()));
        NOT_IMPLEMENTED_YET

      } else if (eTarget == CT_FLOAT) {
        d8 dVal = strtod(szStr->azChar(), &pEndPtr);
        ASSERT(pEndPtr == nullptr);
        return FloatLiteral::pCreate(dVal);

      } else if (eTarget == CT_DATE) {
        pCoercedLiteral = DateLiteral::pCreate(DateLiteral::xParseStr(szStr, bParseResult));
        ASSERT(bParseResult);
        return pCoercedLiteral;

      } else if (eTarget == CT_TIME) {
        pCoercedLiteral = TimeLiteral::pCreate(TimeLiteral::xParseStr(szStr, bParseResult));
        ASSERT(bParseResult);
        return pCoercedLiteral;

      } else if (eTarget == CT_TIMESTAMP) {
        pCoercedLiteral = TimestampLiteral::pCreate(TimestampLiteral::xParseStr(szStr, bParseResult));
        ASSERT(bParseResult);
        return pCoercedLiteral;
      }
      NEVER_REACH

    case CT_DATE:
      if (eTarget == CT_TIMESTAMP) {
        Dte xDte = pExpr->xDateVal();
        Tsp xTsp;
        xTsp.vSet(xDte.iYear(), xDte.iMonth(), xDte.iDay(), 0, 0, 0, 0);
        return TimestampLiteral::pCreate(xTsp);
      }
      NEVER_REACH

    case CT_BOOLEAN:
      if (eTarget == CT_VARCHAR || eTarget == CT_CHAR) {
        return StringLiteral::pCreate(pExpr->bValue() ? new Text("TRUE"_Stz) : new Text("FALSE"_Stz));
      }
      NEVER_REACH

    default:
      NEVER_REACH
  }

  NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
