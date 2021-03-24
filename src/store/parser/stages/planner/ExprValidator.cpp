/*============================================================================*/
/* ExprValidator.hpp - Validates that expressions are being specified and     */
/* used correctly                                                             */
/*============================================================================*/

#include "BASE.h"
#include "ExprValidator.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
#include "MetadataNode.hpp"
#include "ShadowRoot.hpp"
#include "SqlException.hpp"

#include "SeApi.hpp"
#include "InfoSet.hpp"
#include "TableMetadata.hpp"

#include "Expr.hpp"
#include "AggregateExpr.hpp"
#include "LiteralExpr.hpp"
#include "ColumnRef.hpp"
#include "ProjectedColumn.hpp"
#include "FilterOp.hpp"
#include "AggregateOp.hpp"


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
  QueryNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);

  // Validate that the result types of any expressions are not invalid.
  vValidateExprResultType(pRootNode, pPlanNode);

  // Validate that all predicate expressions have a boolean result type
  vValidatePredicatesAreBoolean(pRootNode, pPlanNode);

  // Validate any aggregate/group-by expressions
  vValidateGroupByExprs(pRootNode, pPlanNode);

  // Validate that all expressions that
  vValidateSqlOpExprsAgainstGroupBy(pRootNode, pPlanNode);

  // Validate that ProjectOps cannot produce both aggregate & non-aggregate
  // expressions without a group-by clause
  vValidateProjectExprs(pRootNode, pPlanNode);

}

/*----------------------------------------------------------------------------*/
/* Validate that the result types of any expressions are not invalid.         */
/* An invalid combination of exprs is one that returns CT_NONE for its type.  */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateExprResultType(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // Walk the graph looking for all expressions
  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!Expr::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pExprNode = static_cast<QueryNode*>(pNode);

    // This will throw an exception if an error is found.
    Expr::vValidateResultType(pExprNode, pRootNode);

    // No need to descend any further into the expression
    return GraphNav::WALK_SKIP_SUBTREE;
  };

  // Walk the /plan node, it's properties and its children
  GraphNav::bWalk(pPlanNode->pFirstChild(), true, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Validate that all predicate expressions (e.g. in filterOps) return a       */
/* boolean expression.                                                        */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidatePredicatesAreBoolean(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // Walk the graph looking for all filterOps
  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!FilterOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pFilterNode = static_cast<QueryNode*>(pNode);
    QueryNode* pExprNode = FilterOp::pFilterExpr(pFilterNode);

    eCT eType = Expr::eType(pExprNode, pRootNode);

    if (eType == CT_BOOLEAN) {
      // All good, keep looking for more filterOps
      return GraphNav::WALK_CONTINUE;
    }

    // Throw an error
    s2K sBuf;
    Expr::vDescribe(pExprNode, sBuf);

    SemanticException::vThrow(EC_EXPR_MUST_BE_BOOLEAN,
                              " : %s"_Stz,
                              sBuf.szStz());

    return GraphNav::WALK_ABORT; // Only required to silence a compiler warning
  };

  // Walk the /plan node and its children. We're not interested in walking
  // the properties because we are looking for filterOps
  GraphNav::bWalk(pPlanNode->pFirstChild(), false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate that aggregate and group-by expressions are correctly specified   */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateGroupByExprs(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // If there is group by list then an AggregateOp will be generated
  // rather than an ProjectOp for an early plan.
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    return AggregateOp::bIsExactType(pNode);
  };

  QueryNode* pAggregateOp = static_cast<QueryNode*>(
      GraphNav::pSearch(pPlanNode, false, true, fCondition, nullptr) );

  if (!pAggregateOp) {
    return;
  }

  // Get the list of projected expressions
  QueryNode* pProjectExprs = AggregateOp::pProjectedColumns(pAggregateOp);

  // Get the group by list
  QueryNode* pGroupByList = AggregateOp::pGroupByList(pAggregateOp);

  // Check that all non-aggregate expressions that are being projected
  // are either a part of the group by list or are dependent on the
  // expressions in the group by list
  // This is done in vValidateExprsAgainstGroupBy()

  // Check that the group by expressions cannot be aggregate expressions
  vCheckGroupByExprsNotAggregates(pGroupByList);

  // Check that the group by expressions are not literals.
  // TODO: Eventually we will support integer literals here to refer to column ordinals
  vCheckGroupByExprsNotLiterals(pGroupByList);
}

/*----------------------------------------------------------------------------*/
/* Validate that if there is no group by list then projections must either be */
/* all aggregates or all non-aggregates. This is applicable to an early plan. */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateProjectExprs(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // If there is no group by list then a simple ProjectOp will be generated
  // rather than an AggregateOp.
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    return ProjectOp::bIsExactType(pNode);
  };

  QueryNode* pProjectOp = static_cast<QueryNode*>(
      GraphNav::pSearch(pPlanNode, false, true, fCondition, nullptr) );

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
  PlanNodeList* pColExprList = PlanObject::pColumnExprs(static_cast<QueryNode*>(pProjectOp->pFirstChild()), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Let's count how many aggregate columns there are and compare to the number of children
  i8 iNumColumns = pProjectExprs->iNumChildren();
  i8 iNumAggCols = 0;
  i8 iNumLitCols = 0; // literal values only

  while (pIter->bHasNext()) {
    QueryNode* pProjExpr = static_cast<QueryNode*>(pIter->pNext());
    QueryNode* pExpr = ProjectedColumn::pExprNode(pProjExpr);

    // Convert the expression from a symbolic expression to an ordinal one.
    // This represents the work that this ProjectOp has to do.
    QueryNode* pOrdExpr = Expr::pSymbolicToOrdinal(pExpr, pColExprList, pRootNode);

    // Check if this ordinal expression is an aggregate
    if (Expr::bContainsAnAggregate(pOrdExpr)) {
      ++iNumAggCols;

    } else if (Expr::bIsLiteral(pOrdExpr)) {
      ++iNumLitCols;
    }

    delete pOrdExpr;
  }
  delete pIter;
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
void ExprValidator::vCheckNonAggExprIsInGroupBy(QueryNode* pExpr, QueryNode *pGroupByList)
{
  ASSERT(pExpr);

  PlanNodeIter* pGroupByIter = pGroupByList->pNewChildrenIter();

  // We will walk the expression tree and:
  //  - if the subExpr is an actual expression aggregate then skip that subtree
  //  - if the subExpr is a literal then skip that subtree
  //  - if the subExpr matches any of the group-by expressions then skip that subtree
  //  - if the subExpr is a ColumnRef then we have an error (because it is not in
  //    the groupby list)
  //  - otherwise continue walking/descending the expression graph

  auto fVisitor = [pGroupByList](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    ASSERT(Expr::bIsOfType(pNode));

    QueryNode* pExpr = static_cast<QueryNode*>(pNode);

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
      QueryNode* pGroupByExpr = static_cast<QueryNode*>(pIter->pNext());

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
      Expr::vDescribe(pExpr, sBuf);

      SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST, ": %s"_Stz, sBuf.szStz());
    }

    // Keep walking/descending the expression graph
    return GraphNav::WALK_CONTINUE;
  };

  // Walk the expression graph
  GraphNav::bWalk(pExpr, false, true, fVisitor, nullptr);
}


