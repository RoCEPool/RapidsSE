/*============================================================================*/
/* ColumnParameter.hpp - A reference to a column by its parameter num (1-based)*/
/*============================================================================*/

#include "BASE.h"
#include "ColumnParameter.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "NodeNamer.hpp"

#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/expr/columnParameter"_Stz;

static Stz* C_SYMBOLICEXPR_PROP = "symbolicExpr"_Stz;
static Stz* C_SYMBOLICEXPR_DEFN = "/__root__/definitions/object/expr"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create expression column parameter nodes.               */
/* pSymExpr will be duplicated and saved as a parameter.                      */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnParameter::pCreate(i8 iParameter, QueryNode* pSymExpr)
{
  _static_
  ASSERT(iParameter > 0);
  ASSERT(pSymExpr);
  ASSERT(Expr::bIsOfType(pSymExpr));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  Value xValue(iParameter);

  QueryNode* pColParameter = new QueryNode(sName, C_DEFN, static_cast<Value&&>(xValue));

  QueryNode* pDupSymExpr = pSymExpr->pDeepCopy(true);
  pDupSymExpr->vSetName(C_SYMBOLICEXPR_PROP);
  pColParameter->vAppendProperty(pDupSymExpr);

  return pColParameter;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ColumnParameter::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pColumnParameter = _pQueryNode(pNode);

  sBuf.vCatSprint("%%%lld"_Stz, iParameter(pColumnParameter));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ColumnParameter::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnParameter::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT ColumnParameter::eType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);
  UNUSED(pRootNode);

  // This should never be called because a column parameter knows nothing of
  // the expression nodes it is representing. Instead, the symbolic expression
  // should be queried for its type, scale, precision, etc.
  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the 1-based reference to a column                                  */
/*----------------------------------------------------------------------------*/
i8 ColumnParameter::iParameter(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  return pNode->iValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the internal columnExpr node that is represented by this parameter */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnParameter::pColumnExpr(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pColExprNode = _pQueryNode(GraphNav::pFindProperty(C_SYMBOLICEXPR_PROP, pNode));
  ASSERT(pColExprNode);
  ASSERT(Expr::bIsOfType(pColExprNode));

  return pColExprNode;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
