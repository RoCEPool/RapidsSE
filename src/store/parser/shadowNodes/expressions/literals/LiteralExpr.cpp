/*============================================================================*/
/* LiteralExpr.cpp - a literal expression                                     */
/*============================================================================*/

#include "BASE.h"
#include "LiteralExpr.hpp"
#include "StringLiteral.hpp"
#include "IntegerLiteral.hpp"
#include "FloatLiteral.hpp"
#include "DecimalLiteral.hpp"
#include "BooleanLiteral.hpp"
#include "DateLiteral.hpp"
#include "TimeLiteral.hpp"
#include "TimestampLiteral.hpp"
#include "NullLiteral.hpp"
#include "eCT.h"
#include "TypeSystem.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "SqlException.hpp"

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
  else if (  DecimalLiteral::bIsOfType(pNode))  {   DecimalLiteral::vDescribe(pNode, sBuf, pRootNode); }
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
i8 LiteralExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pRootNode);

  if      (      StringLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_VARCHAR);     }
  else if (     BooleanLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_BOOLEAN);     }
  else if (     IntegerLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_INTEGER);     }
  else if (       FloatLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_FLOAT);       }
  else if (     DecimalLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_DECIMAL);     }
  else if (        DateLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_DATE);        }
  else if (        TimeLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_TIME);        }
  else if (   TimestampLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_TIMESTAMP);   }
