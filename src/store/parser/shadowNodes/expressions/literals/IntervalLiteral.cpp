/*============================================================================*/
/* IntervalLiteral.hpp - a general interval literal expression                */
/*============================================================================*/

#include "BASE.h"
#include "IntervalLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_INTERVAL_LITERAL_NAME_PREFIX = "IntervalLiteral"_Stz;
static Stz* C_INTERVAL_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/interval"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* IntervalLiteral::pCreate(Ivl xIvlVal)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_INTERVAL_LITERAL_DEFN, sName);

  Value xVal(xIvlVal);

  QueryNode* pExpr = new QueryNode(sName, C_INTERVAL_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* IntervalLiteral::pCreateNull()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_INTERVAL_LITERAL_DEFN, sName);

  Value xVal{};
  xVal.vSetNull(Value::VT_INTERVAL);

  QueryNode* pExpr = new QueryNode(sName, C_INTERVAL_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void IntervalLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_INTERVAL);
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
    return;
  }

  Ivl xIvl = pNode->xIvlVal();
  eIV eIvlType = xIvl.eIvlType();
  Stz* szNeg = xIvl.bIsNeg() ? "-"_Stz : ""_Stz;

  // We can't sprintf from a tightly packed structure as the width of
  // each field is not regular or consistent. We can end up printing
  // part of an adjacent field. So copy the values onto the stack first,
  // then we can just sprintf with %lld.
  u8 uYears;
  u8 uMonths;
  u8 uDays;
  u8 uHours;
  u8 uMins;
  u8 uSecs;
  u8 uMicros;

  switch (eIvlType) {
    case IV_YEAR:
      uYears = xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths / 12;

      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      uYears,
                      aIV_field[eIvlType]);
      break;

    case IV_MONTH:
      uMonths = xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths;

      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      uMonths,
                      aIV_field[eIvlType]);
      break;

    case IV_YEAR_TO_MONTH:
      uYears = xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths / 12;
      uMonths = xIvl.T.m_xIvl.u.m_xIvlYM.m_uMonths % 12;

      sBuf.vCatSprint("INTERVAL %s'%lld-%lld' %s"_Stz,
                      szNeg,
                      uYears,
                      uMonths,
                      aIV_field[eIvlType]);
      break;

    case IV_DAY:
      uDays = xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M);

      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      uDays,
                      aIV_field[eIvlType]);
      break;

    case IV_HOUR:
      uHours = xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_HR * C_1M);

      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      uHours,
                      aIV_field[eIvlType]);
      break;

    case IV_MINUTE:
      uMins = xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_MIN * C_1M);
      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      uMins,
                      aIV_field[eIvlType]);
      break;

    case IV_SECOND:
      uSecs = xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / C_1M;
      uMicros = xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M;

      if (uMicros > 0) {
        sBuf.vCatSprint("INTERVAL %s'%lld.%06lld' %s"_Stz,
                        szNeg,
                        uSecs,
                        uMicros,
                        aIV_field[eIvlType]);
      } else {
        sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                        szNeg,
                        uSecs,
                        aIV_field[eIvlType]);
      }
      break;

    case IV_DAY_TO_HOUR:
      uDays  =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M);
      uHours = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_DAY * C_1M)) / (C_SECS_PER_HR * C_1M);

      sBuf.vCatSprint("INTERVAL %s'%lld %lld' %s"_Stz,
                      szNeg,
                      uDays,
                      uHours,
                      aIV_field[eIvlType]);
      break;

    case IV_DAY_TO_MINUTE:
      uDays  =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M);
      uHours = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_DAY * C_1M)) / (C_SECS_PER_HR * C_1M);
      uMins  = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR * C_1M)) / (C_SECS_PER_MIN * C_1M);

      sBuf.vCatSprint("INTERVAL %s'%lld %lld:%lld' %s"_Stz,
                      szNeg,
                      uDays,
                      uHours,
                      uMins,
                      aIV_field[eIvlType]);
      break;

    case IV_DAY_TO_SECOND:
      uDays   =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_DAY * C_1M);
      uHours  = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_DAY * C_1M)) / (C_SECS_PER_HR * C_1M);
      uMins   = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR  * C_1M)) / (C_SECS_PER_MIN * C_1M);
      uSecs   = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_MIN * C_1M)) / C_1M;
      uMicros =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M;

      if (uMicros > 0) {
        sBuf.vCatSprint("INTERVAL %s'%lld %lld:%lld:%lld.%06lld' %s"_Stz,
                        szNeg,
                        uDays,
                        uHours,
                        uMins,
                        uSecs,
                        uMicros,
                        aIV_field[eIvlType]);
      } else {
        sBuf.vCatSprint("INTERVAL %s'%lld %lld:%lld:%lld' %s"_Stz,
                        szNeg,
                        uDays,
                        uHours,
                        uMins,
                        uSecs,
                        aIV_field[eIvlType]);
      }
      break;

    case IV_HOUR_TO_MINUTE:
      uHours =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_HR * C_1M);
      uMins  = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR * C_1M)) / (C_SECS_PER_MIN * C_1M);

      sBuf.vCatSprint("INTERVAL %s'%lld:%lld' %s"_Stz,
                      szNeg,
                      uHours,
                      uMins,
                      aIV_field[eIvlType]);
      break;

    case IV_HOUR_TO_SECOND:
      uHours  =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_HR  * C_1M);
      uMins   = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_HR  * C_1M)) / (C_SECS_PER_MIN * C_1M);
      uSecs   = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_MIN * C_1M)) / C_1M;
      uMicros =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M;

      if (uMicros > 0) {
        sBuf.vCatSprint("INTERVAL %s'%lld:%lld:%lld.%06lld' %s"_Stz,
                        szNeg,
                        uHours,
                        uMins,
                        uSecs,
                        uMicros,
                        aIV_field[eIvlType]);
      } else {
        sBuf.vCatSprint("INTERVAL %s'%lld:%lld:%lld' %s"_Stz,
                        szNeg,
                        uHours,
                        uMins,
                        uSecs,
                        aIV_field[eIvlType]);
      }
      break;

    case IV_MINUTE_TO_SECOND:
      uMins   =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros / (C_SECS_PER_MIN * C_1M);
      uSecs   = (xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % (C_SECS_PER_MIN * C_1M)) / C_1M;
      uMicros =  xIvl.T.m_xIvl.u.m_xIvlDT.m_uMicros % C_1M;

      if (uMicros > 0) {
        sBuf.vCatSprint("INTERVAL %s'%lld:%lld.%06lld' %s"_Stz,
                        szNeg,
                        uMins,
                        uSecs,
                        uMicros,
                        aIV_field[eIvlType]);
      } else {
        sBuf.vCatSprint("INTERVAL %s'%lld:%lld' %s"_Stz,
                        szNeg,
                        uMins,
                        uSecs,
                        aIV_field[eIvlType]);
      }
      break;

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the interval of this expression.                                      */
/*----------------------------------------------------------------------------*/
Ivl IntervalLiteral::xValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_INTERVAL);

  return pNode->xIvlVal();
}

/*----------------------------------------------------------------------------*/
/* Sets the interval of this expression.                                       */
/*----------------------------------------------------------------------------*/
void IntervalLiteral::vSetValue(PlanNode* pNode, Ivl xVal)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_INTERVAL);
  pNode->pValue()->vSet(xVal);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 IntervalLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_INTERVAL_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IntervalLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_INTERVAL_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId IntervalLiteral::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pRootNode);

  return TypeId::xIdOf(pExprNode->xIvlVal().eIvlType());
}

/*----------------------------------------------------------------------------*/
/* Returns the specific type of interval subtype                              */
/*----------------------------------------------------------------------------*/
eIV IntervalLiteral::eIvlType(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  return pNode->xIvlVal().eIvlType();
}


/*----------------------------------------------------------------------------*/
/* Returns true if the interval falls into the year-month category            */
/*----------------------------------------------------------------------------*/
eCT IntervalLiteral::eIvlCategory(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  TypeId xType = TypeId::xIdOf(pNode->xIvlVal().eIvlType());
  return xType.eCTVal();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
