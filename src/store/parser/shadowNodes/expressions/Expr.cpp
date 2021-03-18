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
#include "ColumnWildcard.hpp"
#include "ColumnOrdinal.hpp"
#include "OperatorExpr.hpp"
#include "ProjectedExpr.hpp"

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

  if      (   LiteralExpr::bIsOfType(pNode))  {    LiteralExpr::vDescribe(pNode, sBuf); }
  else if (     ColumnRef::bIsOfType(pNode))  {      ColumnRef::vDescribe(pNode, sBuf); }
  else if (ColumnWildcard::bIsOfType(pNode))  { ColumnWildcard::vDescribe(pNode, sBuf); }
  else if ( ColumnOrdinal::bIsOfType(pNode))  {  ColumnOrdinal::vDescribe(pNode, sBuf); }
  else if (  OperatorExpr::bIsOfType(pNode))  {   OperatorExpr::vDescribe(pNode, sBuf); }

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

  if      (    LiteralExpr::bIsOfType(pExprNode))  { return    LiteralExpr::eType(pExprNode, pRootNode); }
  else if (      ColumnRef::bIsOfType(pExprNode))  { return      ColumnRef::eType(pExprNode, pRootNode); }
  else if ( ColumnWildcard::bIsOfType(pExprNode))  { return ColumnWildcard::eType(pExprNode, pRootNode); }
  else if (   OperatorExpr::bIsOfType(pExprNode))  { return   OperatorExpr::eType(pExprNode, pRootNode); }
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

  SemanticException::vThrow(EC_EXPR_INVALID_COMBINATION,
                            ": "_Stz,
                            sBuf.szStz() );
}

/*----------------------------------------------------------------------------*/
/* Takes a symbolic expression and returns a copy of it that is relative to   */
/* column ordinals for a given list of column expressions.                    */
/* The returned QueryNode must be freed. Column ordinals are 1-based.         */
/*----------------------------------------------------------------------------*/
QueryNode* Expr::pSymbolicToOrdinal(QueryNode*    pSymbolicExpr,
                                    PlanNodeList* pProjectedExprList,
                                    PlanNode*     pRootNode)
{
  ASSERT(bIsOfType(pSymbolicExpr));

  // e.g. given the symbolic expression SUM(COUNT(DISTINCT colA) + 1)
  // and the fact that COUNT(DISTINCT colA) is already being returned
  // by a child operator as the 3rd column, then this function would
  // return the ordinal expression: SUM($3 + 1)

  // Strategy:
  // Try to match the entire expression to a column in the columnExprList.
  // If this fails, then strip off the outermost expression and try again
  // with each child.
  // Finally, duplicate any parent expressions to rebuild the expression
  // tree using these ordinals.

  QueryNode* pOrdinalExpr;

  // First we need to check if any of the columns in the columnExprList
  // exactly match our symbolic expression.
  PlanNodeIter* pIter = pProjectedExprList->pNewIter();
  i8 iOrdinal = 1;

  while (pIter->bHasNext()) {
    QueryNode* pProjectedExpr = static_cast<QueryNode*>(pIter->pNext());
    QueryNode* pColExpr = ProjectedExpr::pExprNode(pProjectedExpr);

    s2K sBuf1;
    s2K sBuf2;
    Expr::vDescribe(pSymbolicExpr, sBuf1);
    Expr::vDescribe(pColExpr, sBuf2);
    vTrace("bIsSymbolicallyEqual:\n  %s\n  %s\n\n"_Stz, sBuf1.szStz(), sBuf2.szStz());

    if (Expr::bIsSymbolicallyEqual(pSymbolicExpr, pColExpr)) {
      // Create an ordinal expression that represents this entire expression
      // (sub-) tree and return it
      pOrdinalExpr = ColumnOrdinal::pCreate(iOrdinal);
      delete pIter;
      return pOrdinalExpr;
    }

    ++iOrdinal;
  }

  delete pIter;

  // There were no columns that completely matched the symbolic expression.
  // Therefore we need to strip a layer off the expression and try again
  // by recursively calling ourselves. We do this by iterating the children
  // of this symbolic expression.

  if (pSymbolicExpr->nChildren() == 0) {
    // Duplicate this expression and return it
    pOrdinalExpr = pSymbolicExpr->pDeepCopy();
  }

  // This symbolic expr has children. We will need to try to convert all children
  // to ordinal expressions and then duplicate this current expression. So
  // create a list for the converted children.

  PlanNodeList* pOrdinalChildren = new PlanNodeList();

  // Iterate the children of the symbolic expression
  pIter = pSymbolicExpr->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChildSymbolicExpr = static_cast<QueryNode*>(pIter->pNext());

    // Convert the child from symbolic to ordinal. If not, this will
    // duplicate the child expression anyway. Add it to a list of such children.
    QueryNode* pChildOrdinalExpr  = pSymbolicToOrdinal(pChildSymbolicExpr, pProjectedExprList, pRootNode);
    pOrdinalChildren->vAppend(pChildOrdinalExpr);
  }
  delete pIter;

  // Now we can deep copy the current symbolic expression but give it new children
  pOrdinalExpr = pSymbolicExpr->pDeepCopy(pOrdinalChildren);

  return pOrdinalExpr;
}

