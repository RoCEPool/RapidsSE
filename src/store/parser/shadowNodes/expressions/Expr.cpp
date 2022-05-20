/*============================================================================*/
/* Expr.cpp - a generic expression                                            */
/*============================================================================*/

#include "BASE.h"
#include "Expr.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

#include "store/parser/shadowNodes/expressions/literals/LiteralExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/AggregateExpr.hpp"
#include "ColumnRef.hpp"
#include "AliasRef.hpp"
#include "ColumnWildcard.hpp"
#include "ColumnOrdinal.hpp"
#include "ColumnParameter.hpp"
#include "OperatorExpr.hpp"
#include "ProjectedColumn.hpp"
#include "AdditionExpr.hpp"
#include "MultiplicationExpr.hpp"
#include "AndExpr.hpp"
#include "OrExpr.hpp"
#include "XorExpr.hpp"
#include "EqExpr.hpp"
#include "NeExpr.hpp"
#include "LtExpr.hpp"
#include "GtExpr.hpp"
#include "LeExpr.hpp"
#include "GeExpr.hpp"
#include "AvgExpr.hpp"
#include "SumExpr.hpp"
#include "CountExpr.hpp"
#include "MinExpr.hpp"
#include "MaxExpr.hpp"
#include "DivisionExpr.hpp"
#include "SubqueryRefExpr.hpp"
#include "MiscExpr.hpp"
#include "CastExpr.hpp"
#include "CaseExpr.hpp"
#include "WhenExpr.hpp"
#include "SubstringExpr.hpp"
#include "ShadowRoot.hpp"

