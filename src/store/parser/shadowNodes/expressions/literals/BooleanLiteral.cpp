/*============================================================================*/
/* BooleanLiteral.cpp - a boolean literal expression                          */
/*============================================================================*/

#include "BASE.h"
#include "BooleanLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_BOOLEAN_LITERAL_NAME_PREFIX = "BooleanLiteral"_Stz;
static Stz* C_BOOLEAN_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/boolean"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* BooleanLiteral::pCreate(b1 bVal)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_BOOLEAN_LITERAL_DEFN, sName);

  Value xVal(bVal);

  QueryNode* pExpr = new QueryNode(sName, C_BOOLEAN_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void BooleanLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_BOOLEAN);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    sBuf.vCatSprint("%s"_Stz, (pNode->bValue() ? "true"_Stz : "false"_Stz));
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the boolean of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
b1 BooleanLiteral::bValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_BOOLEAN);

  if (pNode->bIsNullValue()) {
    return 0;
  } else {
    return pNode->bValue();
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 BooleanLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_BOOLEAN_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 BooleanLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_BOOLEAN_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT BooleanLiteral::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return CT_BOOLEAN;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
