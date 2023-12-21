/*============================================================================*/
/* LiteralExpr.cpp - a literal expression                                     */
/*============================================================================*/

#include <cmath>
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
#include "IntervalLiteral.hpp"
#include "NullLiteral.hpp"
#include "eCT.h"
#include "TypeSystem.hpp"
#include "ComparisonExpr.hpp"
#include "TypeId.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "SqlException.hpp"
#include "eEC.h"

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
  else if ( IntervalLiteral::bIsOfType(pNode))  {  IntervalLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (     NullLiteral::bIsOfType(pNode))  {      NullLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void LiteralExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat(xTypeId(_pQueryNode(pNode), pRootNode).szTypeName());
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
TypeId LiteralExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pRootNode);

  if      (      StringLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_VARCHAR);     }
  else if (     BooleanLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_BOOLEAN);     }
  else if (     IntegerLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_INTEGER);     }
  else if (       FloatLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_FLOAT);       }
  else if (     DecimalLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_DECIMAL);     }
  else if (        DateLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_DATE);        }
  else if (        TimeLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_TIME);        }
  else if (   TimestampLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_TIMESTAMP);   }
  else if (    IntervalLiteral::bIsOfType(pExprNode))  { return IntervalLiteral::xTypeId(pExprNode, pRootNode); }
  else if (        NullLiteral::bIsOfType(pExprNode))  { return TypeId::xIdOf(CT_UNKNOWN);     }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 LiteralExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  if      (      StringLiteral::bIsOfType(pExprNode))  { return    StringLiteral::iPrecision(pExprNode,pRootNode); }
  else if (     BooleanLiteral::bIsOfType(pExprNode))  { return   BooleanLiteral::iPrecision(pExprNode,pRootNode); }
  else if (     IntegerLiteral::bIsOfType(pExprNode))  { return   IntegerLiteral::iPrecision(pExprNode,pRootNode); }
  else if (       FloatLiteral::bIsOfType(pExprNode))  { return     FloatLiteral::iPrecision(pExprNode,pRootNode); }
  else if (     DecimalLiteral::bIsOfType(pExprNode))  { return   DecimalLiteral::iPrecision(pExprNode,pRootNode); }
  else if (        DateLiteral::bIsOfType(pExprNode))  { return      DateLiteral::iPrecision(pExprNode,pRootNode); }
  else if (        TimeLiteral::bIsOfType(pExprNode))  { return      TimeLiteral::iPrecision(pExprNode,pRootNode); }
  else if (   TimestampLiteral::bIsOfType(pExprNode))  { return TimestampLiteral::iPrecision(pExprNode,pRootNode); }
  else if (    IntervalLiteral::bIsOfType(pExprNode))  { return  IntervalLiteral::iPrecision(pExprNode,pRootNode); }
  else if (        NullLiteral::bIsOfType(pExprNode))  { return      NullLiteral::iPrecision(pExprNode,pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 LiteralExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  if      (      StringLiteral::bIsOfType(pExprNode))  { return    StringLiteral::iScale(pExprNode,pRootNode); }
  else if (     BooleanLiteral::bIsOfType(pExprNode))  { return   BooleanLiteral::iScale(pExprNode,pRootNode); }
  else if (     IntegerLiteral::bIsOfType(pExprNode))  { return   IntegerLiteral::iScale(pExprNode,pRootNode); }
  else if (       FloatLiteral::bIsOfType(pExprNode))  { return     FloatLiteral::iScale(pExprNode,pRootNode); }
  else if (     DecimalLiteral::bIsOfType(pExprNode))  { return   DecimalLiteral::iScale(pExprNode,pRootNode); }
  else if (        DateLiteral::bIsOfType(pExprNode))  { return      DateLiteral::iScale(pExprNode,pRootNode); }
  else if (        TimeLiteral::bIsOfType(pExprNode))  { return      TimeLiteral::iScale(pExprNode,pRootNode); }
  else if (   TimestampLiteral::bIsOfType(pExprNode))  { return TimestampLiteral::iScale(pExprNode,pRootNode); }
  else if (    IntervalLiteral::bIsOfType(pExprNode))  { return  IntervalLiteral::iScale(pExprNode,pRootNode); }
  else if (        NullLiteral::bIsOfType(pExprNode))  { return      NullLiteral::iScale(pExprNode,pRootNode); }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns true if an expr can be implicitly coerced to a given data type     */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsImplicitlyCoercibleTo(TypeId xTargetTypeId, QueryNode* pExpr,
                                         QueryNode* pParentExpr, PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(LiteralExpr::bIsOfType(pExpr));

  // Note: An implicit type coercion is one that is done by the system automatically.
  //       A conversion that is not handled here doesn't mean that it isn't
  //       supported, it's just that the system isn't doing it automatically and
  //       therefore it is up to the user to *explicitly* apply a cast expression.

  TypeId xCurrentTypeId = LiteralExpr::xTypeId(pExpr, pRootNode);
  eCT eCurrentType = xCurrentTypeId.eCTVal();
  eCT eTargetType = xTargetTypeId.eCTVal();

  eIV eCurrentIvlType = xCurrentTypeId.eIVVal();


  // We can always coerce the current type to itself
  if (xCurrentTypeId == xTargetTypeId)                              { return true; }

  Stz* szStr;

  switch (eCurrentType) {

    case CT_INTEGER:
      if (eTargetType == CT_DECIMAL || eTargetType == CT_FLOAT)     { return true; }
      break;

    case CT_DECIMAL:
      if (eTargetType == CT_FLOAT)                              { return true; }
      break;

    // Note: The string conversions are dependent on value of the string itself.
    case CT_VARCHAR:  // fall through
    case CT_CHAR:
      szStr = StringLiteral::szValue(pExpr);

      if (eTargetType == CT_INTEGER && StrLib::bIsInteger(szStr->azChar()))         { return true; }
      if (eTargetType == CT_DECIMAL &&
          (
              StrLib::bIsInteger(szStr->azChar()) ||
              StrLib::bIsDecimal(szStr->azChar())
          ))   { return true; }
      if (eTargetType == CT_FLOAT &&
          (
              StrLib::bIsInteger(szStr->azChar()) ||
              StrLib::bIsDecimal(szStr->azChar()) ||
              StrLib::bIsFloat(szStr->azChar())
          ))   { return true; }
      if (eTargetType == CT_DATE && DateLiteral::bIsDateStr(szStr))              { return true; }
      if (eTargetType == CT_TIME && TimeLiteral::bIsTimeStr(szStr))              { return true; }
      if (eTargetType == CT_TIMESTAMP &&
          (
              DateLiteral::bIsDateStr(szStr) ||
              TimestampLiteral::bIsTimestampStr(szStr)
          ))   { return true; }
      if (eTargetType == CT_CHAR || eTargetType == CT_VARCHAR)      { return true; }
      break;

    case CT_DATE:
      if (eTargetType == CT_TIMESTAMP)                          { return true; }
      break;

    case CT_BOOLEAN:
      if (eTargetType == CT_VARCHAR || eTargetType == CT_CHAR)      { return true; }
      break;

    case CT_UNKNOWN:
      // This is coercible to any other type.
      return true;

    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
      if (bIsImplicitlyCoercibleTo(eCurrentIvlType, xTargetTypeId, pParentExpr, pRootNode)) {
        return true;
      }
      break;

    default:
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns true if an interval can be implicitly coerced to another interval  */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsImplicitlyCoercibleTo(eIV eCurrentIvl, TypeId xTargetTypeId,
                                         QueryNode* pParentExpr, PlanNode* pRootNode)
{
  eCT eTargetType = xTargetTypeId.eCTVal();
  eIV eTargetIvl  = xTargetTypeId.eIVVal();

  // Let's just separate whether the target type is another interval or not.
  // First let's look at the non-interval mappings supported.
  if (eTargetIvl == IV_NONE) {
    switch (eCurrentIvl) {
      case IV_YEAR:   // fall through
      case IV_MONTH:  // fall through
      case IV_DAY:    // fall through
      case IV_HOUR:   // fall through
      case IV_MINUTE: // fall through
      case IV_SECOND:
        // IMPORTANT: The coercion of single field intervals to an integer
        // is only applicable in certain contexts. In this case, only applicable
        // when occurring in a comparison with an integer. If we don't check for
        // this and just apply it broadly then the system ends up accepting some
        // blatently wrong expressions such as INTERVAL '1' YEAR + INTERVAL '1' DAY
        // because it casts both of them to an integer.
        //
        // NOTE: In the future we should support coercing an IV_SECOND to
        // a DECIMAL. But because our decimal handling only has scale of 2
        // we cannot handle the scale of 6 required for fractional seconds.
        // TODO: Also add a mapping of IV_SECOND to DECIMAL.
        if (eTargetType == CT_INTEGER && pParentExpr && ComparisonExpr::bIsOfType(pParentExpr)) {
          // There must be at least one integer child and any interval children must all be
          // of the same type. Remember that a BETWEEN expr has more than 2 children.
          bool bFoundInteger = false;
          eIV eIvlType = IV_NONE;

          PlanNodeIter* pChildIter = pParentExpr->pNewChildrenIter();
          while (pChildIter->bHasNext()) {
            PlanNode* pChild = pChildIter->pNext();
            TypeId xChildType = Expr::xTypeId(_pQueryNode(pChild), pRootNode);

            if (xChildType.eCTVal() == CT_INTEGER) {
              bFoundInteger = true;
              continue;
            }
            if (xChildType.eCTVal() == CT_INTERVAL_YM || xChildType.eCTVal() == CT_INTERVAL_DT) {
              if (eIvlType == IV_NONE) {
                eIvlType = xChildType.eIVVal();
                continue;
              }

              if (eIvlType != xChildType.eIVVal()) {
                // Found more than 1 interval type. This can occur in a BETWEEN expression.
                delete pChildIter;
                return false;
              }
            }

          }
          delete pChildIter;
          return bFoundInteger;
        }
        return false;

      default:
        return false;
    }

  } else {
    // We need to be careful about which interval types are compatible.
    // The general rule is that we support coercing amongst the same
    // interval class (day-time in this case) but the resulting type will
    // be a superset of the individual interval types.
    // Don't need to worry about coercing to the same type.

    // For starters, we cannot coerce a day-time interval to a year-month,
    // and vice versa.
    b1 bSameCategory = (bIsYearMonthInterval(eCurrentIvl) && bIsYearMonthInterval(eTargetIvl)) ||
                       (bIsDayTimeInterval(eCurrentIvl) && bIsDayTimeInterval(eTargetIvl));

    if (!bSameCategory) {
      return false;
    }

    switch (eCurrentIvl) {
      case IV_YEAR:
        switch (eTargetIvl) {
          case IV_YEAR:
          case IV_YEAR_TO_MONTH:
            return true;
          default:
            return false;
        }

      case IV_MONTH:
        switch (eTargetIvl) {
          case IV_MONTH:
          case IV_YEAR_TO_MONTH:
            return true;
          default:
            return false;
        }

      case IV_YEAR_TO_MONTH:
        switch (eTargetIvl) {
          case IV_YEAR_TO_MONTH:
            return true;
          default:
            return false;
        }

      case IV_DAY:
        switch (eTargetIvl) {
          case IV_DAY:
          case IV_DAY_TO_HOUR:
          case IV_DAY_TO_MINUTE:
          case IV_DAY_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_HOUR:
        switch (eTargetIvl) {
          case IV_HOUR:
          case IV_DAY_TO_HOUR:
          case IV_DAY_TO_MINUTE:
          case IV_DAY_TO_SECOND:
          case IV_HOUR_TO_MINUTE:
          case IV_HOUR_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_MINUTE:
        switch (eTargetIvl) {
          case IV_MINUTE:
          case IV_DAY_TO_MINUTE:
          case IV_DAY_TO_SECOND:
          case IV_HOUR_TO_MINUTE:
          case IV_HOUR_TO_SECOND:
          case IV_MINUTE_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_SECOND:
        switch (eTargetIvl) {
          case IV_SECOND:
          case IV_DAY_TO_SECOND:
          case IV_HOUR_TO_SECOND:
          case IV_MINUTE_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_DAY_TO_HOUR:
        switch (eTargetIvl) {
          case IV_DAY_TO_HOUR:
          case IV_DAY_TO_MINUTE:
          case IV_DAY_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_DAY_TO_MINUTE:
        switch (eTargetIvl) {
          case IV_DAY_TO_MINUTE:
          case IV_DAY_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_DAY_TO_SECOND:
        switch (eTargetIvl) {
          case IV_DAY_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_HOUR_TO_MINUTE:
        switch (eTargetIvl) {
          case IV_DAY_TO_SECOND:
          case IV_DAY_TO_MINUTE:
          case IV_HOUR_TO_MINUTE:
          case IV_HOUR_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_HOUR_TO_SECOND:
        switch (eTargetIvl) {
          case IV_DAY_TO_SECOND:
          case IV_HOUR_TO_SECOND:
            return true;
          default:
            return false;
        }

      case IV_MINUTE_TO_SECOND:
        switch (eTargetIvl) {
          case IV_DAY_TO_SECOND:
          case IV_HOUR_TO_SECOND:
          case IV_MINUTE_TO_SECOND:
            return true;
          default:
            return false;
        }

      default:
        NEVER_REACH
    }
  }

  return false;
}


/*----------------------------------------------------------------------------*/
/* Returns a new expression object which represents pExpr coerced to eTarget. */
/* Returns nullptr if nothing was coerced.                                    */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pCoerceTo(TypeId xTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(xTargetTypeId.bIsValid());
  ASSERT(pExpr);
  ASSERT(LiteralExpr::bIsOfType(pExpr));
  ASSERT(LiteralExpr::xTypeId(pExpr, pRootNode).eCTVal() != CT_NONE);

  TypeId xCurrentTypeId = Expr::xTypeId(pExpr, pRootNode);
  eCT eCurrentType = xCurrentTypeId.eCTVal();
  eCT eTargetType = xTargetTypeId.eCTVal();
  s2K sBuf;

  // Are we coercing the type to itself?
  if (xCurrentTypeId == xTargetTypeId) {
    // Return a new copy as that is what is being expected
    return pExpr->pDeepCopy(false);
  }


  switch (eCurrentType) {

    case CT_BOOLEAN:  // **************************************************** From CT_BOOLEAN to...
      switch (eTargetType) {
        case CT_CHAR:   // falls through
        case CT_VARCHAR:          return StringLiteral::pCreate(pExpr->bValue() ? new Text("TRUE"_Stz) : new Text("FALSE"_Stz));
        default:                  return nullptr;
      }

    // Note: The string conversions are dependent on value of the string itself.
    case CT_CHAR:     // fall through
    case CT_VARCHAR:  // ******************************************** From CT_CHAR/CT_VARCHAR to...
      switch (eTargetType) {
        case CT_DATE:             return      pVarcharToDate(pExpr, pRootNode);
        case CT_DECIMAL:          return   pVarcharToDecimal(pExpr, pRootNode);
        case CT_FLOAT:            return     pVarcharToFloat(pExpr, pRootNode);
        case CT_INTEGER:          return   pVarcharToInteger(pExpr, pRootNode);
        case CT_TIME:             return      pVarcharToTime(pExpr, pRootNode);
        case CT_TIMESTAMP:        return pVarcharToTimestamp(pExpr, pRootNode);
        case CT_CHAR:     // falls through
        case CT_VARCHAR:          return pExpr->pDeepCopy(false);   // expecting a new copy being returned.
        default:                  return nullptr;
      }

    case CT_DATE:     // ******************************************************* From CT_DATE to...
      switch (eTargetType) {
        case CT_TIMESTAMP:        return pDateToTimestamp(pExpr, pRootNode);
        case CT_CHAR:   // fall through
        case CT_VARCHAR:          return pLiteralToVarchar(pExpr, pRootNode);
        default:                  return nullptr;
      }


    case CT_DECIMAL:    // ************************************************** From CT_DECIMAL to...
      switch (eTargetType) {
        case CT_FLOAT:            return   pDecimalToFloat(pExpr, pRootNode);
        case CT_INTEGER:          return pDecimalToInteger(pExpr, pRootNode);
        case CT_CHAR:   // falls through
        case CT_VARCHAR:          return pLiteralToVarchar(pExpr, pRootNode);
        default:                  return nullptr;
      }


    case CT_FLOAT :    // ***************************************************** From CT_FLOAT to...
      switch (eTargetType) {
        case CT_DECIMAL:          return pFloatToDecimal(pExpr, pRootNode);
        case CT_INTEGER:          return IntegerLiteral::pCreate((i8) pExpr->dValue());
        case CT_CHAR:   // falls through
        case CT_VARCHAR:          return pLiteralToVarchar(pExpr, pRootNode);
        default:                  return nullptr;
      }

    case CT_INTEGER:    // ************************************************** From CT_INTEGER to...
      switch (eTargetType) {
        case CT_DECIMAL:          return pIntegerToDecimal(pExpr, pRootNode);
        case CT_FLOAT:            return FloatLiteral::pCreate((d8) pExpr->iValue());
        case CT_CHAR:   // fall through
        case CT_VARCHAR:          return pLiteralToVarchar(pExpr, pRootNode);
        default:                  return nullptr;
      }

    case CT_INTERVAL_DT:  // falls through
    case CT_INTERVAL_YM:  // ********************************************* From CT_INTERVAL_* to...
      switch (eTargetType) {
        case CT_DECIMAL:          return pIntervalToDecimal(pExpr, pRootNode, xCurrentTypeId.eIVVal());
        case CT_FLOAT:            return pIntervalToFloat(pExpr, pRootNode, xCurrentTypeId.eIVVal());
        case CT_INTEGER:          return pIntervalToInteger(pExpr, pRootNode, xCurrentTypeId.eIVVal());
        case CT_INTERVAL_DT:      return (eTargetType == eCurrentType) ? pIntervalToInterval(pExpr, pRootNode, xTargetTypeId.eIVVal()) : nullptr;
        case CT_INTERVAL_YM:      return (eTargetType == eCurrentType) ? pIntervalToInterval(pExpr, pRootNode, xTargetTypeId.eIVVal()) : nullptr;
        default:                  return nullptr;
      }

    case CT_TIME:     // ******************************************************* From CT_TIME to...
      switch (eTargetType) {
        case CT_TIMESTAMP:        return pDateToTimestamp(pExpr, pRootNode);
        case CT_CHAR:   // fall through
        case CT_VARCHAR:          return pLiteralToVarchar(pExpr, pRootNode);
        default:                  return nullptr;
      }

    case CT_TIMESTAMP:     // ********************************************* From CT_TIMESTAMP to...
      switch (eTargetType) {
        case CT_DATE:             return pTimestampToDate(pExpr, pRootNode);
        case CT_TIME:             return pTimestampToTime(pExpr, pRootNode);
        case CT_CHAR:   // fall through
        case CT_VARCHAR:          return pLiteralToVarchar(pExpr, pRootNode);
        default:                  return nullptr;
      }

    case CT_UNKNOWN:  // **************************************************** From CT_UNKNOWN to...
//      switch (eTargetType) {
//        case CT_BOOLEAN:          return   BooleanLiteral::pCreateNull();
//        case CT_CHAR:             return    StringLiteral::pCreateNull();
//        case CT_DATE:             return      DateLiteral::pCreateNull();
//        case CT_DECIMAL:          return   DecimalLiteral::pCreateNull();
//        case CT_FLOAT:            return     FloatLiteral::pCreateNull();
//        case CT_INTEGER:          return   IntegerLiteral::pCreateNull();
//        case CT_INTERVAL_DT:      return  IntervalLiteral::pCreateNull();
//        case CT_INTERVAL_YM:      return  IntervalLiteral::pCreateNull();
//        case CT_TIME:             return      TimeLiteral::pCreateNull();
//        case CT_TIMESTAMP:        return TimestampLiteral::pCreateNull();
//        case CT_VARCHAR:          return    StringLiteral::pCreateNull();
//        default:                  return nullptr;
//      }
      return NullLiteral::pCreate();    // TODO: this isn't right...

    default:
      NEVER_REACH
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Convert an integer literal to a varchar literal. May return null.          */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pLiteralToVarchar(QueryNode* pExpr, PlanNode* pRootNode)
{
  s2K sBuf;

  // Intervals currently not being cast to strings to be consistent
  // with the execution engine.
  if (IntervalLiteral::bIsOfType(pExpr)) {
    return nullptr;
  }

  if (TimestampLiteral::bIsOfType(pExpr)) {
    TimestampLiteral::vDescribeValueOnly(pExpr, sBuf, pRootNode);

  } else if (DateLiteral::bIsOfType(pExpr)) {
    DateLiteral::vDescribeValueOnly(pExpr, sBuf, pRootNode);

  } else if (TimeLiteral::bIsOfType(pExpr)) {
    TimeLiteral::vDescribeValueOnly(pExpr, sBuf, pRootNode);

  } else {
    LiteralExpr::vDescribe(pExpr, sBuf, pRootNode);
  }

  return StringLiteral::pCreate(new Text(sBuf.szStz()));
}

/*----------------------------------------------------------------------------*/
/* Convert a date literal to a timestamp literal. May return null.            */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pDateToTimestamp(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  Dte xDte = pExpr->xDateVal();
  Tsp xTsp;
  xTsp.vSet(xDte.iYear(), xDte.iMonth(), xDte.iDay(), 0, 0, 0, 0);
  return TimestampLiteral::pCreate(xTsp);
}

/*----------------------------------------------------------------------------*/
/* Convert an decimal literal to a float literal. May return null.            */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pDecimalToFloat(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  d8 dVal;
  Bcd xBcd;
  i8 iMagnitude;
  i8 iMaxMagnitude;

  xBcd = pExpr->xDecimalVal();

  // Check to see if the decimal is bigger than what we can fit into a float.
  // This covers both positive and negative values.
  iMagnitude = IntLib::iLog10(IntLib::iAbs(xBcd.T.m_iMantissa)) - xBcd.T.m_uScale;
  iMaxMagnitude = (i8) FltLib::dLog10(C_D8_MAX);

  if (iMagnitude > iMaxMagnitude) {
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, "decimal value exceeds float value limits"_Stz);
  }

  dVal = xBcd.T.m_iMantissa / FltLib::dPow(10, xBcd.T.m_uScale);
  return FloatLiteral::pCreate(dVal);
}

/*----------------------------------------------------------------------------*/
/* Convert an decimal literal to an integer literal. May return null.         */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pDecimalToInteger(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  i8 iVal;
  Bcd xBcd;
  i8 iMagnitude;
  i8 iMaxMagnitude;

  xBcd = pExpr->xDecimalVal();

  // Check to see if the decimal is bigger than what we can fit into an integer.
  // This covers both positive and negative values.
  iMagnitude = IntLib::iLog10(IntLib::iAbs(xBcd.T.m_iMantissa)) - xBcd.T.m_uScale;
  iMaxMagnitude = (i8) IntLib::iLog10(C_I8_MAX);

  if (iMagnitude > iMaxMagnitude) {
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, "decimal value exceeds integer value limits"_Stz);
  }

  iVal = xBcd.T.m_iMantissa / IntLib::iPow(10, xBcd.T.m_uScale);

  if (iVal > C_I8_MAX || iVal < C_I8_MIN) {
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, "decimal value exceeds integer value limits"_Stz);
  }
  return IntegerLiteral::pCreate(iVal);
}

/*----------------------------------------------------------------------------*/
/* Convert a float literal to a decimal literal. May return null.             */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pFloatToDecimal(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  d8 dVal;
  Bcd xBcd;
  s2K sBuf;

  // TODO: (Craig) When we support precision/scale calculus the coercision to
  //       a decimal should include the correct scale. For now we have a fixed
  //       scale of 2 so it is hard coded like that.

  // Test that the integer is not outside the decimal's range
  dVal = pExpr->dValue();
  if (dVal > C_BCD_MAX || dVal < C_BCD_MIN) {
    sBuf.vSprint(
        ": Float literal %9f is outside the valid decimal range of %lld to %lld"_Stz,
        dVal,
        C_BCD_MIN,
        C_BCD_MAX);
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, sBuf.szStz());
  }

  // REVISE: If the value is small enough then represent it with 2 decimal places
  // so it prints nicely. It should strictly have a scale of 0.
  if ( (dVal * 100) < C_BCD_MAX &&
       (dVal * 100) > C_BCD_MIN ) {
    xBcd.vSet(dVal * 100, 2);

  } else {
    xBcd.vSet((i8) pExpr->dValue(), 0);
  }

  return DecimalLiteral::pCreate(xBcd);
}


/*----------------------------------------------------------------------------*/
/* Convert an integer literal to a decimal literal. May return null.          */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pIntegerToDecimal(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  i8 iVal;
  Bcd xBcd;
  s2K sBuf;

  // TODO: (Craig) When we support precision/scale calculus the coercision to
  //       a decimal should include the correct scale. For now we have a fixed
  //       scale of 2 so it is hard coded like that.

  // Test that the integer is not outside the decimal's range
  iVal = pExpr->iValue();
  if (iVal > C_BCD_MAX || iVal < C_BCD_MIN) {
    sBuf.vSprint(
        ": Integer literal %lld is outside the valid decimal range of %lld to %lld"_Stz,
        iVal,
        C_BCD_MIN,
        C_BCD_MAX);
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, sBuf.szStz());
  }

  // REVISE: If the value is small enough then represent it with 2 decimal places
  // so it prints nicely. It should strictly have a scale of 0.
  if ( (iVal * 100) < C_BCD_MAX &&
       (iVal * 100) > C_BCD_MIN ) {
    xBcd.vSet(iVal * 100, 2);

  } else {
    xBcd.vSet(pExpr->iValue(), 0);
  }

  return DecimalLiteral::pCreate(xBcd);
}

/*----------------------------------------------------------------------------*/
/* Convert an interval literal to an decimal literal. May return null.        */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pIntervalToDecimal(QueryNode* pExpr, PlanNode* pRootNode, eIV eCurrentIVType)
{
  UNUSED(pRootNode);

  // TODO: Support coercing the INTERVAL SECOND to a DECIMAL with fractional second support

  Ivl xIvl = IntervalLiteral::xValue(pExpr);
  Bcd xBcd;

  // Extract the single field from the relevant intervals.
  switch (eCurrentIVType) {
    case IV_YEAR:   xBcd.vSet(xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths / 12, 0); break;
    case IV_MONTH:  xBcd.vSet(xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths, 0); break;
    case IV_DAY:    xBcd.vSet(xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_DAY), 0); break;
    case IV_HOUR:   xBcd.vSet(xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_HR), 0); break;
    case IV_MINUTE: xBcd.vSet(xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_MIN), 0); break;
    case IV_SECOND: xBcd.vSet(xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / C_1M, 0); break;

    default:
      return nullptr;
  }

  // Create the decimal literal and return it.
  QueryNode* pResultNode = DecimalLiteral::pCreate(xBcd);
  return pResultNode;
}

/*----------------------------------------------------------------------------*/
/* Convert an interval literal to an float literal. May return null.          */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pIntervalToFloat(QueryNode* pExpr, PlanNode* pRootNode, eIV eCurrentIVType)
{
  UNUSED(pRootNode);

  Ivl xIvl = IntervalLiteral::xValue(pExpr);
  d8 dVal;

  // Extract the single field from the relevant intervals.
  switch (eCurrentIVType) {
    case IV_YEAR:   dVal = (i8) xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths / 12; break;
    case IV_MONTH:  dVal = (i8) xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths; break;
    case IV_DAY:    dVal = (i8) xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_DAY); break;
    case IV_HOUR:   dVal = (i8) xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_HR); break;
    case IV_MINUTE: dVal = (i8) xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_MIN); break;
    case IV_SECOND: dVal = (i8) xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / C_1M; break;

    default:
      return nullptr;
  }

  // Create the integer decimal and return it.
  QueryNode* pResultNode = FloatLiteral::pCreate(dVal);
  return pResultNode;
}

/*----------------------------------------------------------------------------*/
/* Convert an interval literal to an integer literal. May return null.        */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pIntervalToInteger(QueryNode* pExpr, PlanNode* pRootNode, eIV eCurrentIVType)
{
  UNUSED(pRootNode);

  Ivl xIvl = IntervalLiteral::xValue(pExpr);
  i8 iVal;

  // Extract the single field from the relevant intervals.
  switch (eCurrentIVType) {
    case IV_YEAR:   iVal = (i8) xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths / 12; break;
    case IV_MONTH:  iVal = (i8) xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths; break;
    case IV_DAY:    iVal = (i8) xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_DAY); break;
    case IV_HOUR:   iVal = (i8) xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_HR); break;
    case IV_MINUTE: iVal = (i8) xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_1M * C_SECS_PER_MIN); break;
    case IV_SECOND: iVal = (i8) xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / C_1M; break;

    default:
      return nullptr;
  }

  // Create the integer literal and return it.
  QueryNode* pResultNode = IntegerLiteral::pCreate(iVal);
  return pResultNode;
}

