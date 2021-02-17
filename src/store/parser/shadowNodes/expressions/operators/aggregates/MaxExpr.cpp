/*============================================================================*/
/* MaxExpr.hpp - an MAX() aggregate expression                                */
/*============================================================================*/

#include "BASE.h"
#include "MaxExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_MAX_EXPR_NAME_PREFIX = "MaxExpr"_Stz;
static Stz* C_MAX_EXPR_DEFN = "/__root__/definitions/object/expr/operator/aggregate/max"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression nodes.                                */
/*----------------------------------------------------------------------------*/
QueryNode* MaxExpr::pCreate(b1 bIsDistinct, QueryNode* pChildExpr)
{
  _static_

  // Create the properties
  Value xDistinctVal(bIsDistinct);
  QueryNode* pDistinct = new QueryNode(const_cast<Stz*>(C_DISTINCT_PROP_NAME),
                                       const_cast<Stz*>(C_DISTINCT_PROP_TYPE),
                                       static_cast<Value&&>(xDistinctVal));

  PlanNodeList* pProperties = new PlanNodeList;
  pProperties->vAppend(pDistinct);

  // and the children
  PlanNodeList* pChildren = new PlanNodeList;
  if (pChildExpr) {
    pChildren->vAppend(pChildExpr);
  }

  // Finally, create this object
  s256 sName;
  ShadowNode::vObjName(C_MAX_EXPR_NAME_PREFIX, sName);

  Value xVal;
  QueryNode* pMaxExpr = new QueryNode(sName, C_MAX_EXPR_DEFN, static_cast<Value&&>(xVal), pProperties, pChildren);
  return pMaxExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void MaxExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("MAX("_Stz);

  b1 bDistinct = bIsDistinct(pNode);
  if (bDistinct) {
    sBuf.vConcat("DISTINCT("_Stz);
  }

  ASSERT(pNode->nChildren() == 1);
  PlanNode* pChildExpr = pNode->pFirstChild();
  Expr::vDescribe(pChildExpr, sBuf);

  if (bDistinct) {
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 MaxExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_MAX_EXPR_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
