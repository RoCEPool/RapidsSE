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
#include "AliasRef.hpp"
#include "ColumnWildcard.hpp"
#include "ColumnOrdinal.hpp"
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
  else if (      AliasRef::bIsOfType(pNode))  {       AliasRef::vDescribe(pNode, sBuf); }
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
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_EXPR_DEFN);
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
  else if (       AliasRef::bIsOfType(pExprNode))  { return       AliasRef::eType(pExprNode, pRootNode); }
  else if ( ColumnWildcard::bIsOfType(pExprNode))  { return ColumnWildcard::eType(pExprNode, pRootNode); }
  else if (   OperatorExpr::bIsOfType(pExprNode))  { return   OperatorExpr::eType(pExprNode, pRootNode); }
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
  auto fVisitor = [&bIsAgg](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {
    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (AggregateExpr::bIsOfType(pNode)) {
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
  auto fVisitor = [&bHasNonAggColumnRef](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (AggregateExpr::bIsOfType(pNode)) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    if (ColumnRef::bIsOfType(pNode)) {
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
b1 Expr::bIsLiteral(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  b1 bIsLit = true;

  // Walk the graph, looking at the children only and return if any of them
  // are ColumnRefs or ColumnOrdinals expressions.
  auto fVisitor = [&bIsLit](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (    ColumnRef::bIsOfType(pNode) ||
        ColumnOrdinal::bIsOfType(pNode)) {
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
QueryNode* Expr::pSymbolicToOrdinal(QueryNode*     pSymbolicExpr,
                                    QueryNodeList* pColumnExprList,
                                    PlanNode*      pRootNode)
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
  QueryNodeIter* pIter = pColumnExprList->pNewIter();
  i8 iOrdinal = 1;

  while (pIter->bHasNext()) {
    QueryNode* pProjectedColumn = pIter->pNext();
    QueryNode* pColExpr = ProjectedColumn::pExprNode(pProjectedColumn);

//    s2K sBuf1;
//    s2K sBuf2;
//    Expr::vDescribe(pSymbolicExpr, sBuf1);
//    Expr::vDescribe(pColExpr, sBuf2);
//    vTrace("bIsSymbolicallyEqual:\n  %s\n  %s\n\n"_Stz, sBuf1.szStz(), sBuf2.szStz());

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
    QueryNode* pChildOrdinalExpr  = pSymbolicToOrdinal(pChildSymbolicExpr, pColumnExprList, pRootNode);
    pOrdinalChildren->vAppend(pChildOrdinalExpr);
  }
  delete pChildIter;

  // Now we can deep copy the current symbolic expression but give it new children
  pOrdinalExpr = pSymbolicExpr->pDeepCopy(true, pOrdinalChildren);

  return pOrdinalExpr;
}

/*----------------------------------------------------------------------------*/
/* Compare two expression trees and return true if they are equal with        */
/* respect to the operations and symbols used.                                */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsSymbolicallyEqual(QueryNode* pExpr1, QueryNode* pExpr2)
{
  // Are we comparing the same actual objects?
  if (pExpr1 == pExpr2) return true;

  // Is one of them not null and the other null?
  if (pExpr1 && !pExpr2) return false;
  if (!pExpr1 && pExpr2) return false;

  // We don't compare names as they are allowed to be different
  // (except in the case of properties, but see below).

  // The type of the expressions must must match
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
b1 Expr::bIsOrdinal(QueryNode* pExpr)
{
  b1 bIsOrdinal = true;

  // Walk the expression graph (children only), looking for ColumnRef.
  // Lambda to execute on each node:
  auto fVisitor = [&bIsOrdinal](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (ColumnRef::bIsOfType(pNode)) {
      bIsOrdinal = false;
      return GraphNav::WALK_ABORT;
    }

    // Column wildcards should have been replaced by now
    ASSERT(!ColumnWildcard::bIsOfType(pNode));

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bBoundedWalk(pExpr, fVisitor, nullptr);

  return bIsOrdinal;
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