/*----------------------------------------------------------------------------*/
/* Compare two expression trees and return true if they are equal with        */
/* respect to the operations and symbols used.                                */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsSymbolicallyEqual(QueryNode* pExpr1, QueryNode* pExpr2)
{
  if (pExpr1 == pExpr2) return true;

  // Is one of them not null and the other null?
  if (pExpr1 && !pExpr2) return false;
  if (!pExpr1 && pExpr2) return false;

  // We don't compare names as they are allowed to be different
  // (except in the case of properties, but see below).

  // Types must match
  if (!pExpr1->szType()->bEqual(pExpr2->szType())) return false;

  // Special case: the property values for ColumnRef nodes need to
  // be compared in a case insensitive way.
  if (ColumnRef::bIsOfType(pExpr1)) {
    return ColumnRef::bIsSymbolicallyEqual(pExpr1, pExpr2);
  }

  // Values must match. Note that this does a case sensitive comparison.
  if (pExpr1->xValue() != pExpr2->xValue()) return false;

  // All properties must match. Ideally they shouldn't need to be in
  // the same order, but it is simpler this way for now.
  if (pExpr1->nProperties() != pExpr2->nProperties()) return false;
  if (pExpr1->nProperties() > 0) {
    PlanNodeIter* pIter1 = pExpr1->pNewPropertyIter();
    PlanNodeIter* pIter2 = pExpr2->pNewPropertyIter();

    while (pIter1->bHasNext()) {
      QueryNode* pProp1 = static_cast<QueryNode*>(pIter1->pNext());
      QueryNode* pProp2 = static_cast<QueryNode*>(pIter2->pNext());

      // Compare the properties
      if (!bIsSymbolicallyEqual(pProp1, pProp2)) return false;

      // Both properties should also have the same name
      if (!pProp1->szName()->bEqual(pProp2->szName())) return false;
    }

    delete pIter1;
    delete pIter2;
  }

  // All children must match and must be in the same order.
  // TODO: There are a number of operations where the order of children is
  //  not significant to the outcome. e.g. (1 + col2) == (col2 + 1)
  if (pExpr1->nChildren() != pExpr2->nChildren()) return false;
  if (pExpr1->nChildren() > 0) {
    PlanNodeIter* pIter1 = pExpr1->pNewChildrenIter();
    PlanNodeIter* pIter2 = pExpr2->pNewChildrenIter();

    while (pIter1->bHasNext()) {
      QueryNode* pChild1 = static_cast<QueryNode*>(pIter1->pNext());
      QueryNode* pChild2 = static_cast<QueryNode*>(pIter2->pNext());

      // Compare the children
      if (!bIsSymbolicallyEqual(pChild1, pChild2)) return false;
    }

    delete pIter1;
    delete pIter2;
  }

  // The two expressions must be symbolically equal
  return true;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the expression does not contain any column references.     */
/* All expressions are relative column ordinals, operators or literals only.  */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsOrdinal(QueryNode* pExpr)
{
  b1 bIsOrdinal = true;

  // Walk the expression graph (children only), looking for ColumnRef.
  // Lambda to execute on each node:
  auto fVisitor = [&bIsOrdinal](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    if (ColumnRef::bIsOfType(pNode)) {
      bIsOrdinal = false;
      return GraphNav::WALK_ABORT;
    }

    // Column wildcards should have been replaced by now
    ASSERT(!ColumnWildcard::bIsOfType(pNode));

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bWalk(pExpr, fVisitor, nullptr);

  return bIsOrdinal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