/*----------------------------------------------------------------------------*/
/* Convert an interval literal to an interval literal. May return null.       */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pIntervalToInterval(QueryNode* pExpr, PlanNode* pRootNode, eIV eTargetIVType)
{
  UNUSED(pRootNode);

  Ivl xIvl = IntervalLiteral::xValue(pExpr);
  xIvl.T.m_xIvl.m_eType = eTargetIVType;
  return IntervalLiteral::pCreate(xIvl);
}

/*----------------------------------------------------------------------------*/
/* Convert a timestamp literal to an date literal. May return null.           */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pTimestampToDate(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  Tsp xTsp;
  Dte xDte;

  xTsp = TimestampLiteral::xValue(pExpr);
  xDte.vSet(xTsp.iYear(), xTsp.iMonth(), xTsp.iDay());

  QueryNode* pCoercedLiteral = DateLiteral::pCreate(xDte);
  return pCoercedLiteral;
}

/*----------------------------------------------------------------------------*/
/* Convert a timestamp literal to an time literal. May return null.           */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pTimestampToTime(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  Tsp xTsp;
  Tme xTme;

  xTsp = TimestampLiteral::xValue(pExpr);
  xTme.vSet(xTsp.iHour(), xTsp.iMin(), xTsp.iSec(), xTsp.iMicro());

  QueryNode* pCoercedLiteral = TimeLiteral::pCreate(xTme);
  return pCoercedLiteral;
}