//  else if (IntervalYMLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_INTERVAL_YM); }
//  else if (IntervalDTLiteral::bIsOfType(pExprNode))  { return TypeSystem::iIdOf(CT_INTERVAL_DT); }
  else if (      NullLiteral::bIsOfType(pExprNode))    { return TypeSystem::iIdOf(CT_UNKNOWN);     }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns true if an expr can be implicitly coerced to a given data type     */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsImplicitlyCoercibleTo(i8 iTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(LiteralExpr::bIsOfType(pExpr));

  // Note: An implicit type coercion is one that is done by the system automatically.
  //       A conversion that is not handled here doesn't mean that it isn't
  //       supported, it's just that the system isn't doing it automatically and
  //       therefore it is up to the user to *explicitly* apply a cast expression.

  i8 iCurrentTypeId = LiteralExpr::iType(pExpr, pRootNode);
  eCT eCurrentType = TypeSystem::eCTVal(iCurrentTypeId);
  // TODO: handle intervals

  // We can always coerce the current type to itself
  if (eCurrentType == iTargetTypeId)                              { return true; }

  Stz* szStr;

  switch (eCurrentType) {

    case CT_INTEGER:
      if (iTargetTypeId == CT_DECIMAL || iTargetTypeId == CT_FLOAT)     { return true; }
      break;

    case CT_DECIMAL:
      if (iTargetTypeId == CT_FLOAT)                              { return true; }
      break;

    // Note: The string conversions are dependent on value of the string itself.
    case CT_VARCHAR:  // fall through
    case CT_CHAR:
      szStr = StringLiteral::szValue(pExpr);

      if (iTargetTypeId == CT_INTEGER && StrLib::bIsInteger(szStr->azChar()))         { return true; }
      if (iTargetTypeId == CT_DECIMAL &&
          (
              StrLib::bIsInteger(szStr->azChar()) ||
              StrLib::bIsDecimal(szStr->azChar())
          ))   { return true; }
      if (iTargetTypeId == CT_FLOAT &&
          (
              StrLib::bIsInteger(szStr->azChar()) ||
              StrLib::bIsDecimal(szStr->azChar()) ||
              StrLib::bIsFloat(szStr->azChar())
          ))   { return true; }
      if (iTargetTypeId == CT_DATE && DateLiteral::bIsDateStr(szStr))              { return true; }
      if (iTargetTypeId == CT_TIME && TimeLiteral::bIsTimeStr(szStr))              { return true; }
      if (iTargetTypeId == CT_TIMESTAMP &&
          (
              DateLiteral::bIsDateStr(szStr) ||
              TimestampLiteral::bIsTimestampStr(szStr)
          ))   { return true; }
      if (iTargetTypeId == CT_CHAR || iTargetTypeId == CT_VARCHAR)      { return true; }
      break;

    case CT_DATE:
      if (iTargetTypeId == CT_TIMESTAMP)                          { return true; }
      break;

    case CT_BOOLEAN:
      if (iTargetTypeId == CT_VARCHAR || iTargetTypeId == CT_CHAR)      { return true; }
      break;

    case CT_UNKNOWN:
      // This is coercible to any other type.
      return true;

    default:
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns a new expression object which represents pExpr coerced to eTarget  */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pCoerceTo(i8 iTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(iTargetTypeId > CT_NONE);
  ASSERT(iTargetTypeId < CT_ENUMS);
  ASSERT(pExpr);
  ASSERT(LiteralExpr::bIsOfType(pExpr));
  ASSERT(LiteralExpr::iType(pExpr, pRootNode) != CT_NONE);

  QueryNode* pCoercedLiteral = nullptr;
  Stz* szStr;
  b1 bParseResult;
  char* pEndPtr;
  i8 iCurrentTypeId = Expr::iType(pExpr, pRootNode);
  eCT eCurrentType = TypeSystem::eCTVal(iCurrentTypeId);
  // TODO: handle intervals

  // Are we coercing the type to itself?
  if (eCurrentType == iTargetTypeId) {
    // Return a new copy as that is what is being expected
    return pExpr->pDeepCopy(false);
  }


  switch (eCurrentType) {

    case CT_INTEGER:
      if (iTargetTypeId == CT_DECIMAL) {
        Bcd xBcd;
        xBcd.vSet(pExpr->iValue(), 0);
        return DecimalLiteral::pCreate(xBcd);

      } else if (iTargetTypeId == CT_FLOAT) {
        return FloatLiteral::pCreate((d8) pExpr->iValue());
      }
      NEVER_REACH

    case CT_DECIMAL:
      if (iTargetTypeId == CT_FLOAT) {
        Bcd xBcd = pExpr->xDecimalVal();

        // Check to see if the decimal is bigger than what we can fit into a float.
        // This covers both positive and negative values.
        i8 iMagnitude = IntLib::iLog10(IntLib::iAbs(xBcd.T.m_iMantissa)) - xBcd.T.m_iScale;
        i8 iMaxMagnitude = (i8) FltLib::dLog10(C_D8_MAX);

        if (iMagnitude > iMaxMagnitude) {
          SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, "decimal value exceeds float value limits"_Stz);
        }

        d8 dVal = xBcd.T.m_iMantissa / FltLib::dPow(10, xBcd.T.m_iScale);
        return FloatLiteral::pCreate(dVal);
      }
      NEVER_REACH

      // Note: The string conversions are dependent on value of the string itself.
    case CT_VARCHAR:  // fall through
    case CT_CHAR:
      szStr = StringLiteral::szValue(pExpr);

      if (iTargetTypeId == CT_INTEGER) {
        return IntegerLiteral::pCreate(StrLib::iToI8(szStr->azChar()));

      } else if (iTargetTypeId == CT_DECIMAL) {
        pCoercedLiteral = DecimalLiteral::pCreate(DecimalLiteral::xParseStr(szStr, bParseResult));
        ASSERT(bParseResult);
        return pCoercedLiteral;

      } else if (iTargetTypeId == CT_FLOAT) {
        d8 dVal = strtod(szStr->azChar(), &pEndPtr);
        ASSERT(pEndPtr == nullptr || *pEndPtr == 0);
        return FloatLiteral::pCreate(dVal);

      } else if (iTargetTypeId == CT_DATE) {
        pCoercedLiteral = DateLiteral::pCreate(DateLiteral::xParseStr(szStr, bParseResult));
        ASSERT(bParseResult);
        return pCoercedLiteral;

      } else if (iTargetTypeId == CT_TIME) {
        pCoercedLiteral = TimeLiteral::pCreate(TimeLiteral::xParseStr(szStr, bParseResult));
        ASSERT(bParseResult);
        return pCoercedLiteral;

      } else if (iTargetTypeId == CT_TIMESTAMP) {
        if (TimestampLiteral::bIsTimestampStr(szStr)){
          pCoercedLiteral = TimestampLiteral::pCreate(TimestampLiteral::xParseStr(szStr, bParseResult));

        } else if (DateLiteral::bIsDateStr(szStr)) {
          Dte xDte = DateLiteral::xParseStr(szStr, bParseResult);
          Tsp xTsp;
          xTsp.vSet(xDte.iYear(), xDte.iMonth(), xDte.iDay(), 0, 0, 0, 0);
          pCoercedLiteral = TimestampLiteral::pCreate(xTsp);

        } else {
          NEVER_REACH
        }

          ASSERT(bParseResult);
        return pCoercedLiteral;

      } else if (iTargetTypeId == CT_CHAR || iTargetTypeId == CT_VARCHAR) {
        return pExpr->pDeepCopy(false);
      }
      NEVER_REACH

    case CT_DATE:
      if (iTargetTypeId == CT_TIMESTAMP) {
        Dte xDte = pExpr->xDateVal();
        Tsp xTsp;
        xTsp.vSet(xDte.iYear(), xDte.iMonth(), xDte.iDay(), 0, 0, 0, 0);
        return TimestampLiteral::pCreate(xTsp);
      }
      NEVER_REACH

    case CT_BOOLEAN:
      if (iTargetTypeId == CT_VARCHAR || iTargetTypeId == CT_CHAR) {
        return StringLiteral::pCreate(pExpr->bValue() ? new Text("TRUE"_Stz) : new Text("FALSE"_Stz));
      }
      NEVER_REACH

    case CT_UNKNOWN:
      return NullLiteral::pCreate();

    default:
      NEVER_REACH
  }

  NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
