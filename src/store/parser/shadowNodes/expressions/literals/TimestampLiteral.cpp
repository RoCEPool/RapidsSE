/*============================================================================*/
/* TimestampLiteral.cpp - a timestamp literal expression                                */
/*============================================================================*/

#include "BASE.h"
#include "TimestampLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TIMESTAMP_LITERAL_NAME_PREFIX = "TimestampLiteral"_Stz;
static Stz* C_TIMESTAMP_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/timestamp"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* TimestampLiteral::pCreate(Tsp xTsp)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_TIMESTAMP_LITERAL_DEFN, sName);

  Value xVal(xTsp);

  QueryNode* pExpr = new QueryNode(sName, C_TIMESTAMP_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TimestampLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIMESTAMP);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    Tsp xTsp = pNode->xTspVal();
    sBuf.vCatSprint("TIMESTAMP '%04lld-%02lld-%02lld %02lld:%02lld:%02lld'"_Stz,
                    xTsp.iYear(), xTsp.iMonth(), xTsp.iDay(), xTsp.iHour(), xTsp.iMin(), xTsp.iSec());

    if (bHasMicros(pNode)) {
      sBuf.vCatSprint(".%06lld"_Stz, xTsp.iMicro());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the timestamp of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
Tsp TimestampLiteral::xValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIMESTAMP);

  if (pNode->bIsNullValue()) {
    Tsp xTsp;
    xTsp.vSetNull();
    return xTsp;

  } else {
    return pNode->xTspVal();
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the timestamp of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
b1 TimestampLiteral::bHasMicros(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIMESTAMP);

  if (pNode->bIsNullValue()) {
    return false;

  } else {
    return pNode->xTspVal().iMicro() != 0;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 TimestampLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TIMESTAMP_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TimestampLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TIMESTAMP_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT TimestampLiteral::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return CT_TIMESTAMP;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
