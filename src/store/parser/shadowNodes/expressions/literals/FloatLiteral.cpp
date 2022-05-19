/*============================================================================*/
/* FloatLiteral.cpp - a float literal expression                              */
/*============================================================================*/

#include "BASE.h"
#include "FloatLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_FLOAT_LITERAL_NAME_PREFIX = "FloatLiteral"_Stz;
static Stz* C_FLOAT_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/float"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* FloatLiteral::pCreate(d8 dVal)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_FLOAT_LITERAL_DEFN, sName);

  Value xVal(dVal);

  QueryNode* pExpr = new QueryNode(sName, C_FLOAT_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void FloatLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_FLOAT);
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    sBuf.vCatSprint("%e"_Stz, pNode->dValue());
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the float of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
d8 FloatLiteral::dValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_FLOAT);

  if (pNode->bIsNullValue()) {
    return 0;
  } else {
    return pNode->dValue();
  }
}

/*----------------------------------------------------------------------------*/
/* Sets the float of this expression. NB: call bIsNull() to check if the      */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
void FloatLiteral::vSetValue(PlanNode* pNode, d8 dVal)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_FLOAT);
  pNode->pValue()->vSet(dVal);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 FloatLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_FLOAT_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 FloatLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_FLOAT_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
i8 FloatLiteral::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return CT_FLOAT;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
