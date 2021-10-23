/*============================================================================*/
/* StringLiteralExpr.cpp - a literal expression                                     */
/*============================================================================*/

#include "BASE.h"
#include "StringLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_STRING_LITERAL_NAME_PREFIX = "StringLiteral"_Stz;
static Stz* C_STRING_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/string"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* StringLiteral::pCreate(Text* pStr)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_STRING_LITERAL_DEFN, sName);

  Value xVal(pStr);

  QueryNode* pExpr = new QueryNode(sName, C_STRING_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void StringLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_STRING);
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    sBuf.vCatSprint("'%s'"_Stz, pNode->szValue());
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the string of this expression, or nullptr if it is a null value       */
/*----------------------------------------------------------------------------*/
Stz* StringLiteral::szValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_STRING);

  if (pNode->bIsNullValue()) {
    return nullptr;
  } else {
    return pNode->szValue();
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 StringLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STRING_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StringLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STRING_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT StringLiteral::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return CT_VARCHAR;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
