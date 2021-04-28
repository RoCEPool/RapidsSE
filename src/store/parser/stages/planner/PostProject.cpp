/*============================================================================*/
/* PostProject.hpp - Ensures only selected columns are returned in a resultset*/
/*============================================================================*/

#include "BASE.h"
#include "PostProject.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"
#include "ProjectOp.hpp"
#include "FilterOp.hpp"
#include "SetOp.hpp"
#include "OrderOp.hpp"
#include "OrderItem.hpp"
#include "Expr.hpp"
#include "ProjectedColumns.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Private classes/structs -------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// A container for recording certain operators when we are walking the graph
class OpContainer {
public:
  QueryNode* m_pProjectOp;
  QueryNode* m_pFilterOp;
  QueryNode* m_pOrderOp;

  OpContainer() {
    m_pProjectOp = nullptr;
    m_pFilterOp = nullptr;
    m_pOrderOp = nullptr;
  }
};


/*----------------------------------------------------------------------------*/
/* Adds a ProjectOp to the plan if it is required to trim the projected       */
/* columns to only those that the user asked for                              */
/*----------------------------------------------------------------------------*/
void PostProject::vCheckPostProject(PlanNode *pRootNode)
{
  PlanNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  // Walk each node and look for the StatementMarkers
  GraphNav::bGlobalWalk(pPlanNode, false, true, eStmtMarkerVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Visits all nodes, looking for StatementMarkers                             */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl PostProject::eStmtMarkerVisitor(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
{
  UNUSED(sPath);
  UNUSED(iDepth);

  if (!StatementMarker::bIsOfType(pNode)) {
    return GraphNav::WALK_CONTINUE;
  }

  PlanNode* pRootNode = _pRootNode(pvArg);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  QueryNode* pStatementMarker = _pQueryNode(pNode);

  // We've got a StatementMarker section but now we need to determine if we
  // need to add a ProjectOp at the top.

  // If there is a SetOp in this section then ignore it completely
  auto fHasSetOp = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);
    return SetOp::bIsOfType(pNode);
  };

  PlanNode* pSetOp = GraphNav::pBoundedSearch(pStatementMarker, false, true, fHasSetOp, nullptr);
  if (pSetOp) {
    return GraphNav::WALK_CONTINUE;
  }

  // There is no SetOp in this plan

  // Walk each node of this current statement section and look for OrderOps
  // and FilterOps that sit above a ProjectOp/AggregateOp in the plan.
  // Record them into a struct.
  OpContainer xContainer;
  GraphNav::bBoundedWalk(pStatementMarker, false, true, eStmtSectionVisitor, &xContainer);

  if (!xContainer.m_pProjectOp) {
    // No project, no worries
    return GraphNav::WALK_CONTINUE;
  }

  if (!xContainer.m_pFilterOp && !xContainer.m_pOrderOp) {
    // No potentially troublesome operators sitting above a projection.
    // So nothing for us to worry about here.
    return GraphNav::WALK_CONTINUE;
  }

  // There are operators with expressions that are sitting above a reprojection
  // in the plan. We need to check if the expressions in these operators are
  // expressions that are not present in the projection under them. If so
  // we need to add another ProjectOp above these sql operators to ensure that
  // all intermediate or working columns are filtered out.
  vAddPostProject(&xContainer, pStatementMarker, pRootNode);

  return GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* Visits all nodes within a StatementMarker subsection and record those      */
/* operators that contain expressions and sit above a project/aggregate op    */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl PostProject::eStmtSectionVisitor(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
{
  UNUSED(sPath);
  UNUSED(iDepth);

  OpContainer* pContainer = static_cast<OpContainer*>(pvArg);
  ASSERT(pContainer);

  if (FilterOp::bIsOfType(pNode)) {
    ASSERT(!pContainer->m_pFilterOp);
    pContainer->m_pFilterOp = _pQueryNode(pNode);

  } else if (OrderOp::bIsOfType(pNode)) {
    ASSERT(!pContainer->m_pOrderOp);
    pContainer->m_pOrderOp = _pQueryNode(pNode);

  } else if (ProjectOp::bIsOfType(pNode)) {
    ASSERT(!pContainer->m_pProjectOp);
    pContainer->m_pProjectOp = _pQueryNode(pNode);
    return GraphNav::WALK_SKIP_SUBTREE;
  }

  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* Checks if there are any expressions in sql operators that are in excess    */
/* of the project list and adds a post project if required                    */
/*----------------------------------------------------------------------------*/
void PostProject::vAddPostProject(void* pvContainer, QueryNode* pStatementMarker, PlanNode* pRootNode)
{
  // This is ugly with OpContainer being passed as a void*, but I don't want
  // to expose the OpContainer class outside of this file, so I can't list it
  // in the header file. Either I pass it as a void* or I make this a free
  // (non-class) function, in which case the definition of this function would
  // not be in the header file.

  OpContainer* pContainer = static_cast<OpContainer*>(pvContainer);
  ASSERT(pContainer);
  ASSERT(pContainer->m_pProjectOp);
  ASSERT(pStatementMarker);
  ASSERT(pRootNode);

  b1 bPostProjectRequired = false;

  // Get the list of columns that should be projected
  QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pContainer->m_pProjectOp);

  // Check if there are any expressions in the filter or order ops that
  // are not equal to any of the expressions in the project list.
  if (pContainer->m_pFilterOp) {
    // Get the expr
    QueryNode* pFilterExpr = FilterOp::pFilterExpr(pContainer->m_pFilterOp);
    ASSERT(pFilterExpr);

    // Check if the expr is equal to or a superset of one of them in the project list.
    // We also need to consider if the filter expression is an aggregate where the
    // projected column is not.
    if (!ProjectedColumns::bHasExprEqualOrSuperset(pProjectedColumns, pFilterExpr, false)) {
      bPostProjectRequired = true;
    }
  }

  if (!bPostProjectRequired && pContainer->m_pOrderOp) {
    // Iterate the ordering expressions
    QueryNode* pOrderItemList = OrderOp::pOrderItemList(pContainer->m_pOrderOp);
    PlanNodeIter* pIter = pOrderItemList->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pOrderItem = _pQueryNode(pIter->pNext());
      ASSERT(OrderItem::bIsOfType(pOrderItem));

      QueryNode* pExpr = OrderItem::pExprNode(pOrderItem);
      ASSERT(Expr::bIsOfType(pExpr));

      if (!ProjectedColumns::bHasExprEqualOrSuperset(pProjectedColumns, pExpr, false)) {
        bPostProjectRequired = true;
      }
    }

    delete pIter;
  }

  if (bPostProjectRequired) {
    // Copy the select list
    QueryNodeList* pSelectList = new QueryNodeList();
    PlanNodeIter* pColumnIter = pProjectedColumns->pNewChildrenIter();

    while (pColumnIter->bHasNext()) {
      QueryNode* pProjectedColumn = _pQueryNode(pColumnIter->pNext());
      QueryNode* pColumnCopy = pProjectedColumn->pDeepCopy(true);
      pSelectList->vAppend(pColumnCopy);
    }

    delete pColumnIter;

    // Create the new post project
    QueryNode* pPostProject = ProjectOp::pCreate(pSelectList);
    ASSERT(pSelectList->nItems() == 0);
    delete pSelectList;

    // Add in the post project
    pStatementMarker->vInsertAsParentOfChildren(pPostProject);
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
