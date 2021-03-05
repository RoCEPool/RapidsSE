/*============================================================================*/
/* NullLiteralExpr.cpp - a literal expression                                     */
/*============================================================================*/

#include "BASE.h"
#include "NullLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NULL_LITERAL_NAME_PREFIX = "NullLiteral"_Stz;
static Stz* C_NULL_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/null"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* NullLiteral::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_NULL_LITERAL_NAME_PREFIX, sName);

  Value xVal;
  xVal.vSetNull(Value::VT_UNKNOWN);

  QueryNode* pExpr = new QueryNode(sName, C_NULL_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void NullLiteral::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_UNKNOWN);

  sBuf.vConcat("NULL"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 NullLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_NULL_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT NullLiteral::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  NOT_IMPLEMENTED_YET
  //return CT_UNKNOWN; // TODO
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
