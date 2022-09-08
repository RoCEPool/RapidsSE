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

  switch (eIvlType) {
    case IV_YEAR:
      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlYr.m_uYears,
                      aIV_field[eIvlType]);
      break;

    case IV_MONTH:
      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlMon.m_uMonths,
                      aIV_field[eIvlType]);
      break;

    case IV_YEAR_TO_MONTH:
      sBuf.vCatSprint("INTERVAL %s'%lld-%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlYrToMon.m_uMonths / 12,
                      xIvl.T.m_xIvl.u.m_xIvlYrToMon.m_uMonths % 12,
                      aIV_field[eIvlType]);
      break;

    case IV_DAY:
      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlDay.m_uDays,
                      aIV_field[eIvlType]);
      break;

    case IV_HOUR:
      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlHr.m_uHours,
                      aIV_field[eIvlType]);
      break;

    case IV_MINUTE:
      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlMin.m_uMinutes,
                      aIV_field[eIvlType]);
      break;

    case IV_SECOND:
      sBuf.vCatSprint("INTERVAL %s'%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlSec.m_uSeconds,
                      aIV_field[eIvlType]);
      break;

    case IV_DAY_TO_HOUR:
      sBuf.vCatSprint("INTERVAL %s'%lld %lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlDayToHr.m_uDays,
                      xIvl.T.m_xIvl.u.m_xIvlDayToHr.m_uMinutes / 60,
                      aIV_field[eIvlType]);
      break;

    case IV_DAY_TO_MINUTE:
      sBuf.vCatSprint("INTERVAL %s'%lld %lld:%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlDayToMin.m_uDays,
                      xIvl.T.m_xIvl.u.m_xIvlDayToMin.m_uMinutes / 60,
                      xIvl.T.m_xIvl.u.m_xIvlDayToMin.m_uMinutes % 60,
                      aIV_field[eIvlType]);
      break;

    case IV_DAY_TO_SECOND:
      if (xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uFrac > 0) {
        sBuf.vCatSprint("INTERVAL %s'%lld %lld:%lld:%lld.%06lld' %s"_Stz,
                        szNeg,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uDays,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uMinutes / 60,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uMinutes % 60,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uSeconds,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uFrac,
                        aIV_field[eIvlType]);
      } else {
        sBuf.vCatSprint("INTERVAL %s'%lld %lld:%lld:%lld' %s"_Stz,
                        szNeg,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uDays,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uMinutes / 60,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uMinutes % 60,
                        xIvl.T.m_xIvl.u.m_xIvlDayToSec.m_uSeconds,
                        aIV_field[eIvlType]);
      }
      break;

    case IV_HOUR_TO_MINUTE:
      sBuf.vCatSprint("INTERVAL %s'%lld:%lld' %s"_Stz,
                      szNeg,
                      xIvl.T.m_xIvl.u.m_xIvlHrToMin.m_uMinutes / 60,
                      xIvl.T.m_xIvl.u.m_xIvlHrToMin.m_uMinutes % 60,
                      aIV_field[eIvlType]);
      break;

    case IV_HOUR_TO_SECOND:
      if (xIvl.T.m_xIvl.u.m_xIvlHrToSec.m_uFrac > 0) {
        sBuf.vCatSprint("INTERVAL %s'%lld:%lld:%lld.%06lld' %s"_Stz,
                        szNeg,
                        xIvl.T.m_xIvl.u.m_xIvlHrToSec.m_uMinutes / 60,
                        xIvl.T.m_xIvl.u.m_xIvlHrToSec.m_uMinutes % 60,
                        xIvl.T.m_xIvl.u.m_xIvlHrToSec.m_uSeconds,
                        xIvl.T.m_xIvl.u.m_xIvlHrToSec.m_uFrac,
                        aIV_field[eIvlType]);
      } else {
        sBuf.vCatSprint("INTERVAL %s'%lld:%lld:%lld' %s"_Stz,
                        szNeg,
                        xIvl.T.m_xIvl.u.m_xIvlHrToSec.m_uMinutes / 60,
                        xIvl.T.m_xIvl.u.m_xIvlHrToSec.m_uMinutes % 60,
                        xIvl.T.m_xIvl.u.m_xIvlHrToSec.m_uSeconds,
                        aIV_field[eIvlType]);
      }
      break;

    case IV_MINUTE_TO_SECOND:
      if (xIvl.T.m_xIvl.u.m_xIvlMinToSec.m_uFrac > 0) {
        sBuf.vCatSprint("INTERVAL %s'%lld:%lld.%06lld' %s"_Stz,
                        szNeg,
                        xIvl.T.m_xIvl.u.m_xIvlMinToSec.m_uMinutes % 60,
                        xIvl.T.m_xIvl.u.m_xIvlMinToSec.m_uSeconds,
                        xIvl.T.m_xIvl.u.m_xIvlMinToSec.m_uFrac,
                        aIV_field[eIvlType]);
      } else {
        sBuf.vCatSprint("INTERVAL %s'%lld:%lld' %s"_Stz,
                        szNeg,
                        xIvl.T.m_xIvl.u.m_xIvlMinToSec.m_uMinutes % 60,
                        xIvl.T.m_xIvl.u.m_xIvlMinToSec.m_uSeconds,
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