/*----------------------------------------------------------------------------*/
/* Convert a varchar literal to a date literal. May return null.              */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pVarcharToDate(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

//  Stz* szStr = StringLiteral::szValue(pExpr);
//  b1 bParseResult;
//
//  Dte xDte = DateLiteral::xParseStr(szStr, bParseResult);
//  if (!bParseResult) {
//    return nullptr;
//  }
//
//  QueryNode* pCoercedLiteral = DateLiteral::pCreate(xDte);
//  return pCoercedLiteral;

  Stz* szStr = StringLiteral::szValue(pExpr);
  b1 bParseResult;

  if (DateLiteral::bIsDateStr(szStr)) {
    Dte xDte = DateLiteral::xParseStr(szStr, bParseResult);
    ASSERT(bParseResult);
    QueryNode* pCoercedLiteral = DateLiteral::pCreate(xDte);
    return pCoercedLiteral;

  } else if (TimestampLiteral::bIsTimestampStr(szStr)) {
    Tsp xTsp = TimestampLiteral::xParseStr(szStr, bParseResult);
    ASSERT(bParseResult);

    Dte xDte{};
    xDte.vSet(xTsp.iYear(), xTsp.iMonth(), xTsp.iDay());

    QueryNode* pCoercedLiteral = DateLiteral::pCreate(xDte);
    return pCoercedLiteral;

  } else {
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* Convert a varchar literal to a decimal literal. May return null.           */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pVarcharToDecimal(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  Stz* szStr = StringLiteral::szValue(pExpr);
  Bcd xBcd = DecimalLiteral::xParseStr(szStr);
  QueryNode* pCoercedLiteral = DecimalLiteral::pCreate(xBcd);
  return pCoercedLiteral;
}

/*----------------------------------------------------------------------------*/
/* Convert a varchar literal to a float literal. May return null.             */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pVarcharToFloat(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  d8 dVal;
  Stz* szStr = StringLiteral::szValue(pExpr);
  char* pEndPtr;

  dVal = strtod(szStr->azChar(), &pEndPtr);

  if (dVal == 0 && pEndPtr == szStr->azChar()) {
    // No conversion occurred
    SemanticException::vThrow(EC_INVALID_VALUE,
                              ": '%s': Unable to convert string literal to float literal"_Stz,
                              szStr);

  } else if ( (dVal == HUGE_VAL || dVal == -HUGE_VAL) && errno == ERANGE) {
    // Overflow
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE,
                              ": %s: Floating point overflow. Max/min floating point value supported is %10e to %10e"_Stz,
                              szStr,
                              C_FLT_MIN,
                              C_FLT_MAX);

  } else if (dVal == 0 && errno == ERANGE) {
    // Underflow
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE,
                              ": %s: Floating point underflow. Max/min floating point value supported is %10e to %10e"_Stz,
                              szStr,
                              C_FLT_MIN,
                              C_FLT_MAX);
  }

  if (dVal > C_FLT_MAX || dVal < C_FLT_MIN) {
    // Not an overflow, but still outside the floating point bounds accepted by RapidsSE
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE,
                              ": %s: Floating point overflow. Max/min floating point value supported is %10e to %10e"_Stz,
                              szStr,
                              C_FLT_MIN,
                              C_FLT_MAX);
  }

  ASSERT(pEndPtr == nullptr || *pEndPtr == 0);
  return FloatLiteral::pCreate(dVal);
}

