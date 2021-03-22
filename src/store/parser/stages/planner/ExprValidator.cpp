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
#include "ProjectedExpr.hpp"
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

  // Validate that ProjectOps cannot produce both aggregate & non-aggregate
  // expressions without a group-by clause
  vValidateAggsWithNoGroupBy(pRootNode, pPlanNode);

  // Validate any HAVING expressions
  vValidateHavingExprs(pRootNode, pPlanNode);
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
  GraphNav::bWalk(pPlanNode, true, true, fVisitor, pRootNode);
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
  GraphNav::bWalk(pPlanNode, false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate that aggregate and group-by expressions are correctly specified   */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateGroupByExprs(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // A lambda we use when walking the graph to check each Aggregate Op we encounter
  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

    if (!AggregateOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pAggNode = static_cast<QueryNode*>(pNode);

    // Get the list of projected expressions
    QueryNode* pProjectExprs = AggregateOp::pProjectedExprs(pAggNode);

    // Get the group by list
    QueryNode* pGroupByList = AggregateOp::pGroupByList(pAggNode);

    // Check that all non-aggregate expressions that are being projected
    // are either a part of the group by list or are dependent on the
    // expressions in the group by list
    vCheckNonAggExprsAreInGroupBy(pProjectExprs, pGroupByList);

    // Check that the group by expressions cannot be aggregate expressions
    vCheckGroupByExprsNotAggregates(pGroupByList);

    // Check that the group by expressions are not literals.
    // TODO: Eventually we will support integer literals here to refer to column ordinals
    vCheckGroupByExprsNotLiterals(pGroupByList);

    return GraphNav::WALK_CONTINUE;
  };

  // Walk the /plan node and its children. We're not interested in walking
  // the properties because we are looking for aggregateOps
  GraphNav::bWalk(pPlanNode, false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate that if there is no group by list then projections must either be */
/* all aggregates or all non-aggregates                                       */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateAggsWithNoGroupBy(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // If there is no group by list then a simple ProjectOp will be generated
  // rather than an AggregateOp.

  // A lambda we use when walking the graph to check each Project Op we encounter
  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

    if (!ProjectOp::bIsExactType(pNode)) {    // <-- note bIsExactType(), not bIsOfType()
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pProjectOpNode = static_cast<QueryNode*>(pNode);

    // Get the list of projected expressions
    QueryNode* pProjectExprs = ProjectOp::pProjectedExprs(pProjectOpNode);

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
    ASSERT(pNode->nChildren() == 1);
    PlanNodeList* pColExprList = PlanObject::pColumnExprs(static_cast<QueryNode*>(pNode->pFirstChild()), pRootNode);
    ASSERT(pColExprList);
    ASSERT(pColExprList->nItems() > 0);

    // Let's count how many aggregate columns there are and compare to the number of children
    i8 iNumColumns = pProjectExprs->iNumChildren();
    i8 iNumAggCols = 0;

    while (pIter->bHasNext()) {
      QueryNode* pProjExpr = static_cast<QueryNode*>(pIter->pNext());
      QueryNode* pExpr = ProjectedExpr::pExprNode(pProjExpr);

      // Convert the expression from a symbolic expression to an ordinal one.
      // This represents the work that this ProjectOp has to do.
      QueryNode* pOrdExpr = Expr::pSymbolicToOrdinal(pExpr, pColExprList, pRootNode);

      // Check if this ordinal expression is an aggregate
      if (Expr::bContainsAnAggregate(pOrdExpr)) {
        ++iNumAggCols;
      }

      delete pOrdExpr;
    }
    delete pIter;
    delete pColExprList;

    if (iNumAggCols == 0 || iNumAggCols == iNumColumns) {
      // We are projecting all aggregates or all non-aggregates.
      // So no problems here
      return GraphNav::WALK_CONTINUE;
    }

    // Throw an error
    SemanticException::vThrow(EC_EXPR_AGG_MIX_NO_GROUPBY_LIST);

    return GraphNav::WALK_ABORT; // This is purely to avoid a compiler warning
  };

  // Walk the /plan node and its children. We're not interested in walking
  // the properties because we are looking for ProjectOps
  GraphNav::bWalk(pPlanNode, false, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Check that all non-aggregate expressions that are being projected are      */
/* either a part of the group by list or are dependent on the expressions in  */
/* the group by list                                                          */
/*----------------------------------------------------------------------------*/
void ExprValidator::vCheckNonAggExprsAreInGroupBy(QueryNode* pProjectExprs, QueryNode *pGroupByList)
{
  ASSERT(pProjectExprs);
  ASSERT(pProjectExprs->nChildren() > 0);
  ASSERT(pGroupByList);

  PlanNodeIter* pProjectExprIter = pProjectExprs->pNewChildrenIter();
  while (pProjectExprIter->bHasNext()) {
    QueryNode* pProjectExpr = static_cast<QueryNode*>(pProjectExprIter->pNext());
    QueryNode* pExpr = ProjectedExpr::pExprNode(pProjectExpr);

    // If we have an aggregate expression then skip over it.
    // We're interested in non-aggregates instead.
    if (Expr::bContainsAnAggregate(pExpr)) {
      continue;
    }

    // Got a non-aggregate expression. Is this expr a superset of any
    // expressions in the group-by list?
    b1 bInGroupByList = false;
    PlanNodeIter* pGroupByIter = pGroupByList->pNewChildrenIter();
    while (pGroupByIter->bHasNext()) {
      QueryNode* pGroupExpr = static_cast<QueryNode*>(pGroupByIter->pNext());

      // Is the projected non-aggregate expression equal to or a superset
      // of this group-by expression?
      if (Expr::bIsEqualOrSupersetOf(pExpr, pGroupExpr)) {
        bInGroupByList = true;
        break;
      }
    }
    delete pGroupByIter;

    if (!bInGroupByList) {
      delete pProjectExprIter;

      // Throw an error
      s2K sBuf;
      Expr::vDescribe(pExpr, sBuf);

      SemanticException::vThrow(EC_EXPR_NOT_IN_GROUPBY_LIST,
                                " : %s"_Stz,
                                sBuf.szStz());
    }

  }
  delete pProjectExprIter;
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
/* Validate that any HAVING exprs are correctly specified                     */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateHavingExprs(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // TODO
  //  - expr must either be dependent on a column from the group by list or it must be an aggregate expression


}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
