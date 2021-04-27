/*============================================================================*/
/* OrderExpr.hpp - a expression and direction for ordering rows               */
/*============================================================================*/

#include "BASE.h"
#include "OrderExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "Expr.hpp"
#include "BooleanObject.hpp"
#include "NodeNamer.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ORDEREXPR_DEFN   = "/__root__/definitions/object/compound/orderExpr"_Stz;
static Stz* C_ORDEREXPR_NAME   = "OrderExpr"_Stz;

static Stz* C_EXPR_PROP  = "expr"_Stz;
static Stz* C_EXPR_DEFN  = "/__root__/definitions/object/expr"_Stz;

static Stz* C_DIRECTION_PROP  = "isAscending"_Stz;
static Stz* C_DIRECTION_DEFN  = "/__root__/definitions/object/valueObject/booleanObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create OrderExpr nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* OrderExpr::pCreate(QueryNode* pExprNode, b1 bAscendingDirection)
{
  _static_

  ASSERT(pExprNode);
  ASSERT(Expr::bIsOfType(pExprNode));

  // Create the direction property
  QueryNode* pDirectionNode = BooleanObject::pCreate(bAscendingDirection);
  pDirectionNode->vSetName(C_DIRECTION_PROP);

  // Rename the expr property
  pExprNode->vSetName(C_EXPR_PROP);

  // Create the OrderExpr object
  s256 sName;
  NodeNamer::vGenerateName(C_ORDEREXPR_DEFN, sName);
  QueryNode* pOrderExpr = new QueryNode(sName.szStz(), C_ORDEREXPR_DEFN);

  // Add the properties to the orderExpr node
  pOrderExpr->vAppendProperty(pExprNode);
  pOrderExpr->vAppendProperty(pDirectionNode);

  return pOrderExpr;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the catalog reference into a buffer               */
/*----------------------------------------------------------------------------*/
void OrderExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pOrderExpr = _pQueryNode(pNode);

  QueryNode* pExprNode = _pQueryNode(GraphNav::pFindProperty(C_EXPR_PROP, pOrderExpr));
  QueryNode* pDirectionNode = _pQueryNode(GraphNav::pFindProperty(C_DIRECTION_PROP, pOrderExpr));

  ASSERT(pExprNode);
  ASSERT(Expr::bIsOfType(pExprNode));
  ASSERT(pDirectionNode);
  ASSERT(BooleanObject::bIsOfType(pDirectionNode));

  s2K sExprDesc;
  Expr::vDescribe(pExprNode, sExprDesc);

  sBuf.vCatSprint("(%s) %s"_Stz, sExprDesc.szStz(), pDirectionNode->bValue() ? "ASC" : "DESC");
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the OrderExpr definition    */
/*----------------------------------------------------------------------------*/
b1 OrderExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ORDEREXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OrderExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ORDEREXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the node that is an expression to order by                         */
/*----------------------------------------------------------------------------*/
QueryNode* OrderExpr::pExprNode(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pExprNode = _pQueryNode(GraphNav::pFindProperty(C_EXPR_PROP, pNode));

  ASSERT(pExprNode);
  ASSERT(Expr::bIsOfType(pExprNode));

  return pExprNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the ordering should be ascending or false for descending   */
/*----------------------------------------------------------------------------*/
b1 OrderExpr::bIsAscending(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pDirectionNode = _pQueryNode(GraphNav::pFindProperty(C_DIRECTION_PROP, pNode));

  ASSERT(pDirectionNode);
  ASSERT(BooleanObject::bIsOfType(pDirectionNode));

  return pDirectionNode->bValue();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
