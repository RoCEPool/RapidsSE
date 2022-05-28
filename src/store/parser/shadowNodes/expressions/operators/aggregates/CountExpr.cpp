/*============================================================================*/
/* CountExpr.hpp - an COUNT() aggregate expression                            */
/*============================================================================*/

#include "BASE.h"
#include "CountExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "NodeNamer.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COUNT_EXPR_NAME_PREFIX = "CountExpr"_Stz;
static Stz* C_COUNT_EXPR_DEFN = "/__root__/definitions/object/expr/operator/aggregate/count"_Stz;
static Stz* C_ISCOUNTALL_PROP_NAME = "isCountAll"_Stz;
static Stz* C_ISCOUNTALL_DEFN = "/__root__/definitions/object/valueObject/booleanObject"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression nodes.                                */
/*----------------------------------------------------------------------------*/
QueryNode* CountExpr::pCreate(b1 bIsCountAll, b1 bIsDistinct, QueryNode* pChildExpr)
{
  _static_

  // Create the properties
  Value xCountAllVal(bIsCountAll);
  QueryNode* pCountAll = new QueryNode(C_ISCOUNTALL_PROP_NAME,
                                       C_ISCOUNTALL_DEFN,
                                       static_cast<Value&&>(xCountAllVal));

  Value xDistinctVal(bIsDistinct);
  QueryNode* pDistinct = new QueryNode(const_cast<Stz*>(C_DISTINCT_PROP_NAME),
                                       const_cast<Stz*>(C_DISTINCT_PROP_TYPE),
                                       static_cast<Value&&>(xDistinctVal));

  PlanNodeList* pProperties = new PlanNodeList;
  pProperties->vAppend(pCountAll);
  pProperties->vAppend(pDistinct);

  // and the children
  PlanNodeList* pChildren = new PlanNodeList;
  if (pChildExpr) {
    pChildren->vAppend(pChildExpr);
  }

  // Finally, create this object
  s256 sName;
  NodeNamer::vGenerateName(C_COUNT_EXPR_DEFN, sName);

  Value xVal;
  QueryNode* pCountExpr = new QueryNode(sName, C_COUNT_EXPR_DEFN, static_cast<Value&&>(xVal), pProperties, pChildren);
  return pCountExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void CountExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsCountAll(pNode)) {
    sBuf.vConcat("COUNT(*)"_Stz);

  } else {
    sBuf.vConcat("COUNT("_Stz);

    b1 bDistinct = bIsDistinct(pNode);
    if (bDistinct) {
      sBuf.vConcat("DISTINCT("_Stz);
    }

    ASSERT(pNode->nChildren() == 1);
    PlanNode* pChildExpr = pNode->pFirstChild();
    Expr::vDescribe(pChildExpr, sBuf, pRootNode);

    if (bDistinct) {
      sBuf.vConcat(")"_Stz);
    }

    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if this is a COUNT(*) aggregate                               */
/*----------------------------------------------------------------------------*/
b1 CountExpr::bIsCountAll(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  PlanNode* pCountAllNode = GraphNav::pFindProperty(C_ISCOUNTALL_PROP_NAME, pNode);
  ASSERT(pCountAllNode);

  return pCountAllNode->bValue();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 CountExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COUNT_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 CountExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_COUNT_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId CountExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  // Reference:
  // SQL standard 2008 (draft): Part 2 (Foundation), page 537, section 10.9, point (6)
  return TypeId::xIdOf(CT_INTEGER);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
