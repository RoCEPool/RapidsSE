/*============================================================================*/
/* PartitionPlan.hpp - Parallelise the plan by partitions                     */
/*============================================================================*/

#include "BASE.h"
#include "PartitionPlan.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"
#include "ProjectedColumn.hpp"
#include "StringObject.hpp"
#include "ProjectOp.hpp"
#include "Expr.hpp"
#include "ColumnRef.hpp"
#include "ColumnOrdinal.hpp"
#include "ProjectOp.hpp"
#include "AggregateOp.hpp"

#include "SetOp.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Parallelise the plan into partitions                                       */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionPlan(PlanNode *pRootNode)
{
  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanRoot);
  ASSERT(ShadowPlan::bIsOfType(pPlanRoot));

  // Iterate each node, looking for a StatementMarker
  auto fVisitor = [pRootNode, pPlanRoot]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    vPartitionStatementSection(pStatementMarker, pRootNode, pPlanRoot);

    // We keep walking down each section because we will only push a ProjectOp
    // down one level at a time.
    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pPlanRoot->pFirstChild(), false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionStatementSection(QueryNode* pStatementMarker, PlanNode* pRootNode, QueryNode* pPlanRoot)
{
  // TODO: How effective is it to partition each statement section?
  //       StatementSections are used for separating completely different statements
  //       (e.g. SELECTS and INSERTS), as well as separating logically separate
  //       parts within the same statement (e.g. the child queries of a UNION).
  //
  // TODO: One possible way to partition the plan is to walk the statement section
  //       from the bottom up, make the necessary changes to the Sql operator and at
  //       the same time maintain a side graph that will be inserted at the top of
  //       the statement section for the fan-in from the partitions.
  //       Should we maintain separate side graphs for node-level fan-in separately
  //       from origin-level fan-in?
  //       Walking the graph from the bottom up will allow us to make some changes to
  //       the graph without affecting the walk (as the call stack is unwinding).
  //       e.g. we could snip the current node, but we wouldn't want to remove the
  //       parent node.


  // The canonical representation of a SELECT query will look a bit like this:
  //              ProjectOp            (post project)
  //              LimitOp              (LIMIT)
  //              OrderOp              (ORDER BY)
  //              FilterOp             (HAVING)
  //              AggregateOp          (select list, GROUP BY)
  //              JoinOp
  //    FilterOp        FilterOp       (WHERE)
  //    TableScanOp     TableScanOp

  // Walk the graph bottom up.
  // Duplicate the node.
  // If the node is a data source then set location to all partitions
  //   and push it onto a stack.
  // If the node is not a data source then pop a node from the stack,
  //   make that node a child of the current node and then push the current
  //   node back onto the stack.
  // If the node is a join or set operator then pop two nodes from the stack,
  //   make them children of the current node and then push the current node
  //   back onto the stack.
  // Meanwhile, maintain a chain of nodes that represent operators at the node
  //   and origin levels.
  // Finally, join all of the operators at the partition, node and origin levels together.
  //
  // Notes:
  // Not all statement sections will be parallelised with operators at all 3 levels.
  // e.g. a set operator (and everything above it) will always run on the origin only.

}


/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
