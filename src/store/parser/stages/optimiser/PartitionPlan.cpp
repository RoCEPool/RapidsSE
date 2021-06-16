/*============================================================================*/
/* PartitionPlan.hpp - Parallelise the plan by partitions                     */
/*============================================================================*/

#include "BASE.h"
#include "PartitionPlan.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "QueryNodeStack.hpp"

#include "GraphNav.hpp"
#include "GraphEditor.hpp"

#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"

#include "StatementMarker.hpp"
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

  QueryNodeStack* pStmtSectionStack = new QueryNodeStack;

  // Iterate each node, looking for a StatementMarker
  auto fVisitor = [pRootNode, pPlanRoot, pStmtSectionStack]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    QueryNode* pPartitionedSection = pPartitionStatementSection(pStatementMarker, pRootNode, pPlanRoot, pStmtSectionStack);
    pStmtSectionStack->vPush(pPartitionedSection);

    return GraphNav::WALK_CONTINUE;
  };

  // Iterate from the bottom up so that parent nodes can encapsulate child nodes and branches
  GraphNav::bGlobalWalkBottomUp(pPlanRoot->pFirstChild(), false, true, fVisitor, pRootNode);
  ASSERT(pStmtSectionStack->nItems() == 1);

  // Replace the logical plan with the partitioned plan
  GraphEditor::vReplaceChild(pPlanRoot, _pQueryNode(pPlanRoot->pFirstChild()), pStmtSectionStack->pPop());

  // TODO: Handle errors and cleanup allocated memory
  ASSERT(pStmtSectionStack->nItems() == 0);
  delete pStmtSectionStack;
}

/*----------------------------------------------------------------------------*/
/* Parallelises a statement marker section. Returns a new subgraph            */
/*----------------------------------------------------------------------------*/
QueryNode* PartitionPlan::pPartitionStatementSection(QueryNode* pStatementMarker,
                                                     PlanNode* pRootNode,
                                                     QueryNode* pPlanRoot,
                                                     QueryNodeStack* pStmtSectionStack)
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
  // Duplicate the node (without children).
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

  QueryNode*      pOriginChain = nullptr;
  QueryNode*      pNodeChain   = nullptr;
  QueryNodeStack* pPartitionStack = new QueryNodeStack();

  // A lambda that will modify/populate the pPartitionStack, pNodeChain and pOriginChain
  auto fVisitor = [pRootNode, pStmtSectionStack, &pOriginChain, &pNodeChain, pPartitionStack]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pCurNode = _pQueryNode(pNode);



    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bBoundedWalkBottomUp(pStatementMarker, false, true, fVisitor, nullptr);

  // Put together the partition-level, node-level and origin-level operators
  ASSERT(pOriginChain || pNodeChain || pPartitionStack->nItems() > 0);
  QueryNode* pPartitionChain = (pPartitionStack->nItems() > 0 ? pPartitionStack->pPop() : nullptr);

  QueryNode* pPartitionedSection = pConsolidateChains(pOriginChain, pNodeChain, pPartitionChain);

  // TODO: Handle errors and cleanup allocated memory
  ASSERT(pPartitionStack->nItems() == 0);
  delete pPartitionStack;
  return pPartitionedSection;
}

/*----------------------------------------------------------------------------*/
/* Consolidates separate plain chains from the partition, node & origin level */
/*----------------------------------------------------------------------------*/
QueryNode* PartitionPlan::pConsolidateChains(QueryNode* pOriginChain,
                                             QueryNode* pNodeChain,
                                             QueryNode* pPartitionChain)
{
  QueryNode* pChain;

  if (pOriginChain) {
    // We have origin-level operators and maybe node and partition-level operators too
    pChain = pOriginChain;

    if (pNodeChain)      { GraphEditor::vConnectChains(pNodeChain, pOriginChain);    }
    if (pPartitionChain) { GraphEditor::vConnectChains(pNodeChain, pPartitionChain); }

  } else if (pNodeChain) {
    // Only have node-partitioned operators and possibly partition-level operators
    pChain = pNodeChain;

    if (pPartitionChain) { GraphEditor::vConnectChains(pNodeChain, pPartitionChain); }

  } else {
    // Only have partition-level operators
    ASSERT(pPartitionChain);
    pChain = pPartitionChain;
  }

  return pChain;
}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
