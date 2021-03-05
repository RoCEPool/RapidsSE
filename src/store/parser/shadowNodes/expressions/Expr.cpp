/*============================================================================*/
/* Expr.cpp - a generic expression                                            */
/*============================================================================*/

#include "BASE.h"
#include "Expr.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "store/parser/shadowNodes/expressions/literals/LiteralExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/AggregateExpr.hpp"
#include "ColumnRef.hpp"
#include "OperatorExpr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_EXPR_DEFN = "/__root__/definitions/object/expr"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Expr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual expression polymorphism here

  if      ( LiteralExpr::bIsOfType(pNode))  {  LiteralExpr::vDescribe(pNode, sBuf); }
  else if (   ColumnRef::bIsOfType(pNode))  {    ColumnRef::vDescribe(pNode, sBuf); }
  else if (OperatorExpr::bIsOfType(pNode))  { OperatorExpr::vDescribe(pNode, sBuf); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT Expr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      ( LiteralExpr::bIsOfType(pExprNode))  { return  LiteralExpr::eType(pExprNode, pRootNode); }
  else if (   ColumnRef::bIsOfType(pExprNode))  { return    ColumnRef::eType(pExprNode, pRootNode); }
  else if (OperatorExpr::bIsOfType(pExprNode))  { return OperatorExpr::eType(pExprNode, pRootNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if this expression (and therefore any subexpression elements  */
/* it contains) is an aggregate expression                                    */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsAnAggregate(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  b1 bIsAgg = false;

  // Walk the graph, looking at the children only and return if any of them
  // are aggregate expressions.
  auto fVisitor = [&bIsAgg](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {
    if (AggregateExpr::bIsOfType(pNode)) {
      bIsAgg = true;
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bWalk(pNode, false, true, fVisitor, nullptr);

  return bIsAgg;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