/*----------------------------------------------------------------------------*/
/* Check that the group by expressions cannot be aggregate expressions        */
/*----------------------------------------------------------------------------*/
void ExprValidator::vCheckGroupByExprsNotAggregates(QueryNode *pGroupByList)
{
  ASSERT(pGroupByList);
  ASSERT(pGroupByList->nChildren() > 0);

  PlanNodeIter* pIter = pGroupByList->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pExpr = static_cast<QueryNode*>(pIter->pNext());

    if (Expr::bContainsAnAggregate(pExpr)) {
      delete pIter;

      // Throw an error
      s2K sBuf;
      Expr::vDescribe(pExpr, sBuf);

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
void ExprValidator::vCheckGroupByExprsNotLiterals(QueryNode *pGroupByList)
{
  // TODO: Eventually we will support integer literals here to refer to column ordinals
  //       But for now we will just ban all literals

  ASSERT(pGroupByList);
  ASSERT(pGroupByList->nChildren() > 0);

  PlanNodeIter* pIter = pGroupByList->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pExpr = static_cast<QueryNode*>(pIter->pNext());

    if (Expr::bIsLiteral(pExpr)) {
      delete pIter;

      // Throw an error
      s2K sBuf;
      Expr::vDescribe(pExpr, sBuf);

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
void ExprValidator::vValidateSqlOpExprsAgainstGroupBy(PlanNode* pRootNode, QueryNode* pPlanNode)
{
  // First find the AggregateOp (if one exists) so we can get the group by list
  auto fCondition = [](PlanNode* pNode, void* pvArg)-> b1 {
    return (AggregateOp::bIsExactType(pNode));
  };

  QueryNode* pAggregateOp = static_cast<QueryNode*>(
      GraphNav::pSearch(pPlanNode, false, true,
                        fCondition, nullptr) );

  if (!pAggregateOp) {
    return;
  }

  // Get the group by list
  QueryNode* pGroupByList = AggregateOp::pGroupByList(pAggregateOp);

  // Now walk the plan again until we hit this AggregateOp and check that
  // all expressions are either aggregates or they use expressions from
  // the group by list.

  auto fVisitor = [pAggregateOp, pGroupByList](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    ASSERT(PlanObject::bIsOfType(pNode));
    QueryNode* pPlanOp = static_cast<QueryNode*>(pNode);

    b1 bExit = false;

    if (pNode == pAggregateOp) {
      // We've reach the last node
      bExit = true;
    }

    if (FilterOp::bIsOfType(pPlanOp)) {
      // Covers the HAVING clause
      QueryNode* pExpr = FilterOp::pFilterExpr(pPlanOp);
      vCheckNonAggExprIsInGroupBy(pExpr, pGroupByList);

    } else if (ProjectOp::bIsOfType(pPlanOp)) {
      // Covers both ProjectOp in the case of no group by, as well
      // as AggregateOp when a groupby does exit.
      QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pPlanOp);

      PlanNodeIter* pProjectedColumnIter = pProjectedColumns->pNewChildrenIter();
      while (pProjectedColumnIter->bHasNext()) {
        QueryNode* pProjectColumn = static_cast<QueryNode*>(pProjectedColumnIter->pNext());
        ASSERT(ProjectedColumn::bIsOfType(pProjectColumn));

        QueryNode* pExpr = ProjectedColumn::pExprNode(pProjectColumn);

        try {
          vCheckNonAggExprIsInGroupBy(pExpr, pGroupByList);

        } catch (SemanticException& e) {
          delete pProjectedColumnIter;
          throw;
        }
      }

      delete pProjectedColumnIter;
    }

//    } else if (OrderByOp::bIsOfType(pPlanOp)) {
//      QueryNode* pExpr = OrderByOp::pOrderExpr(pPlanOp);
//      vCheckNonAggExprIsInGroupBy(pExpr, pGroupByList);
//    }

    if (bExit) {
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Now walk the graph, looking at the SqlOps
  GraphNav::bWalk(pPlanNode->pFirstChild(), false, true, fVisitor, nullptr);
}



/*----------------------------------------------------------------------------*/
/* If there is no GROUP BY then validate the expressions of all sql operators */
/* that sit above the ProjectOp. Aggregate exprs may or may not be allowed    */
/* depending on the expressions in the ProjectOp.                             */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateSqlOpExprsAgainstProjectOp(PlanNode* pRootNode, QueryNode* pPlanNode)
{
  // First find the ProjectOp
  auto fCondition = [](PlanNode *pNode, void *pvArg) -> b1 {
    return (ProjectOp::bIsExactType(pNode));
  };

  QueryNode *pProjectOp = static_cast<QueryNode *>(
      GraphNav::pSearch(pPlanNode, false, true,
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
    QueryNode* pProjectedColumn = static_cast<QueryNode*>(pIter->pNext());
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


  auto fVisitor = [pProjectOp, bAggregationAllowed, pNonAggColRef]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    ASSERT(PlanObject::bIsOfType(pNode));
    QueryNode* pPlanOp = static_cast<QueryNode*>(pNode);


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
          Expr::vDescribe(pNonAggColRef, sBuf);
          SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                    " : %s"_Stz,
                                    sBuf.szStz());

        } else {
          // Throw an error that the expression is not in the group by list
          s2K sBuf;
          Expr::vDescribe(pExpr, sBuf);
          SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                    " : %s"_Stz,
                                    sBuf.szStz());
        }
      } else if (bAggregationAllowed && !bHasAgg) {
        // Throw an error that the expression is not in the group by list
        s2K sBuf;
        Expr::vDescribe(pExpr, sBuf);
        SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                  " : %s"_Stz,
                                  sBuf.szStz());
      }
    }

//    } else if (OrderByOp::bIsOfType(pPlanOp)) {
//      QueryNode* pExpr = OrderByOp::pOrderExpr(pPlanOp);
//      b1 bHasAgg = Expr::bContainsAnAggregate(pExpr);
//      if (!bAggregationAllowed && bHasAgg) {
//        // Throw an error that the first non-aggregated columnRef is not in the group by list
//          s2K sBuf;
//          Expr::vDescribe(pNonAggColRef, sBuf);
//          SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
//                                    " : %s"_Stz,
//                                    sBuf.szStz());
//      } else if (bAggregationAllowed && !bHasAgg) {
//        // Throw an error that the expression is not in the group by list
//          s2K sBuf;
//          Expr::vDescribe(pExpr, sBuf);
//          SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
//                                    " : %s"_Stz,
//                                    sBuf.szStz());
//      }
//    }

    return GraphNav::WALK_CONTINUE;
  };

  // Now walk the graph, looking at the SqlOps
  GraphNav::bWalk(pPlanNode->pFirstChild(), false, true, fVisitor, nullptr);
}


/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
