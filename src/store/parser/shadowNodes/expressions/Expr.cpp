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
#include "SqlException.hpp"

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


/*----------------------------------------------------------------------------*/
/* Checks the expression tree to determine that a supported result type       */
/* can be produced. Will throw a SqlException if it is an invalid expression. */
/*----------------------------------------------------------------------------*/
void Expr::vValidateResultType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  ASSERT(pExprNode);
  ASSERT(bIsOfType(pExprNode));

  // We check for an invalid result type by calling eType() and looking for
  // CT_NONE. If an expression produces this result type then we try to
  // decompose the expression to find the root cause. We can do this without
  // manual polymorphism because we know that expressions are nested in
  // parent-child trees, so we can just ask each child of this node for its
  // result type to help narrow it down.

  // First check the overall result. If it is not CT_NONE then the entire
  // expression tree should be ok (because no combination of CT_NONE should
  // produce a valid type).

  eCT eResultType = eType(pExprNode, pRootNode);

  if (eResultType != CT_NONE) {
    // This expression will produce a valid type
    // (though this doesn't mean it will produce the correct type for it's location
    // in the plan)
    ASSERT(eResultType > CT_NONE);
    ASSERT(eResultType < CT_ENUMS);
    return;
  }

  // Check with each of the children to see we can narrow the problem down further.
  if (pExprNode->nChildren() > 0) {
    PlanNodeIter* pIter = pExprNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pChildExpr = static_cast<QueryNode*>(pIter->pNext());

      try {
      // Recurse into this child.
        vValidateResultType(pChildExpr, pRootNode);

      } catch (SqlException& e) {
        delete pIter;
        throw;
      }
    }

    delete pIter;
  }

  // The problem is in this expression node. Throw an exception
  s2K sBuf;
  Expr::vDescribe(pExprNode, sBuf);

  SemanticException::vThrow(
      "Expression type combination is invalid or unsupported for the expression operator : %s"_Stz,
      sBuf.szStz()
      );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
