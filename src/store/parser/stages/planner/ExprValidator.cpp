/*============================================================================*/
/* ExprValidator.hpp - Validates that expressions are being specified and     */
/* used correctly                                                             */
/*============================================================================*/

#include "BASE.h"
#include "ExprValidator.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "SqlException.hpp"

#include "Expr.hpp"
#include "AggregateExpr.hpp"
#include "LiteralExpr.hpp"
#include "ColumnRef.hpp"
#include "ProjectedColumn.hpp"
#include "FilterOp.hpp"
#include "AggregateOp.hpp"
#include "OrderOp.hpp"
#include "OrderItem.hpp"
#include "OrderItemList.hpp"
#include "StatementMarker.hpp"
#include "SetOp.hpp"
#include "JoinOp.hpp"
#include "SubqueryRefExpr.hpp"
#include "SubqueryMarker.hpp"
#include "ExprSubquery.hpp"
#include "CteDef.hpp"

#include "ComparisonExpr.hpp"
#include "EqExpr.hpp"
#include "InExpr.hpp"
#include "ExistsExpr.hpp"
#include "CaseExpr.hpp"
#include "WhenExpr.hpp"

#include "TypeSystem.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Walk the graph and check the semantics of the expressions.                 */
/* e.g. Check that the operation supports the input data types.               */
/* A SqlException will be thrown if an invalid semantic is found.             */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateExprs(PlanNode *pRootNode)
{
  // Find the /plan node
  QueryNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);

  // The plan node should have one or more children that are SubqueryMarker nodes
  ASSERT(pPlanNode->nChildren() > 0);

  // We want to iterate over each logical subgraph, bounded by StatementMarkers,
  // and run the validation checks on each of them.

  auto fVisitor = [pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    // Run the validation logic on the logical subgraph of each StatementMarker encountered.

    // Coerce any expression types to avoid an invalid result type by apply casts as applicable
    vExprTypeCoercion(pRootNode, pStatementMarker);

    // Validate that the result types of any expressions are not invalid.
    vValidateExprResultType(pRootNode, pStatementMarker);

    // Validate that all case expression predicates have a boolean result type
    vValidateCasePredicatesAreBoolean(pRootNode, pStatementMarker);

    // Validate that all operator predicate expressions have a boolean result type
    vValidateOperatorPredicatesAreBoolean(pRootNode, pStatementMarker);

    // Validate that all join predicate expressions have a boolean result type
    vValidateJoinPredicatesAreBoolean(pRootNode, pStatementMarker);

    // Validate that all join predicate expressions produce equi-joins
    vValidateJoinPredicatesAreEquiJoins(pRootNode, pStatementMarker);

    // Validate any aggregate/group-by expressions
    vValidateGroupByExprs(pRootNode, pStatementMarker);

    // Validate that expressions above an AggregateOp are consistent with the
    // group by list (if applicable)
    vValidateSqlOpExprsAgainstGroupBy(pRootNode, pStatementMarker);

    // Similarly, if there is no GROUP BY list then check that expressions
    // above the ProjectOp are consistent with whether the ProjectOp is
    // projecting "grand totals" (single group) or non-aggregated columns
    vValidateSqlOpExprsAgainstProjectOp(pRootNode, pStatementMarker);

    // Validate that ProjectOps cannot produce both aggregate & non-aggregate
    // expressions without a group-by clause
    vValidateProjectExprs(pRootNode, pStatementMarker);

    // Validate that any expression subqueries are projecting the right number of columns
    // for the operator they are used in.
    vValidateExprSubqueryProjections(pRootNode, pStatementMarker);

    return GraphNav::WALK_CONTINUE;
  };

  // Walk the entire plan graph to look for StatementMarkers
  GraphNav::bGlobalWalk(pPlanNode, false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Coerce any expression types to avoid an invalid result type by applying    */
/* type casts as appropriate.                                                 */
/*----------------------------------------------------------------------------*/
void ExprValidator::vExprTypeCoercion(PlanNode *pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // Walk the bounded graph from the bottom up looking for all expressions.
  // That way if we change an expression then we can also check for the effects
  // of that change as we walk upwards. A downwards walk wouldn't allow us to
  // be able to do that.
  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!Expr::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pExprNode = _pQueryNode(pNode);

    // How do we know if an expr needs to be coerced? One way is to check
    // the return type of the expression. It it is invalid it will be set
    // to eCT::CT_NONE. Then we can try to coerce the sub-expressions involved.
    //
    // But this doesn't work for all expressions - there are some special
    // cases such as IN expr. An IN expr always returns a BOOLEAN but some
    // of its values may need to be coerced. So we also handle these cases
    // explicitly.
    //
    // Expression coercion might not succeed. In that case we will pick up
    // the error in vValidateExprResultType() and throw an error there.

    if (Expr::iType(pExprNode, pRootNode) == eCT::CT_NONE) {
      // We might need type coercion here
      Expr::vCoerceSubExprs(pExprNode, pRootNode);

      // If we still haven't been able to correct the problem then abort any
      // further coercions. This error will be picked up in vValidateExprResultType().
      if (Expr::iType(pExprNode, pRootNode) == eCT::CT_NONE) {
        return GraphNav::WALK_ABORT;
      }
    }

    // Keep walking up the graph
    return GraphNav::WALK_CONTINUE;
  };

  // Walk the node, it's properties and its children
  GraphNav::bBoundedWalkBottomUp(pStartNode->pFirstChild(), true, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Validate that the result types of any expressions are not invalid.         */
/* An invalid combination of exprs is one that returns eCT::CT_NONE for its type.  */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateExprResultType(PlanNode *pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // Walk the graph looking for all expressions
  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!Expr::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pExprNode = _pQueryNode(pNode);

    // This will throw an exception if an error is found.
    Expr::vValidateResultType(pExprNode, pRootNode);

    // No need to descend any further into the expression
    return GraphNav::WALK_SKIP_SUBTREE;
  };

  // Walk the node, it's properties and its children
  GraphNav::bBoundedWalk(pStartNode->pFirstChild(), true, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Validate that all case expression predicates have a boolean result type    */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateCasePredicatesAreBoolean(PlanNode *pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // Walk the graph looking for all WhenExpr objects
  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!WhenExpr::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pWhenExpr = _pQueryNode(pNode);
    QueryNode* pPredicate = WhenExpr::pPredicateExpr(pWhenExpr);
    ASSERT(Expr::bIsOfType(pPredicate));

    eCT eType = TypeSystem::eCTVal(Expr::iType(pPredicate, pRootNode));

    if (eType == eCT::CT_BOOLEAN) {
      // All good, keep looking for more filterOps
      return GraphNav::WALK_CONTINUE;
    }

    // Throw an error
    s2K sBuf;
    Expr::vDescribe(pWhenExpr, sBuf, pRootNode);

    SemanticException::vThrow(EC_EXPR_MUST_BE_BOOLEAN,
                              " : %s"_Stz,
                              sBuf.szStz());

    return GraphNav::WALK_ABORT; // Only required to silence a compiler warning
  };

  // Walk the node and its children. We're not interested in walking
  // the properties because we are looking for filterOps
  GraphNav::bBoundedWalk(pStartNode->pFirstChild(), true, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Validate that all predicate expressions (e.g. in filterOps) return a       */
/* boolean expression.                                                        */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateOperatorPredicatesAreBoolean(PlanNode *pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // Walk the graph looking for all filterOps
  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!FilterOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pFilterNode = _pQueryNode(pNode);
    QueryNode* pExprNode = FilterOp::pFilterExpr(pFilterNode);

    eCT eType = TypeSystem::eCTVal(Expr::iType(pExprNode, pRootNode));

    if (eType == eCT::CT_BOOLEAN) {
      // All good, keep looking for more filterOps
      return GraphNav::WALK_CONTINUE;
    }

    // Throw an error
    s2K sBuf;
    Expr::vDescribe(pExprNode, sBuf, pRootNode);

    SemanticException::vThrow(EC_EXPR_MUST_BE_BOOLEAN,
                              " : %s"_Stz,
                              sBuf.szStz());

    return GraphNav::WALK_ABORT; // Only required to silence a compiler warning
  };

  // Walk the node and its children. We're not interested in walking
  // the properties because we are looking for filterOps
  GraphNav::bBoundedWalk(pStartNode->pFirstChild(), false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate that all join predicate expressions (e.g. in JoinOps) return a    */
/* boolean expression.                                                        */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateJoinPredicatesAreBoolean(PlanNode *pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // Walk the graph looking for all JoinOps
  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!JoinOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pJoinNode = _pQueryNode(pNode);
    QueryNode* pExprNode = JoinOp::pJoinPredicate(pJoinNode);

    eCT eType = TypeSystem::eCTVal(Expr::iType(pExprNode, pRootNode));

    if (eType == eCT::CT_BOOLEAN) {
      // All good, keep looking for more JoinOps
      return GraphNav::WALK_CONTINUE;
    }

    // Throw an error
    s2K sBuf;
    Expr::vDescribe(pExprNode, sBuf, pRootNode);

    SemanticException::vThrow(EC_EXPR_MUST_BE_BOOLEAN,
                              " : %s"_Stz,
                              sBuf.szStz());

    return GraphNav::WALK_ABORT; // Only required to silence a compiler warning
  };

  // Walk the node and its children. We're not interested in walking
  // the properties because we are looking for JoinOps
  GraphNav::bBoundedWalk(pStartNode->pFirstChild(), false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate that all join predicate expressions are equi-joins                */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateJoinPredicatesAreEquiJoins(PlanNode *pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // Walk the graph looking for all JoinOps
  auto fVisitor = [pRootNode](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!JoinOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    if (JoinOp::eJoinType(pNode) == JT_CROSS) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pJoinNode = _pQueryNode(pNode);
    QueryNode* pExprNode = JoinOp::pJoinPredicate(pJoinNode);
    b1 bGotEqComparison  = false;

    // Walk the expression tree looking for ComparisonExpr.
    // If we get one that isn't an EqExpr then we have a non-equi-join
    auto fExprVisitor = [&bGotEqComparison]
                        (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {
      UNUSED(sPath);
      UNUSED(iDepth);
      UNUSED(pvArg);

      if (EqExpr::bIsOfType(pNode)) {
        bGotEqComparison = true;
        return GraphNav::WALK_ABORT;
      }

      return GraphNav::WALK_CONTINUE;
    };

    // Walk the expression tree
    GraphNav::bGlobalWalk(pExprNode, false, true, fExprVisitor, nullptr);

    if (!bGotEqComparison) {
      // Throw an error
      s2K sBuf;
      Expr::vDescribe(pExprNode, sBuf, pRootNode);

      SemanticException::vThrow(EC_EXPR_MUST_BE_EQUIJOIN,
                                " : %s"_Stz,
                                sBuf.szStz());
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk the node and its children. We're not interested in walking
  // the properties because we are looking for JoinOps
  GraphNav::bBoundedWalk(pStartNode->pFirstChild(), false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate that aggregate and group-by expressions are correctly specified   */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateGroupByExprs(PlanNode *pRootNode, QueryNode* pStartNode)
{
  UNUSED(pRootNode);

  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // If there is group by list then an AggregateOp will be generated
  // rather than an ProjectOp for an early plan.
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);
    return AggregateOp::bIsExactType(pNode);
  };

  QueryNode* pAggregateOp = _pQueryNode(
      GraphNav::pGlobalSearch(pStartNode, false, true, fCondition, nullptr) );

  if (!pAggregateOp) {
    return;
  }

  // Get the list of projected expressions
  QueryNode* pProjectExprs = AggregateOp::pProjectedColumns(pAggregateOp);
  UNUSED(pProjectExprs);

  // Get the group by list
  QueryNode* pGroupByList = AggregateOp::pGroupByList(pAggregateOp);

  // Check that all non-aggregate expressions that are being projected
  // are either a part of the group by list or are dependent on the
  // expressions in the group by list
  // This is done in vValidateExprsAgainstGroupBy()

  // Check that the group by expressions cannot be aggregate expressions
  vCheckGroupByExprsNotAggregates(pGroupByList, pRootNode);

  // Check that the group by expressions are not literals.
  // TODO: Eventually we will support integer literals here to refer to column ordinals
  vCheckGroupByExprsNotLiterals(pGroupByList, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate that if there is no group by list then projections must either be */
/* all aggregates or all non-aggregates. This is applicable to an early plan. */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateProjectExprs(PlanNode *pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // If there is no group by list then a simple ProjectOp will be generated
  // rather than an AggregateOp.
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);
    return ProjectOp::bIsExactType(pNode);
  };

  QueryNode* pProjectOp = _pQueryNode(
      GraphNav::pGlobalSearch(pStartNode, false, true, fCondition, nullptr) );

  if (!pProjectOp) {
    return;
  }

  // Get the list of projected expressions
  QueryNode* pProjectExprs = ProjectOp::pProjectedColumns(pProjectOp);

  // Iterate the expressions being projected
  PlanNodeIter* pIter = pProjectExprs->pNewChildrenIter();

  // The basic algorithm is to work out whether this ProjectOp is producing
  // a mix of aggregate expressions and non-aggregate expressions together.
  // To do that we will iterate each of the expressions in the projection
  // list, convert them to ordinal expressions (as this will represent the
  // work that this ProjectOp is doing rather than what work has been done
  // in the plan below it), and then count how many of these expressions
  // are aggregate/non-aggregates.

  // Get the list of column expressions being given to this plan from its child.
  // This is the list of expression symbols that represent each column of
  // each row that this sql operator will receive.
  ASSERT(pProjectOp->nChildren() == 1);
  QueryNodeList* pColExprList = PlanObject::pColumnExprs(_pQueryNode(pProjectOp->pFirstChild()), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pProjectOp, pRootNode);

  // Let's count how many aggregate columns there are and compare to the number of children
  i8 iNumColumns = pProjectExprs->iNumChildren();
  i8 iNumAggCols = 0;
  i8 iNumLitCols = 0; // literal values only

  while (pIter->bHasNext()) {
    QueryNode* pProjColumn = _pQueryNode(pIter->pNext());
    QueryNode* pExpr = ProjectedColumn::pExprNode(pProjColumn);

    // Convert the expression from a symbolic expression to an ordinal one.
    // This represents the work that this ProjectOp has to do.
    QueryNode* pOrdExpr = Expr::pSymbolicToOrdinal(pExpr, pColExprList, pCorrelatedExprs, pRootNode);

    // Check if this ordinal expression is an aggregate
    if (Expr::bContainsAnAggregate(pOrdExpr)) {
      ++iNumAggCols;

    } else if (Expr::bIsFullyLiteral(pOrdExpr)) {
      ++iNumLitCols;
    }

    delete pOrdExpr;
  }
  delete pIter;
  pColExprList->vDeleteAll();
  delete pColExprList;

  if (iNumAggCols == 0 || iNumAggCols == iNumColumns - iNumLitCols) {
    // We are projecting all aggregates or all non-aggregates.
    // So no problems here
    return;
  }

  // Throw an error
  SemanticException::vThrow(EC_EXPR_AGG_MIX_NO_GROUPBY_LIST);
}


/*----------------------------------------------------------------------------*/
/* Check that any non-aggregated columnRefs in the expression are either a    */
/* part of the group by list or are dependent on the expressions in the group */
/* by list                                                                    */
/*----------------------------------------------------------------------------*/
void ExprValidator::vCheckNonAggExprIsInGroupBy(QueryNode* pExpr, QueryNode *pGroupByList, PlanNode* pRootNode)
{
  ASSERT(pExpr);

  // We will walk the expression tree and:
  //  - if the subExpr is an actual expression aggregate then skip that subtree
  //  - if the subExpr is a literal then skip that subtree
  //  - if the subExpr matches any of the group-by expressions then skip that subtree
  //  - if the subExpr is a ColumnRef then we have an error (because it is not in
  //    the groupby list)
  //  - otherwise continue walking/descending the expression graph

  auto fVisitor = [pGroupByList, pRootNode](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    ASSERT(Expr::bIsOfType(pNode));

    QueryNode* pExpr = _pQueryNode(pNode);

    // Ignore aggregate expressions
    if (AggregateExpr::bIsOfType(pExpr)) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    // Ignore literals
    if (LiteralExpr::bIsOfType(pExpr)) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    // if this expression exactly matches any of the group by expressions then
    // we can skip this subtree
    PlanNodeIter* pIter = pGroupByList->pNewChildrenIter();;

    while (pIter->bHasNext()) {
      QueryNode* pGroupByExpr = _pQueryNode(pIter->pNext());

      if (Expr::bIsSymbolicallyEqual(pExpr, pGroupByExpr)) {
        delete pIter;
        return GraphNav::WALK_SKIP_SUBTREE;
      }
    }

    delete pIter;

    // If this is a ColumnRef then we know it is not in an aggregation and not in
    // the group by list so it is an error.
    if (ColumnRef::bIsOfType(pExpr)) {
      s2K sBuf;
      Expr::vDescribe(pExpr, sBuf, pRootNode);

      SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST, ": %s"_Stz, sBuf.szStz());
    }

    if (SubqueryRefExpr::bIsOfType(pExpr)) {
      // Recursively check that any parameters to this subquery are also in the group by list
      QueryNodeList* pParamList = SubqueryRefExpr::pSymbolicSubqueryParams(pExpr, pRootNode);
      QueryNodeIter* pParamIter = pParamList->pNewIter();

      try {
        while (pParamIter->bHasNext()) {
          QueryNode* pChildParam = pParamIter->pNext();
          vCheckNonAggExprIsInGroupBy(pChildParam, pGroupByList, pRootNode);
        }

      } catch (SqlException& e) {
        delete pParamIter;
        pParamList->vDeleteAll();
        delete pParamList;

        throw;
      }

      delete pParamIter;
      pParamList->vDeleteAll();
      delete pParamList;
    }

    // Keep walking/descending the expression graph
    return GraphNav::WALK_CONTINUE;
  };

  // Walk the expression graph
  GraphNav::bBoundedWalk(pExpr, false, true, fVisitor, nullptr);
}


/*----------------------------------------------------------------------------*/
/* Check that the group by expressions cannot be aggregate expressions        */
/*----------------------------------------------------------------------------*/
void ExprValidator::vCheckGroupByExprsNotAggregates(QueryNode *pGroupByList, PlanNode* pRootNode)
{
  ASSERT(pGroupByList);
  ASSERT(pGroupByList->nChildren() > 0);

  PlanNodeIter* pIter = pGroupByList->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pExpr = _pQueryNode(pIter->pNext());

    if (Expr::bContainsAnAggregate(pExpr)) {
      delete pIter;

      // Throw an error
      s2K sBuf;
      Expr::vDescribe(pExpr, sBuf, pRootNode);

      SemanticException::vThrow(EC_EXPR_IS_AGG_IN_GROUPBY_LIST,
                                " : %s"_Stz,
                                sBuf.szStz());
    }
  }

  delete pIter;
}


/*----------------------------------------------------------------------------*/
/* Check that the group by expressions are not literals                       */
/*----------------------------------------------------------------------------*/
void ExprValidator::vCheckGroupByExprsNotLiterals(QueryNode *pGroupByList, PlanNode* pRootNode)
{
  // TODO: Eventually we will support integer literals here to refer to column ordinals
  //       But for now we will just ban all literals

  ASSERT(pGroupByList);
  ASSERT(pGroupByList->nChildren() > 0);

  PlanNodeIter* pIter = pGroupByList->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pExpr = _pQueryNode(pIter->pNext());

    if (Expr::bIsFullyLiteral(pExpr)) {
      delete pIter;

      // Throw an error
      s2K sBuf;
      Expr::vDescribe(pExpr, sBuf, pRootNode);

      SemanticException::vThrow(EC_EXPR_IS_LIT_IN_GROUPBY_LIST,
                                " : %s"_Stz,
                                sBuf.szStz());
    }
  }

  delete pIter;
}


/*----------------------------------------------------------------------------*/
/* If there is a GROUP BY then validate that all sql operators that sit above */
/* an AggregateOp and have expressions, those expressions must only refer to  */
/* aggregated columns or columns that appear in the group by list             */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateSqlOpExprsAgainstGroupBy(PlanNode* pRootNode, QueryNode* pStartNode)
{
  UNUSED(pRootNode);

  // First find the AggregateOp (if one exists) so we can get the group by list
  auto fCondition = [](PlanNode* pNode, void* pvArg)-> b1 {
    UNUSED(pvArg);
    return (AggregateOp::bIsExactType(pNode));
  };

  QueryNode* pAggregateOp = _pQueryNode(
      GraphNav::pGlobalSearch(pStartNode, false, true,
                              fCondition, nullptr) );

  if (!pAggregateOp) {
    return;
  }

  // Get the group by list
  QueryNode* pGroupByList = AggregateOp::pGroupByList(pAggregateOp);

  // Now walk the plan again until we hit this AggregateOp and check that
  // all expressions are either aggregates or they use expressions from
  // the group by list.

  auto fVisitor = [pAggregateOp, pGroupByList, pRootNode](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    ASSERT(PlanObject::bIsOfType(pNode));
    QueryNode* pPlanOp = _pQueryNode(pNode);

    b1 bExit = false;

    if (pNode == pAggregateOp) {
      // We've reach the last node
      bExit = true;
    }

    if (FilterOp::bIsOfType(pPlanOp)) {
      // Covers the HAVING clause
      QueryNode* pExpr = FilterOp::pFilterExpr(pPlanOp);
      vCheckNonAggExprIsInGroupBy(pExpr, pGroupByList, pRootNode);

    } else if (ProjectOp::bIsOfType(pPlanOp)) {
      // Covers both ProjectOp in the case of no group by, as well
      // as AggregateOp when a groupby does exit.
      QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pPlanOp);

      PlanNodeIter* pProjectedColumnIter = pProjectedColumns->pNewChildrenIter();
      while (pProjectedColumnIter->bHasNext()) {
        QueryNode* pProjectColumn = _pQueryNode(pProjectedColumnIter->pNext());
        ASSERT(ProjectedColumn::bIsOfType(pProjectColumn));

        QueryNode* pExpr = ProjectedColumn::pExprNode(pProjectColumn);

        try {
          vCheckNonAggExprIsInGroupBy(pExpr, pGroupByList, pRootNode);

        } catch (SemanticException& e) {
          delete pProjectedColumnIter;
          throw;
        }
      }

      delete pProjectedColumnIter;

    } else if (OrderOp::bIsOfType(pPlanOp)) {
      // Iterate the order expressions in the order by operator

      QueryNode* pOrderItemList = OrderOp::pOrderItemList(pPlanOp);
      ASSERT(OrderItemList::bIsOfType(pOrderItemList));

      PlanNodeIter* pIter = pOrderItemList->pNewChildrenIter();
      while (pIter->bHasNext()) {
        QueryNode *pOrderItem = _pQueryNode(pIter->pNext());
        ASSERT(OrderItem::bIsOfType(pOrderItem));

        QueryNode *pExpr = OrderItem::pExprNode(pOrderItem);

        try {
          vCheckNonAggExprIsInGroupBy(pExpr, pGroupByList, pRootNode);

        } catch (SqlException& e) {
          delete pIter;
          throw;
        }
      }

      delete pIter;
    }

    if (bExit) {
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Now walk the graph, looking at the SqlOps
  GraphNav::bBoundedWalk(pStartNode->pFirstChild(), false, true, fVisitor, nullptr);
}



/*----------------------------------------------------------------------------*/
/* If there is no GROUP BY then validate the expressions of all sql operators */
/* that sit above the ProjectOp. Aggregate exprs may or may not be allowed    */
/* depending on the expressions in the ProjectOp.                             */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateSqlOpExprsAgainstProjectOp(PlanNode* pRootNode, QueryNode* pStartNode)
{
  UNUSED(pRootNode);

  // First find the ProjectOp
  auto fCondition = [](PlanNode *pNode, void *pvArg) -> b1 {
    UNUSED(pvArg);
    return (ProjectOp::bIsExactType(pNode));
  };

  QueryNode *pProjectOp = static_cast<QueryNode *>(
      GraphNav::pGlobalSearch(pStartNode, false, true,
                              fCondition, nullptr));

  if (!pProjectOp) {
    return;
  }

  // If the project expressions contain any columnRefs that are not part of
  // an aggregate then we cannot have any aggregate terms in the SqlOps
  // above it in the plan. So first determine if aggregations are allowed.
  b1 bAggregationAllowed = true;
  QueryNode* pNonAggColRef = nullptr;

  QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pProjectOp);
  PlanNodeIter* pIter = pProjectedColumns->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pProjectedColumn = _pQueryNode(pIter->pNext());
    QueryNode* pExpr = ProjectedColumn::pExprNode(pProjectedColumn);

    if (Expr::bContainsNonaggregatedColumnRefs(pExpr)) {
      bAggregationAllowed = false;
      pNonAggColRef = pExpr;
      break;
    }
  }

  delete pIter;

  // Now walk the graph until we hit the ProjectOp above and check the expressions
  // of any nodes we encounter.


  auto fVisitor = [pProjectOp, bAggregationAllowed, pNonAggColRef, pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    ASSERT(PlanObject::bIsOfType(pNode));
    QueryNode* pPlanOp = _pQueryNode(pNode);


    if (pNode == pProjectOp) {
      // We don't need to check ProjectOp. That already occurred in vValidateProjectExprs()
      return GraphNav::WALK_ABORT;
    }

    if (FilterOp::bIsOfType(pPlanOp)) {
      // Covers the HAVING clause
      QueryNode* pExpr = FilterOp::pFilterExpr(pPlanOp);
      b1 bHasAgg = Expr::bContainsAnAggregate(pExpr);
      if (!bAggregationAllowed) {
        if (bHasAgg) {
          // Throw an error that the first non-aggregated columnRef is not in the group by list
          s2K sBuf;
          Expr::vDescribe(pNonAggColRef, sBuf, pRootNode);
          SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                    " : %s"_Stz,
                                    sBuf.szStz());

        } else {
          // Throw an error that the expression is not in the group by list
          s2K sBuf;
          Expr::vDescribe(pExpr, sBuf, pRootNode);
          SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                    " : %s"_Stz,
                                    sBuf.szStz());
        }
      } else if (bAggregationAllowed && !bHasAgg) {
        // Throw an error that the expression is not in the group by list
        s2K sBuf;
        Expr::vDescribe(pExpr, sBuf, pRootNode);
        SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                  " : %s"_Stz,
                                  sBuf.szStz());
      }

    } else if (OrderOp::bIsOfType(pPlanOp)) {
      QueryNode* pOrderItemList = OrderOp::pOrderItemList(pPlanOp);
      ASSERT(OrderItemList::bIsOfType(pOrderItemList));

      PlanNodeIter* pIter = pOrderItemList->pNewChildrenIter();
      while (pIter->bHasNext()) {
        QueryNode* pOrderItem = _pQueryNode(pIter->pNext());
        ASSERT(OrderItem::bIsOfType(pOrderItem));

        QueryNode* pExpr = OrderItem::pExprNode(pOrderItem);

        b1 bHasAgg = Expr::bContainsAnAggregate(pExpr);

        if (!bAggregationAllowed && bHasAgg) {
          // Throw an error that the first non-aggregated columnRef is not in the group by list
          s2K sBuf;
          Expr::vDescribe(pNonAggColRef, sBuf, pRootNode);
          delete pIter;
          SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                    " : %s"_Stz,
                                    sBuf.szStz());

        } else if (bAggregationAllowed && !bHasAgg) {
          // Throw an error that the expression is not in the group by list
          s2K sBuf;
          Expr::vDescribe(pExpr, sBuf, pRootNode);
          delete pIter;
          SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                    " : %s"_Stz,
                                    sBuf.szStz());
        }
      }
      delete pIter;
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Now walk the graph, looking at the SqlOps
  GraphNav::bBoundedWalk(pStartNode->pFirstChild(), false, true, fVisitor, nullptr);
}


/*----------------------------------------------------------------------------*/
/* Validate that any expression subqueries are projecting the right number of */
/* columns for the operator they are used in.                                 */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateExprSubqueryProjections(PlanNode* pRootNode, QueryNode* pStartNode)
{

  auto fVisitor = [pRootNode, pStartNode]
                  (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!SubqueryRefExpr::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pSubqueryRefExpr = _pQueryNode(pNode);

    // Get the number of columns being projected by the subquery
    QueryNode* pExprSubquery = SubqueryRefExpr::pSubquery(pSubqueryRefExpr, pRootNode);
    ASSERT(pExprSubquery);
    ASSERT(ExprSubquery::bIsOfType(pExprSubquery));

    QueryNode* pSubqueryDefn = ExprSubquery::pCteDefn(pExprSubquery, pRootNode);
    ASSERT(pSubqueryDefn);
    ASSERT(CteDef::bIsOfType(pSubqueryDefn));

    i8 nColumns = CteDef::nColumns(pSubqueryDefn);
    ASSERT(nColumns > 0);

    // Get the parent operator
    QueryNode* pParentExpr = _pQueryNode(GraphNav::pGetParentOfPath(sPath, pStartNode));
    ASSERT(pParentExpr);

    if (ProjectedColumn::bIsOfType(pParentExpr)) {
      // We've got an expression subquery in a SELECT list.
      // Check that it is only projecting 1 column.
      if (nColumns != 1) {
        Stz* szSubquerySql = ExprSubquery::szSqlText(pExprSubquery);
        SemanticException::vThrow(EC_SUBQUERY_PROJ_TOO_MANY_COLS, ": %s"_Stz, szSubquerySql);
      }

    } else {
      // We've got an expression in some sort of predicate.
      ASSERT(Expr::bIsOfType(pParentExpr));

    if (!ExistsExpr::bIsOfType(pParentExpr)) {
        if (nColumns != 1) {
          Stz* szSubquerySql = ExprSubquery::szSqlText(pExprSubquery);
          SemanticException::vThrow(EC_SUBQUERY_PROJ_TOO_MANY_COLS, ": %s"_Stz, szSubquerySql);
        }
    }
    }


    return GraphNav::WALK_CONTINUE;
  };

  // Now walk the graph, looking at the SqlOps
  GraphNav::bBoundedWalk(pStartNode, true, true, fVisitor, nullptr);
}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
