/*============================================================================*/
/* LiteralExpr.cpp - a literal expression                                     */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LITERAL_NAME_PREFIX = "ExprLiteral"_Stz;
static Stz* C_LITERAL_DEFN = "/__root__/definitions/object/expr/literal"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pCreate(eET eType, Value xVal)
{
  _static_

  ASSERT(eType > ET_NONE && eType < ET_NENUMS);

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_LITERAL_NAME_PREFIX, sName);

  QueryNode* pExpr = new QueryNode(sName, Expr::szTypeDefinition(eType), xVal);
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void LiteralExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  sBuf.vConcat(pNode->szName());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LITERAL_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