#include "NodeNamer.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "SqlException.hpp"
#include "TypeSystem.hpp"


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
void Expr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual expression polymorphism here

  if      (    LiteralExpr::bIsOfType(pNode))  {     LiteralExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (      ColumnRef::bIsOfType(pNode))  {       ColumnRef::vDescribe(pNode, sBuf, pRootNode); }
  else if (       AliasRef::bIsOfType(pNode))  {        AliasRef::vDescribe(pNode, sBuf, pRootNode); }
  else if ( ColumnWildcard::bIsOfType(pNode))  {  ColumnWildcard::vDescribe(pNode, sBuf, pRootNode); }
  else if (  ColumnOrdinal::bIsOfType(pNode))  {   ColumnOrdinal::vDescribe(pNode, sBuf, pRootNode); }
  else if (ColumnParameter::bIsOfType(pNode))  { ColumnParameter::vDescribe(pNode, sBuf, pRootNode); }
  else if (   OperatorExpr::bIsOfType(pNode))  {    OperatorExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (SubqueryRefExpr::bIsOfType(pNode))  { SubqueryRefExpr::vDescribe(pNode, sBuf, pRootNode); }
  else if (       MiscExpr::bIsOfType(pNode))  {        MiscExpr::vDescribe(pNode, sBuf, pRootNode); }

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
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId Expr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // TODO: manual expression polymorphism here

  if      (    LiteralExpr::bIsOfType(pExprNode))  { return LiteralExpr::xTypeId(pExprNode, pRootNode); }
  else if (      ColumnRef::bIsOfType(pExprNode))  { return ColumnRef::xTypeId(pExprNode, pRootNode); }
  else if (       AliasRef::bIsOfType(pExprNode))  { return AliasRef::xTypeId(pExprNode, pRootNode); }
  else if ( ColumnWildcard::bIsOfType(pExprNode))  { return ColumnWildcard::xTypeId(pExprNode, pRootNode); }
  else if (  ColumnOrdinal::bIsOfType(pExprNode))  { return ColumnOrdinal::xTypeId(pExprNode, pRootNode); }
  else if (ColumnParameter::bIsOfType(pExprNode))  { return ColumnParameter::xTypeId(pExprNode, pRootNode); }
  else if (   OperatorExpr::bIsOfType(pExprNode))  { return OperatorExpr::xTypeId(pExprNode, pRootNode); }
  else if (SubqueryRefExpr::bIsOfType(pExprNode))  { return SubqueryRefExpr::xTypeId(pExprNode, pRootNode); }
  else if (       MiscExpr::bIsOfType(pExprNode))  { return MiscExpr::xTypeId(pExprNode, pRootNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if this expression (and therefore any subexpression elements  */
/* it contains) contains an aggregate expression. It may also contain non-    */
/* aggregated terms too.                                                      */
/*----------------------------------------------------------------------------*/
b1 Expr::bContainsAnAggregate(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  b1 bIsAgg = false;

  // Walk the graph, looking at the children only and return if any of them
  // are aggregate expressions.
  auto fVisitor = [&bIsAgg](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {
    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pExpr = _pQueryNode(pNode);

    if (AliasRef::bIsOfType(pExpr)) {
      pExpr = AliasRef::pAliasedExpr(_pQueryNode(pExpr));
    }

    if (AggregateExpr::bIsOfType(pExpr)) {
      bIsAgg = true;
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bBoundedWalk(pNode, true, true, fVisitor, nullptr);

  return bIsAgg;
}

/*----------------------------------------------------------------------------*/
/* Returns true if this expression (and any subexpression elements it contains)*/
/* has a ColumnRef that is not part of an aggregation. If true, this is not to */
/* say that this expression tree doesn't contain aggregations, it could, but  */
/* it may also contain non-aggregated ColumnRefs too. e.g. the expression     */
/* "col1 + SUM(col2)" contains both an aggregate term and a non-aggregated    */
/* ColumnRef.                                                                 */
/*----------------------------------------------------------------------------*/
b1 Expr::bContainsNonaggregatedColumnRefs(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  b1 bHasNonAggColumnRef = false;

  // Walk the graph, looking at the children only and return if any of them
  // are aggregate expressions.
  auto fVisitor = [&bHasNonAggColumnRef](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pExpr = _pQueryNode(pNode);

    if (AliasRef::bIsOfType(pNode)) {
      pExpr = AliasRef::pAliasedExpr(_pQueryNode(pNode));
    }

    if (AggregateExpr::bIsOfType(pExpr)) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    if (ColumnRef::bIsOfType(pExpr)) {
      bHasNonAggColumnRef = true;
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bBoundedWalk(pNode, true, true, fVisitor, nullptr);

  return bHasNonAggColumnRef;
}

/*----------------------------------------------------------------------------*/
/* Returns true if this expression tree only comprises literals and operators.*/
/* i.e., returns false if a ColumnRef or ColumnOrdinal is encountered.        */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsFullyLiteral(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  b1 bIsLit = true;

  // Walk the graph, looking at the children only and return if any of them
  // are ColumnRefs or ColumnOrdinals expressions.
  auto fVisitor = [&bIsLit](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pExpr = _pQueryNode(pNode);

    if (AliasRef::bIsOfType(pNode)) {
      pExpr = AliasRef::pAliasedExpr(_pQueryNode(pNode));
    }

    if (      ColumnRef::bIsOfType(pExpr) ||
          ColumnOrdinal::bIsOfType(pExpr) ||
        ColumnParameter::bIsOfType(pExpr)) {
      bIsLit = false;
      return GraphNav::WALK_ABORT;
    }

    // Should not encounter column wildcards
    ASSERT(!ColumnWildcard::bIsOfType(pNode));

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bBoundedWalk(pNode, true, true, fVisitor, nullptr);

  return bIsLit;
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

  TypeId xResultTypeId = xTypeId(pExprNode, pRootNode);
  eCT eResultType = xResultTypeId.eCTVal();

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
      QueryNode* pChildExpr = _pQueryNode(pIter->pNext());

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
  Expr::vDescribe(pExprNode, sBuf, pRootNode);

  SemanticException::vThrow(EC_EXPR_INVALID_COMBINATION,
                            ": %s"_Stz,
                            sBuf.szStz() );
}

/*----------------------------------------------------------------------------*/
/* Takes a symbolic expression and returns a copy of it that is relative to   */
/* column ordinals for a given list of column expressions.                    */
/* The returned QueryNode must be freed. Column ordinals are 1-based.         */
/*----------------------------------------------------------------------------*/
QueryNode* Expr::pSymbolicToOrdinal(QueryNode*     pSymbolicExpr,
                                    QueryNodeList* pColumnExprList,
                                    QueryNode*     pCorrelatedExprs,
                                    PlanNode*      pRootNode)
{
  ASSERT(bIsOfType(pSymbolicExpr));

  // e.g. given the symbolic expression SUM(COUNT(DISTINCT colA) + 1)
  // and the fact that COUNT(DISTINCT colA) is already being projected
  // by a child operator as the 3rd column, then this function should
  // return the ordinal expression: SUM($3 + 1)

  // Strategy:
  // Try to match the entire expression to a column in the columnExprList
  // or a correlated expression in pCorrelatedExprs.
  // If this fails, then strip off the outermost expression and try again
  // with each child.
  // Finally, duplicate any parent expressions to rebuild the expression
  // tree using these ordinals.

  QueryNode* pOrdinalExpr;

  // First we need to check if any of the columns in the columnExprList
  // exactly match our symbolic expression.
  if (pColumnExprList) {
    QueryNodeIter* pIter = pColumnExprList->pNewIter();
    i8 iOrdinal = 1;

    while (pIter->bHasNext()) {
      QueryNode* pProjectedColumn = pIter->pNext();
      ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

      QueryNode* pColExpr = ProjectedColumn::pExprNode(pProjectedColumn);
      ASSERT(Expr::bIsOfType(pColExpr));

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
  }

  // Check if any of the correlated expressions in pCorrelatedExprs
  // exactly match our symbolic expression.
  if (pCorrelatedExprs->nChildren() > 0) {
    PlanNodeIter* pCorrIter = pCorrelatedExprs->pNewChildrenIter();
    i8 iCorrParam = 1;

    while (pCorrIter->bHasNext()) {
      QueryNode* pCorrExpr = _pQueryNode(pCorrIter->pNext());
      ASSERT(Expr::bIsOfType(pCorrExpr));

      if (Expr::bIsSymbolicallyEqual(pSymbolicExpr, pCorrExpr)) {
        // Create an ordinal expression that represents this entire expression
        // (sub-) tree and return it
        pOrdinalExpr = ColumnParameter::pCreate(iCorrParam, pSymbolicExpr);
        delete pCorrIter;
        return pOrdinalExpr;
      }

      ++iCorrParam;
    }

    delete pCorrIter;
  }

  // There were no columns that completely matched the symbolic expression.
  // Therefore we need to strip a layer off the expression and try again
  // by recursively calling ourselves. We do this by iterating the children
  // of this symbolic expression.

  if (pSymbolicExpr->nChildren() == 0) {

    // Special handling for SubqueryRefExprs
    if (SubqueryRefExpr::bIsOfType(pSymbolicExpr)) {
      pOrdinalExpr = SubqueryRefExpr::pSymbolicToOrdinal(pSymbolicExpr, pColumnExprList, pCorrelatedExprs, pRootNode);
      return pOrdinalExpr;
    }

    // Duplicate this expression and return it
    pOrdinalExpr = pSymbolicExpr->pDeepCopy(true);
    return pOrdinalExpr;
  }

  // This symbolic expr has children. We will need to try to convert all children
  // to ordinal expressions and then duplicate this current expression. So
  // create a list for the converted children.

  PlanNodeList* pOrdinalChildren = new PlanNodeList();

  // Iterate the children of the symbolic expression
  PlanNodeIter* pChildIter = pSymbolicExpr->pNewChildrenIter();

  while (pChildIter->bHasNext()) {
    QueryNode* pChildSymbolicExpr = _pQueryNode(pChildIter->pNext());

    // Convert the child from symbolic to ordinal. If not, this will
    // duplicate the child expression anyway. Add it to a list of such children.
    QueryNode* pChildOrdinalExpr  = pSymbolicToOrdinal(pChildSymbolicExpr, pColumnExprList, pCorrelatedExprs, pRootNode);
    pOrdinalChildren->vAppend(pChildOrdinalExpr);
  }
  delete pChildIter;

  // Now we can deep copy the current symbolic expression but give it new children
  pOrdinalExpr = pSymbolicExpr->pDeepCopy(true, pOrdinalChildren);

  return pOrdinalExpr;
}

/*----------------------------------------------------------------------------*/
/* Takes an ordinal expression and returns a symbolic copy of it based on the */
/* column expressions projected from a child sql operator.                    */
/* The returned QueryNode must be freed.                                      */
/*----------------------------------------------------------------------------*/
QueryNode* Expr::pOrdinalToSymbolic(QueryNode*     pOrdinalExpr,
                                    QueryNodeList* pColumnExprList,
                                    PlanNode*      pRootNode)
{
  UNUSED(pRootNode);

  // First let's deep copy the ordinal expression
  QueryNode* pSymExpr = pOrdinalExpr->pDeepCopy(true);

  // Now we can walk the copied expression and replace any ColumnOrdinals,
  // ColumnParameters and SubqueryRefExprs with their symbolic equivalent.
  auto fVisitor = [pColumnExprList, pRootNode](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode *pExpr = _pQueryNode(pNode);
    ASSERT(Expr::bIsOfType(pExpr));

    if (ColumnOrdinal::bIsOfType(pExpr)) {

      // Which ordinal is this?
      i8 iOrdinal = ColumnOrdinal::iOrdinal(pExpr);
      ASSERT(iOrdinal > 0);
      ASSERT(iOrdinal - 1 < pColumnExprList->nItems());

      // Get the projected expression that represents this ordinal
      QueryNode* pProjectedCol = pColumnExprList->pPeek(iOrdinal - 1);
      ASSERT(ProjectedColumn::bIsOfType(pProjectedCol));

      QueryNode* pProjectedExpr = ProjectedColumn::pExprNode(pProjectedCol);

      // Modify the node in place to represent this projected expression.
      // By modifying it in place we can keep walking the graph because we are not
      // modifying the current node's parent.
      pExpr->vTransmogrifyTo(pProjectedExpr);

      s256 sName;
      NodeNamer::vGenerateName(pExpr, sName);
      pExpr->vSetName(sName.szStz());

      return GraphNav::WALK_CONTINUE;

    } else if (ColumnParameter::bIsOfType(pExpr)) {

      // Extract the expression that represents this column parameter
      QueryNode* pColExpr = ColumnParameter::pColumnExpr(pExpr);
      QueryNode* pDupColExpr = pColExpr->pDeepCopy(true);

      pExpr->vTransmogrifyTo(pDupColExpr, true);
      delete pDupColExpr;
      return GraphNav::WALK_CONTINUE;

    } else if (SubqueryRefExpr::bIsOfType(pExpr)) {
      QueryNode* pSymbolicExpr = SubqueryRefExpr::pOrdinalToSymbolic(pExpr, pColumnExprList, pRootNode);
      pExpr->vTransmogrifyTo(pSymbolicExpr, true);
      delete pSymbolicExpr;
      return GraphNav::WALK_CONTINUE;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pSymExpr, false, true, fVisitor, nullptr);
  return pSymExpr;
}

/*----------------------------------------------------------------------------*/
/* Compare two expression trees and return true if they are equal with        */
/* respect to the operations and symbols used.                                */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsSymbolicallyEqual(QueryNode* pExpr1, QueryNode* pExpr2)
{
  // We cannot assert that pExpr1 and pExpr2 are expressions here because
  // lower down we actually recursively call this function to compare any
  // properties of the expression (e.g. the isDistinct property of an aggregation expr).

  // Are we comparing the same actual objects?
  if (pExpr1 == pExpr2) return true;

  // Is one of them not null and the other null?
  if (pExpr1 && !pExpr2) return false;
  if (!pExpr1 && pExpr2) return false;

  // We don't compare names as they are allowed to be different
  // (except in the case of properties, but see below).

  // The type of the expressions must match
  if (!pExpr1->szType()->bEqual(pExpr2->szType())) {
    // If the types are not equal, then are the at least comparable?

    // Is one or both of them an alias?
    if (AliasRef::bIsOfType(pExpr1)) {
      // pExpr1 is an alias but we know pExpr2 is not because they have different types.
      // Get the aliased expression from pExpr1 and compare this aliased expression to pExpr2.
      QueryNode* pAliasedExpr1 = AliasRef::pAliasedExpr(pExpr1);
      return bIsSymbolicallyEqual(pAliasedExpr1, pExpr2);
    }

    if (AliasRef::bIsOfType(pExpr2)) {
      // pExpr2 is an alias but we know pExpr1 is not because they have different types.
      // Get the aliased expression from pExpr2 and compare this aliased expression to pExpr1.
      QueryNode* pAliasedExpr2 = AliasRef::pAliasedExpr(pExpr2);
      return bIsSymbolicallyEqual(pExpr1, pAliasedExpr2);
    }

    // Are the mathematically comparable?
    //  e.g. A<B is symbolically equal to B>A
    return bIsMathematicallyEqual(pExpr1, pExpr2);
  }

  // Special case: the property values for ColumnRef and AliasRef
  // nodes need to be compared in a case insensitive way.
  if (ColumnRef::bIsOfType(pExpr1)) {
    return ColumnRef::bIsSymbolicallyEqual(pExpr1, pExpr2);

  } else if (AliasRef::bIsOfType(pExpr1)) {
    return AliasRef::bIsSymbolicallyEqual(pExpr1, pExpr2);
  }

  // Values must match. Note that this does a case sensitive comparison.
  if (pExpr1->xValue() != pExpr2->xValue()) return false;

  // All properties must match. Ideally they shouldn't need to be in
  // the same order, but it is simpler this way for now.
  if (pExpr1->nProperties() != pExpr2->nProperties()) return false;

  if (pExpr1->nProperties() > 0) {
    b1 bPropertiesEqual = true;

    PlanNodeIter* pIter1 = pExpr1->pNewPropertyIter();
    PlanNodeIter* pIter2 = pExpr2->pNewPropertyIter();

    while (pIter1->bHasNext()) {
      QueryNode* pProp1 = _pQueryNode(pIter1->pNext());
      QueryNode* pProp2 = _pQueryNode(pIter2->pNext());

      // Compare the properties
      if (!bIsSymbolicallyEqual(pProp1, pProp2)) {
        bPropertiesEqual = false;
        break;
      }

      // Both properties should also have the same name
      if (!pProp1->szName()->bEqual(pProp2->szName())) {
        bPropertiesEqual = false;
        break;
      }
    }

    delete pIter1;
    delete pIter2;

    if (!bPropertiesEqual) return false;
  }

  // All children must match and must be in the same order.
  // If this fails then we will check for the commutative property.
  if (pExpr1->nChildren() != pExpr2->nChildren()) return false;

  if (pExpr1->nChildren() > 0) {
    b1 bChildrenEqual = true;

    PlanNodeIter* pIter1 = pExpr1->pNewChildrenIter();
    PlanNodeIter* pIter2 = pExpr2->pNewChildrenIter();

    while (pIter1->bHasNext()) {
      QueryNode* pChild1 = _pQueryNode(pIter1->pNext());
      QueryNode* pChild2 = _pQueryNode(pIter2->pNext());

      // Compare the children
      if (!bIsSymbolicallyEqual(pChild1, pChild2)) {
        bChildrenEqual = false;
        break;
      }
    }

    delete pIter1;
    delete pIter2;

    if (!bChildrenEqual) {
      // Children are not equal, but maybe we've got a commutative operator
      // such as the AddOp, where A+B is symbolically the same as B+A.
      // Check that the expression is commutative and check if the expressions
      // are symbolically equal by rearranging the order of their children.
      if (!bIsCommutativelyEqual(pExpr1, pExpr2)) return false;
    }
  }

  // The two expressions must be symbolically equal
  return true;
}

/*----------------------------------------------------------------------------*/
/* Compare two expression trees symbolically and return true if               */
/* pSuperExpr is equal to, or a superset of, pSubExpr.                        */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsEqualOrSupersetOf(QueryNode* pSuperExpr, QueryNode* pSubExpr)
{
  ASSERT(pSuperExpr);
  ASSERT(pSubExpr);
  ASSERT(bIsOfType(pSuperExpr));
  ASSERT(bIsOfType(pSubExpr));

  // Check if the two expression trees are equal
  if (bIsSymbolicallyEqual(pSuperExpr, pSubExpr))   return true;

  // Expressions are not equal at this point but may be equal in a child
  // of pSuperExpr.

  // Walk the children of pSuperExpr and test again
  PlanNodeIter* pIter = pSuperExpr->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pChildExpr = _pQueryNode(pIter->pNext());

    // Recurse this function
    if (bIsEqualOrSupersetOf(pChildExpr, pSubExpr)) {
      // We got a match, no need to keep checking
      delete pIter;
      return true;
    }

    // try the next child
  }

  // Couldn't find a match
  delete pIter;
  return false;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the expression does not contain any column references.     */
/* All expressions are relative column ordinals, operators or literals only.  */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsFullyOrdinal(QueryNode* pExpr)
{
  ASSERT(pExpr);
  ASSERT(bIsOfType(pExpr));

  b1 bIsOrdinal = true;

  // Walk the expression graph (children only), looking for ColumnRef.
  // Lambda to execute on each node:
  auto fVisitor = [&bIsOrdinal](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (ColumnRef::bIsOfType(pNode)) {
      bIsOrdinal = false;
      return GraphNav::WALK_ABORT;
    }

    if (SubqueryRefExpr::bIsOfType(pNode) && !SubqueryRefExpr::bIsFullyOrdinal(_pQueryNode(pNode))) {
      bIsOrdinal = false;
      return GraphNav::WALK_ABORT;
    }

    // Column wildcards should have been replaced by now
    ASSERT(!ColumnWildcard::bIsOfType(pNode));

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bBoundedWalk(pExpr, false, true, fVisitor, nullptr);

  return bIsOrdinal;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the expression does not contain any ordinal or parameter   */
/*  column references. Can contain (symbolic) ColumnRefs and literals.        */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsNotOrdinal(QueryNode* pExpr)
{
  ASSERT(pExpr);
  ASSERT(bIsOfType(pExpr));

  b1 bIsOrdinal = false;

  // Walk the expression graph (children only), looking for OrdinalRef.
  // Lambda to execute on each node:
  auto fVisitor = [&bIsOrdinal](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (ColumnOrdinal::bIsOfType(pNode) || ColumnParameter::bIsOfType(pNode)) {
      bIsOrdinal = true;
      return GraphNav::WALK_ABORT;
    }

    // Column wildcards should have been replaced by now
    ASSERT(!ColumnWildcard::bIsOfType(pNode));

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bBoundedWalk(pExpr, false, true,fVisitor, nullptr);

  return !bIsOrdinal;
}

/*----------------------------------------------------------------------------*/
/* Given an expression already in ordinal form, find and return the largest   */
/* non-ordinal/parameterised subset of that expression. If the expression is  */
/* wholly ordinal then nullptr is returned.                                   */
/*----------------------------------------------------------------------------*/
QueryNode* Expr::pLargestNonOrdinalSubExpr(QueryNode* pOrdinalExpr, b1 bIncludeAggregates)
{
  ASSERT(pOrdinalExpr);
  ASSERT(Expr::bIsOfType(pOrdinalExpr));

  // We want to walk down the expression graph and find the largest subset
  // that contains ColumnRefs but no OrdinalRefs.

  // First define some lambda conditions to search for ColumnRefs and OrdinalRefs
  auto fColumnRefCondition    = [](PlanNode* pNode, void* pvArg) -> b1 { UNUSED(pvArg); return       ColumnRef::bIsOfType(pNode); };
  auto fOrdinalRefCondition   = [](PlanNode* pNode, void* pvArg) -> b1 { UNUSED(pvArg); return   ColumnOrdinal::bIsOfType(pNode); };
  auto fParameterRefCondition = [](PlanNode* pNode, void* pvArg) -> b1 { UNUSED(pvArg); return ColumnParameter::bIsOfType(pNode); };
  auto fHasAggregateCondition = [](PlanNode* pNode, void* pvArg) -> b1 { UNUSED(pvArg); return   AggregateExpr::bIsOfType(pNode); };
  auto fHasSubqueryCondition  = [](PlanNode* pNode, void* pvArg) -> b1 { UNUSED(pvArg); return SubqueryRefExpr::bIsOfType(pNode) && !SubqueryRefExpr::bIsFullyOrdinal(_pQueryNode(pNode)); };

  QueryNode* pNonOrdExpr = nullptr;

  auto fVisitor = [bIncludeAggregates, &pNonOrdExpr, fColumnRefCondition,
                   fOrdinalRefCondition, fParameterRefCondition,
                   fHasAggregateCondition, fHasSubqueryCondition]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!bIncludeAggregates && AggregateExpr::bIsOfType(pNode)) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    PlanNode* pColumnRef    = GraphNav::pGlobalSearch(pNode, true, true, fColumnRefCondition,    nullptr);
    PlanNode* pOrdinalRef   = GraphNav::pGlobalSearch(pNode, true, true, fOrdinalRefCondition,   nullptr);
    PlanNode* pParameterRef = GraphNav::pGlobalSearch(pNode, true, true, fParameterRefCondition, nullptr);
    PlanNode* pAggregateRef = GraphNav::pGlobalSearch(pNode, true, true, fHasAggregateCondition, nullptr);
    PlanNode* pSubqueryRef  = GraphNav::pGlobalSearch(pNode, true, true, fHasSubqueryCondition,  nullptr);

    if ( (pColumnRef || pSubqueryRef) && !pOrdinalRef && !pParameterRef && (bIncludeAggregates || !pAggregateRef) ) {
      pNonOrdExpr = _pQueryNode(pNode);
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pOrdinalExpr, false, true, fVisitor, nullptr);
  return pNonOrdExpr;
}

/*----------------------------------------------------------------------------*/
/* Given an expression, return a list of each aggregate term found in         */
/* that expression. The returned list is to be deleted, but the items         */
/* in the list are not to be deleted.                                         */
/* If bDescendAggregates is true then all nested aggregate terms are returned.*/
/* If false then only the outer aggregate terms are returned.                 */
/*----------------------------------------------------------------------------*/
QueryNodeList* Expr::pAggregateExprs(QueryNode* pExpr, b1 bDescendAggregates)
{
  QueryNodeList* pList = new QueryNodeList();

  auto fVisitor = [pList, bDescendAggregates]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pExprNode = _pQueryNode(pNode);

    if (AliasRef::bIsOfType(pNode)) {
      QueryNode* pAliasedExpr = AliasRef::pAliasedExpr(pExprNode);

      // Since the alias expression is a property of alias, and we are not walking
      // properties, we manually search this space and add it to the outer list of aggregates.
      QueryNodeList* pAliasAggs = pAggregateExprs(pAliasedExpr, bDescendAggregates);

      while (pAliasAggs->nItems() > 0) {
        pList->vPush(pAliasAggs->pPop());
      }

      ASSERT(pAliasAggs->nItems() == 0);
      delete pAliasAggs;

    } else if (AggregateExpr::bIsOfType(pExprNode)) {
      pList->vPush(_pQueryNode(pExprNode));
      
      if (!bDescendAggregates) {
        return GraphNav::WALK_SKIP_SUBTREE;
      }
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pExpr, false, true, fVisitor, nullptr);

  return pList;
}

/*----------------------------------------------------------------------------*/
/* Changes the existing expression in place to support super-aggregation of   */
/* partitioned aggregate expressions.                                         */
/*----------------------------------------------------------------------------*/
void Expr::vSuperAggregate(QueryNode* pExpr, PlanNode* pRootNode)
{
  // e.g. an expression of SUM(col1), when all of the partitioned results are
  // being merged, will be super-aggregated with the expression SUM(SUM(col1)).
  // Super-aggregation occurs as follows:
  //
  // SUM(x)   -->   SUM(SUM(x))
  // COUNT(x) -->   SUM(COUNT(x))
  // MIN(x)   -->   MIN(MIN(x))
  // MAX(x)   -->   MAX(MAX(x))
  //
  // Note that an AVG(x) aggregate will be replaced at the partition level by
  // a SUM(x) and COUNT(x). A super-aggregating expression replaces AVG(x) with
  // SUM(SUM(x))/SUM(COUNT(x))

  QueryNodeList* pAggList = Expr::pAggregateExprs(pExpr, false);
  
  QueryNodeIter* pIter = pAggList->pNewIter();

  while (pIter->bHasNext()) {

    QueryNode* pAgg = pIter->pNext();

    if (AvgExpr::bIsOfType(pAgg)) {
      ASSERT(pAgg->nChildren() == 1);
      QueryNode* pChildExpr = _pQueryNode(pAgg->pFirstChild());

      // Form a template that we will then transmogrify the avgExpr node into.

      // Build the SUM(SUM(x)) chain.
      QueryNode* pNumerator = SumExpr::pCreate(false, SumExpr::pCreate(false, pChildExpr->pDeepCopy(true)));

      // Build the SUM(COUNT(x)) chain
      QueryNode* pDenominator = SumExpr::pCreate(false, CountExpr::pCreate(false, false, pChildExpr->pDeepCopy(true)));

      // Handle type differences here.
      // Both the numerator and denominator may need to be explicitly cast in order to
      // maintain the resultant type according to the AVG expression.
      // The denominator is always an integer here, but we need its type to match that
      // of the numerator (type coercion gives this consistency).
      // The numerator may need to be cast if it is an integer as AVG(x) returns a
      // DECIMAL if x is an INTEGER.
      //
      // First get the resulting type of the original AVG operation.
      TypeId xAvgTypeId = Expr::xTypeId(pExpr, pRootNode);
      eCT eAvgType = xAvgTypeId.eCTVal();

      // Now check if it is the same type as the numerator and denominator
      if (Expr::xTypeId(pNumerator, pRootNode) != xAvgTypeId) {
        // Numerator needs to be cast
        QueryNode* pCastExpr = CastExpr::pCoerceTo(xAvgTypeId, pNumerator, pRootNode);

        // Numerator was copied above, so clean up
        delete pNumerator;
        pNumerator = pCastExpr;
      }

      if (Expr::xTypeId(pDenominator, pRootNode) != xAvgTypeId) {
        // Denominator needs to be cast
        QueryNode* pCastExpr = CastExpr::pCoerceTo(xAvgTypeId, pDenominator, pRootNode);

        // Denominator was copied above, so clean up
        delete pDenominator;
        pDenominator = pCastExpr;
      }

      // Build the division expression
      QueryNode* pDivExpr = DivisionExpr::pCreate(pNumerator, pDenominator);

      // Apply this template
      pAgg->vTransmogrifyTo(pDivExpr, true);
      ASSERT(DivisionExpr::bIsOfType(pAgg));

      // Delete the template
      delete pDivExpr;

    } else if (SumExpr::bIsOfType(pAgg)) {
      ASSERT(pAgg->nChildren() == 1);
      QueryNode* pChildExpr = _pQueryNode(pAgg->pFirstChild());

      // Create the inner SUM
      QueryNode* pInnerSum = SumExpr::pCreate(false, pChildExpr->pDeepCopy(true));

      // Insert the inner SUM as a child of the outer SUM
      GraphEditor::vReplaceChild(pAgg, pChildExpr, pInnerSum);


    } else if (CountExpr::bIsOfType(pAgg)) {
      // Form a template that we will then transmogrify the avgExpr node into.

      // Build the inner COUNT(x)
      QueryNode* pInnerCount = pAgg->pDeepCopy(true);

      // Build the outer SUM() expr
      QueryNode* pOuterSum = SumExpr::pCreate(false, pInnerCount);

      // Apply this template
      pAgg->vTransmogrifyTo(pOuterSum, true);
      ASSERT(SumExpr::bIsOfType(pAgg));

      // Delete the template
      delete pOuterSum;


    } else if (MinExpr::bIsOfType(pAgg)) {
      ASSERT(pAgg->nChildren() == 1);
      QueryNode* pChildExpr = _pQueryNode(pAgg->pFirstChild());

      // Create the inner MIN
      QueryNode* pInnerMin = pAgg->pDeepCopy(true);

      // Insert the inner MIN as a child of the outer MIN
      GraphEditor::vReplaceChild(pAgg, pChildExpr, pInnerMin);


    } else if (MaxExpr::bIsOfType(pAgg)) {
      ASSERT(pAgg->nChildren() == 1);
      QueryNode *pChildExpr = _pQueryNode(pAgg->pFirstChild());

      // Create the inner MAX
      QueryNode *pInnerMax = pAgg->pDeepCopy(true);

      // Insert the inner MAX as a child of the outer MAX
      GraphEditor::vReplaceChild(pAgg, pChildExpr, pInnerMax);

    } else {
      NEVER_REACH
    }
  }
  delete pIter;

  pAggList->vRemoveAll();
  delete pAggList;
}

/*----------------------------------------------------------------------------*/
/* Modifies subexpressions by promoting/casting data types so that they       */
/* are type compatible with this expression. Modifies the expression          */
/* tree in place.                                                             */
/*----------------------------------------------------------------------------*/
void Expr::vCoerceSubExprs(QueryNode* pExpr, PlanNode* pRootNode)
{
  _static_
  ASSERT(pExpr);
  ASSERT(bIsOfType(pExpr));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  if (pExpr->nChildren() == 0) {
    // Nothing to do!
    return;
  }

  // Any manual overrides should be handled here, otherwise common logic follows.
  // We would provide overrides here if we don't necessarily expect all of the children
  // of the expression to be of the same type. e.g. a CASE statement has multiple WHEN
  // exprs where each WHEN expr has a test predicate (should be a boolean) and a result
  // value. We expect all of the result values of each WHEN expr to be consistent.

  if (WhenExpr::bIsOfType(pExpr)) {
    WhenExpr::vCoerceSubExprs(pExpr, pRootNode);
    return;

  } else if (CaseExpr::bIsOfType(pExpr)) {
    CaseExpr::vCoerceSubExprs(pExpr, pRootNode);
    return;

  } else if (SubstringExpr::bIsOfType(pExpr)) {
    SubstringExpr::vCoerceSubExprs(pExpr, pRootNode);
    return;
  }

  ASSERT(Expr::xTypeId(pExpr, pRootNode).eCTVal() == CT_NONE);

  // Take a copy of the expression so it can be modified freely.
  QueryNode* pExprCopy = pExpr->pDeepCopy(false);

  // Now we can call a function that tries to coerce the children of this
  // expression to a common type. The first combination that succeeds in
  // returning a type that is not CT_NONE will be used.

  b1 bResult = bCoerceChildrenToCommonType(pExprCopy, pRootNode);

  if (bResult) {
    ASSERT(Expr::xTypeId(pExprCopy, pRootNode).eCTVal() != CT_NONE);

    // We need to replace the children of pExpr with the children of pExprCopy,
    // then clean up.

    // Delete all children in pExpr.
    QueryNode* pChild;
    while ((pChild = pExpr->pPopChild()) != nullptr) {
      ASSERT(Expr::bIsOfType(pChild));
      delete pChild;
    }

    ASSERT(pExpr->nChildren() == 0);

    // Move all children of pExprCopy to pExpr
    while ((pChild = pExprCopy->pPopChild()) != nullptr) {
      ASSERT(Expr::bIsOfType(pChild));
      pExpr->vAppendChild(pChild);
    }

    ASSERT(pExprCopy->nChildren() == 0);
    ASSERT(pExpr->nChildren() > 0);
  }

  delete pExprCopy;
}

/*----------------------------------------------------------------------------*/
/* Try to find a common type that all children of pExpr can be coerced to.    */
/* Return true if the coerced and modified pExpr now returns a value for      */
/* eType() that is not CT_NONE.                                               */
/*----------------------------------------------------------------------------*/
b1 Expr::bCoerceChildrenToCommonType(QueryNode* pExprCopy, PlanNode* pRootNode)
{
  ASSERT(Expr::bIsOfType(pExprCopy));
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(pExprCopy->nChildren() > 0);

  // The assumption is that all child expressions need to be coerced to the same type.
  // Therefore we can walk the child exprs and determine if there is a common type
  // that some or all of them can be coerced to.
  // We do this by looking at the current type and incrementing a counter if they can
  // be coerced to any other type. If all children can be coerced to the same type then
  // that is the common type that will be used.
  // If there is no common type then coercion is not possible and we abort. Otherwise we
  // try to coerce the necessary children and see if that allows the parent expr to return
  // a non-CT_NONE type.

  // An array with an occurrence count for each coercible data type
  i8 aiCoercibleTargets[CT_ENUMS] = { 0 };

  b1 bFixed = false;
  PlanNodeIter* pChildIter = pExprCopy->pNewChildrenIter();

  while (pChildIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pChildIter->pNext());
    ASSERT(Expr::bIsOfType(pChild));

    // Increment the aiCoercibleTargets array if this child can be coerced
    // to each of the data types.
    vIncrementCoercibleTargets(pChild, aiCoercibleTargets, pRootNode);
  }
  delete pChildIter;

  // Determine the common coercible data type, if any
  eCT eCoercibleTarget = eCommonCoercibleTarget(aiCoercibleTargets, pExprCopy->nChildren());
  TypeId xCoercibleTarget = TypeId::xIdOf(eCoercibleTarget);

  if (eCoercibleTarget == CT_NONE) {
    return false;
  }

  // Coerce each child and add them to a list. Remove all existing children of pExprCopy.
  QueryNodeList* pChildList = new QueryNodeList();
  QueryNode* pChild;
  while ((pChild = pExprCopy->pPopChild()) != nullptr) {

    TypeId xChildTypeId = Expr::xTypeId(pChild, pRootNode);
    eCT eChildType = xChildTypeId.eCTVal();

    QueryNode* pCoercedChild;

    if (eChildType == eCoercibleTarget) {
      // No need to coerce, but we need to duplicate the child expr anyway.
      pCoercedChild = pChild->pDeepCopy(false);

    } else {
    // Coerce it to the new type. This will create a new expression object.
      pCoercedChild = Expr::pCoerceTo(xCoercibleTarget, pChild, pRootNode);
    }

    ASSERT(pCoercedChild);
    Expr::bIsOfType(pCoercedChild);

    pChildList->vAppend(pCoercedChild);
    delete pChild;
  }

  ASSERT(pExprCopy->nChildren() == 0);

  // Add the coerced children to pExprCopy
  while ((pChild = pChildList->pPop()) != nullptr) {
    pExprCopy->vAppendChild(pChild);
  }

  ASSERT(pChildList->nItems() == 0);
  delete pChildList;

  // Finally, did this do enough to make the expression viable?
  bFixed = (Expr::xTypeId(pExprCopy, pRootNode).eCTVal() != CT_NONE);
  return bFixed;
}

/*----------------------------------------------------------------------------*/
/* Increments the integer elements of an array if the given expression is     */
/* coercible to each eCT data type.                                           */
/*----------------------------------------------------------------------------*/
void Expr::vIncrementCoercibleTargets(QueryNode* pExpr, i8 (&aiCoercibleTargets)[CT_ENUMS], PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));

   i8 i = 0;

  while (++i != CT_ENUMS) {
    eCT eTarget = (eCT) i;
    TypeId xTargetTypeId = TypeId::xIdOf(eTarget);

    if (bIsImplicitlyCoercibleTo(xTargetTypeId, pExpr, pRootNode)) {
      aiCoercibleTargets[i] += 1;
    }
  }
}


/*----------------------------------------------------------------------------*/
/* Given an array of counts for each data types and a maximum count value,    */
/* choose the data type that should be coerced to, or CT_NONE if none of      */
/* them reached iMaxCount.                                                    */
/*----------------------------------------------------------------------------*/
eCT Expr::eCommonCoercibleTarget(i8 (&aiCoercibleTargets)[CT_ENUMS], i8 iMaxCount)
{
  i8 iNumAtMaxCount = 0;
  eCT eCommonTarget = CT_NONE;

  // First check to see how many data types have a count of iMaxCount.
  // If there is only one then that's our answer.
  for (i8 i=0; i<CT_ENUMS; i++) {
    if (aiCoercibleTargets[i] == iMaxCount) {
      ++iNumAtMaxCount;
      eCommonTarget = (eCT) i;
    }
  }

  if (iNumAtMaxCount == 1) {
    ASSERT(eCommonTarget != CT_NONE);
    ASSERT(eCommonTarget != CT_ENUMS);
    return eCommonTarget;

  } else if (iNumAtMaxCount == 0) {
    return CT_NONE;
  }

  // We have multiple data types that we can coerce to and we have to pick one.
  // If the common targets are numerics or datetimes then we try to pick the
  // simplest form of them - i.e. integers over decimals and floats and dates
  // over timestamps.

  // If there are multiple common coercible targets, this is the order of
  // data types that we will try to coerce to. They are tried from first
  // entry to the last.
  static eCT aeCoercionOrdering[CT_ENUMS - 1] = {
      /*  0 */ CT_INTEGER,
      /*  1 */ CT_DECIMAL,
      /*  2 */ CT_FLOAT,
      /*  3 */ CT_BOOLEAN,
      /*  4 */ CT_DATE,
      /*  5 */ CT_TIME,
      /*  6 */ CT_INTERVAL_DT,  // day-time interval
      /*  7 */ CT_INTERVAL_YM,  // year-month interval
      /*  8 */ CT_TIMESTAMP,
      /*  9 */ CT_VARCHAR,
      /* 10 */ CT_CHAR,
      /* 11 */ CT_UNKNOWN
  };

  ASSERT(iNumAtMaxCount > 1);

  for (eCT ePreference : aeCoercionOrdering) {
    if (aiCoercibleTargets[ePreference] == iMaxCount) {
      eCommonTarget = ePreference;
      break;
    }
  }

  return eCommonTarget;
}

/*----------------------------------------------------------------------------*/
/* Returns true if an expression can be _implicitly_ coerced to a given type. */
/* Note that the system may support additional explicit casts, but only the   */
/* implicit ones are done automatically.                                      */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsImplicitlyCoercibleTo(TypeId xTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));
  ASSERT(Expr::xTypeId(pExpr, pRootNode).bIsValid());

  // If we have a literal then we need to check whether we can convert it appropriately.
  // e.g. we can convert '2000-01-01' to a CT_DATE, but not '2000_01_01'.

  if (LiteralExpr::bIsOfType(pExpr)) {
    return LiteralExpr::bIsImplicitlyCoercibleTo(xTargetTypeId, pExpr, pRootNode);
  }

  // Otherwise we need to cast the expression, so let's ask it whether it can be coerced.
  return CastExpr::bIsImplicitlyCoercibleTo(xTargetTypeId, pExpr, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Returns a new expression object which represents pExpr coerced to eTarget  */
/*----------------------------------------------------------------------------*/
QueryNode* Expr::pCoerceTo(TypeId xTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(xTargetTypeId.bIsValid());
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));
  ASSERT(Expr::xTypeId(pExpr, pRootNode).bIsValid());

  // If we have a literal then we will simply replace it with another literal.
  if (LiteralExpr::bIsOfType(pExpr)) {
    return LiteralExpr::pCoerceTo(xTargetTypeId, pExpr, pRootNode);
  }

  // Otherwise we need to cast the expression.
  return CastExpr::pCoerceTo(xTargetTypeId, pExpr, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Compare two expression trees and return true if they are equal with        */
/* respect to the operations and symbols used.                                */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsCommutativelyEqual(QueryNode* pExpr1, QueryNode* pExpr2)
{
  ASSERT(pExpr1 != pExpr2);
  ASSERT(pExpr1);
  ASSERT(pExpr2);
  ASSERT(pExpr1->szType()->bEqual(pExpr2->szType()));

  // First check that we've got a commutative operator
  b1 bCommutativeExpr =
            AdditionExpr::bIsOfType(pExpr1) ||
      MultiplicationExpr::bIsOfType(pExpr1) ||
                 AndExpr::bIsOfType(pExpr1) ||
                  OrExpr::bIsOfType(pExpr1) ||
                 XorExpr::bIsOfType(pExpr1) ||
                  EqExpr::bIsOfType(pExpr1) ||
                  NeExpr::bIsOfType(pExpr1) ;

  if (!bCommutativeExpr) {
    return false;
  }

  ASSERT(pExpr1->nChildren() == 2);
  ASSERT(pExpr1->nChildren() == pExpr2->nChildren());

  // We already know that the two expressions are not symbolically equal as
  // they are. But if the types are the same and both have two children, what
  // if we symbolically compare:
  //   pExpr1->pChild1() and pExr2->pChild2()
  //   and
  //   pExpr1->pChild2() and pExr2->pChild1()
  // If these are symbolically equal then the two parent expressions are also
  // symbolically equal.

  QueryNode* pChild1 = _pQueryNode(pExpr1->pFirstChild());
  QueryNode* pChild2 = _pQueryNode(pExpr2->pSecondChild());

  if (!bIsSymbolicallyEqual(pChild1, pChild2)) {
    return false;
  }

  pChild1 = _pQueryNode(pExpr1->pSecondChild());
  pChild2 = _pQueryNode(pExpr2->pFirstChild());

  b1 bIsEqual = bIsSymbolicallyEqual(pChild1, pChild2);
  return bIsEqual;
}

/*----------------------------------------------------------------------------*/
/* Compare two expression trees and return true if they are equal with        */
/* respect to the operations and symbols used.                                */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsMathematicallyEqual(QueryNode* pExpr1, QueryNode* pExpr2)
{
  ASSERT(pExpr1 != pExpr2);
  ASSERT(pExpr1);
  ASSERT(pExpr2);

  if (pExpr1->nChildren() != 2) return false;
  if (pExpr1->nChildren() != pExpr2->nChildren())  return false;


  QueryNode* pChild11 = _pQueryNode(pExpr1->pFirstChild());
  QueryNode* pChild12 = _pQueryNode(pExpr1->pSecondChild());
  QueryNode* pChild21 = _pQueryNode(pExpr2->pFirstChild());
  QueryNode* pChild22 = _pQueryNode(pExpr2->pSecondChild());

  // First check that we've got compatible operators

  // Comparing: < and > operators
  // Check if the following combinations of expression operator types are equal
  //   <  and >
  //   >  and <
  //   <= and >=
  //   >= and <=
  if ( (LtExpr::bIsOfType(pExpr1) && GtExpr::bIsOfType(pExpr2)) ||
       (GtExpr::bIsOfType(pExpr1) && LtExpr::bIsOfType(pExpr2)) ||
       (LeExpr::bIsOfType(pExpr1) && GeExpr::bIsOfType(pExpr2)) ||
       (GeExpr::bIsOfType(pExpr1) && LeExpr::bIsOfType(pExpr2)) )
  {
    // child11 < child12 is symbolically to  child21 > child22
    //  if child11 is symbolically equal to child22
    // and child12 is symbolically equal to child21
    return bIsSymbolicallyEqual(pChild11, pChild22) && bIsSymbolicallyEqual(pChild12, pChild21);
  }

  // TODO: We can add in other checks here as they come up
  // TODO: e.g.
  // TODO:    A  = B   <-->  A-B = 0   (and vice versa)
  // TODO:    A  = B+C <-->  A-C = B   (and vice versa)
  // TODO:    A  = B+C <-->  A-B = C
  // TODO:    A  <  B  <-->  A-B <  0
  // TODO:    A  <= B  <-->  A-B <= 0
  // TODO:    A  >  B  <-->  A-B >  0
  // TODO:    A  >= B  <-->  A-B >= 0
  // TODO:
  // TODO:    De Morgan's Laws:
  // TODO:      NOT(A OR B)  <--> NOT(A) AND NOT(B)
  // TODO:      NOT(A AND B) <--> NOT(A) OR NOT(B)


  return false;
}

/*----------------------------------------------------------------------------*/
/* Find the SqlOperator node that contains this expression instance.          */
/* Involves a search of the graph, so can be an expensive operation.          */
/*----------------------------------------------------------------------------*/
QueryNode* Expr::pFindEnclosingSqlOp(QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(bIsOfType(pExpr));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Find the path of pExpr in the graph
  s2K sExprPath;
  i8 iDepth;

  b1 bFound = GraphNav::bGetPathAndDepth(pExpr, pRootNode, sExprPath, iDepth, true);
  ASSERT(bFound);
  UNUSED(bFound);
  ASSERT(sExprPath.nLength() > 0);

  // The parent SqlOperator is the path that contains this expression as a property.
  s2K sSqlOpPath;
  sSqlOpPath.vCopyTill(sExprPath, C_PROPERTY_SEPARATOR);
  ASSERT(!sExprPath.bEqual(sSqlOpPath.szStz()));

  // Look up the path in the grapbh
  QueryNode* pSqlOp = _pQueryNode(GraphNav::pFind(sSqlOpPath.szStz(), pRootNode));
  ASSERT(pSqlOp);
  ASSERT(SqlOperator::bIsOfType(pSqlOp));

  return pSqlOp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
