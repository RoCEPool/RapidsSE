/*============================================================================*/
/* TimeLiteral.cpp - a time literal expression                                */
/*============================================================================*/

#include "BASE.h"
#include "TimeLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TIME_LITERAL_NAME_PREFIX = "TimeLiteral"_Stz;
static Stz* C_TIME_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/time"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* TimeLiteral::pCreate(Tme xTme)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_TIME_LITERAL_DEFN, sName);

  Value xVal(xTme);

  QueryNode* pExpr = new QueryNode(sName, C_TIME_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TimeLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIME);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    Tme xTme = pNode->xTimeVal();
    sBuf.vCatSprint("TIME '%02lld:%02lld:%02lld'"_Stz, xTme.iHour(), xTme.iMin(), xTme.iSec());
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the time of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
Tme TimeLiteral::xValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIME);

  if (pNode->bIsNullValue()) {
    Tme xTme;
    xTme.vSetNull();
    return xTme;

  } else {
    return pNode->xTimeVal();
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 TimeLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TIME_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TimeLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TIME_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT TimeLiteral::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return CT_TIME;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