/*----------------------------------------------------------------------------*/
/* Convert a varchar literal to an integer literal. May return null.          */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pVarcharToInteger(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  Stz* szStr = StringLiteral::szValue(pExpr);
  s2K sBuf;
  bool bOverflowed = false;
  bool bInvalid = false;
  i8 iVal = StrLib::iToI8(szStr->azChar(), &bInvalid, &bOverflowed);

  if (bInvalid) {
    SemanticException::vThrow(EC_INVALID_VALUE,
                              ": '%s': Unable to convert string literal to integer literal"_Stz,
                              szStr);
  }

  if (bOverflowed) {
    sBuf.vSprint(
        ": Integer literal %s is outside the valid integer range of %lld to %lld"_Stz,
        szStr,
        C_I8_MIN,
        C_I8_MAX);
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE, sBuf.szStz());        }

  QueryNode* pResultNode = IntegerLiteral::pCreate(iVal);
  return pResultNode;
}

/*----------------------------------------------------------------------------*/
/* Convert a varchar literal to a time literal. May return null.              */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pVarcharToTime(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  Stz* szStr = StringLiteral::szValue(pExpr);
  b1 bParseResult;

  if (TimeLiteral::bIsTimeStr(szStr)) {
    Tme xTme = TimeLiteral::xParseStr(szStr, bParseResult);
    ASSERT(bParseResult);
    QueryNode* pCoercedLiteral = TimeLiteral::pCreate(xTme);
    return pCoercedLiteral;

  } else if (TimestampLiteral::bIsTimestampStr(szStr)) {
    Tsp xTsp = TimestampLiteral::xParseStr(szStr, bParseResult);
    ASSERT(bParseResult);

    Tme xTme{};
    xTme.vSet(xTsp.iHour(), xTsp.iMin(), xTsp.iSec(), xTsp.iMicro());

    QueryNode* pCoercedLiteral = TimeLiteral::pCreate(xTme);
    return pCoercedLiteral;

  } else {
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* Convert a varchar literal to a timestamp literal. May return null.         */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pVarcharToTimestamp(QueryNode* pExpr, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  Stz* szStr = StringLiteral::szValue(pExpr);
  b1 bParseResult;
  QueryNode* pCoercedLiteral;

  if (TimestampLiteral::bIsTimestampStr(szStr)){
    pCoercedLiteral = TimestampLiteral::pCreate(TimestampLiteral::xParseStr(szStr, bParseResult));

  } else if (DateLiteral::bIsDateStr(szStr)) {
    Dte xDte = DateLiteral::xParseStr(szStr, bParseResult);
    Tsp xTsp;
    xTsp.vSet(xDte.iYear(), xDte.iMonth(), xDte.iDay(), 0, 0, 0, 0);
    pCoercedLiteral = TimestampLiteral::pCreate(xTsp);

  } else {
    return nullptr;
  }

  ASSERT(bParseResult);
  return pCoercedLiteral;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
