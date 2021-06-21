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
#include "TableScanOp.hpp"
#include "AggregateOp.hpp"
#include "ExplainOp.hpp"
#include "FilterOp.hpp"
#include "LimitOp.hpp"
#include "OrderOp.hpp"
#include "ProjectOp.hpp"
#include "UnionOp.hpp"

#include "Expr.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Private Structures/Classes-----------------------------------------------*/
/*----------------------------------------------------------------------------*/

namespace Parser
{
  // An object that holds temporary chains of graphs that are partitioned at
  // different levels before they are all joined together.
  class GraphChains
  {
  public:
    QueryNode *m_pOriginChain = nullptr;
    QueryNode *m_pNodeChain = nullptr;
    QueryNodeStack* m_pPartitionChainStack;

    GraphChains() : m_pOriginChain(nullptr), m_pNodeChain(nullptr), m_pPartitionChainStack(new QueryNodeStack)
    {}

    ~GraphChains()
    {
      QueryNode *pChain;
      while ((pChain = m_pPartitionChainStack->pPop()) != nullptr) {
        delete pChain;
      }

      delete m_pPartitionChainStack;
    }
  };
}


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
  RASSERT(pStmtSectionStack->nItems() == 1);

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

  GraphChains xChains;

  // A lambda that will modify/populate the pPartitionChainStack, pNodeChain and pOriginChain
  auto fVisitor = [pStmtSectionStack, &xChains]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pCurNode = _pQueryNode(pNode);

    // TODO: Looking at any one node, how do you know which chain to add it to?
    //       e.g. how do you tell the difference between a post-project op that has
    //       no aggregates from a regular projectOp within the plan?
    //       Do we need to keep track of some sort of state?

    // TODO: If we encounter certain operators that we know will have a statementMarker as children
    //       then we must join the operator with the chain we pop off pStmtSectionStack.
    //       e.g. SetOp, ExplainOp.

    if  (StatementMarker::bIsOfType(pNode) )    { vPartitionStatementMarker(pCurNode, &xChains, pStmtSectionStack); }
    else if (TableScanOp::bIsExactType(pNode) ) { vPartitionTableScanOp(    pCurNode, &xChains, pStmtSectionStack); }
//  else if (IndexScanOp::bIsExactType(pNode) ) { vPartitionIndexScanOp(    pCurNode, &xChains, pStmtSectionStack); }
    else if (AggregateOp::bIsExactType(pNode) ) { vPartitionAggregateOp(    pCurNode, &xChains, pStmtSectionStack); }
    else if (  ExplainOp::bIsExactType(pNode) ) { vPartitionExplainOp(      pCurNode, &xChains, pStmtSectionStack); }
    else if (   FilterOp::bIsExactType(pNode) ) { vPartitionFilterOp(       pCurNode, &xChains, pStmtSectionStack); }
    else if (    LimitOp::bIsExactType(pNode) ) { vPartitionLimitOp(        pCurNode, &xChains, pStmtSectionStack); }
    else if (    OrderOp::bIsExactType(pNode) ) { vPartitionOrderOp(        pCurNode, &xChains, pStmtSectionStack); }
    else if (  ProjectOp::bIsExactType(pNode) ) { vPartitionProjectOp(      pCurNode, &xChains, pStmtSectionStack); }
    else if (    UnionOp::bIsExactType(pNode) ) { vPartitionUnionOp(        pCurNode, &xChains, pStmtSectionStack); }
    else                                        { NEVER_REACH }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk each node in this section from the bottom up
  GraphNav::bBoundedWalkBottomUp(pStatementMarker, false, true, fVisitor, nullptr);

  // Put together the partition-level, node-level and origin-level operators
  ASSERT(xChains.m_pOriginChain || xChains.m_pNodeChain || xChains.m_pPartitionChainStack->nItems() > 0);
  QueryNode* pPartitionChain = (xChains.m_pPartitionChainStack->nItems() > 0 ? xChains.m_pPartitionChainStack->pPop() : nullptr);

  QueryNode* pPartitionedSection = pConsolidateChains(xChains.m_pOriginChain, xChains.m_pNodeChain, pPartitionChain);

  // TODO: Handle errors and cleanup any allocated memory
  ASSERT(xChains.m_pPartitionChainStack->nItems() == 0);
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

    if (pNodeChain)      { GraphEditor::vConnectChains(pChain, pNodeChain);    }
    if (pPartitionChain) { GraphEditor::vConnectChains(pChain, pPartitionChain); }

  } else if (pNodeChain) {
    // Only have node-partitioned operators and possibly partition-level operators
    pChain = pNodeChain;

    if (pPartitionChain) { GraphEditor::vConnectChains(pChain, pPartitionChain); }

  } else {
    // Only have partition-level operators
    ASSERT(pPartitionChain);
    pChain = pPartitionChain;
  }

  return pChain;
}

/*============================================================================*/

/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a StatementMarker                              */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionStatementMarker(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(StatementMarker::bIsOfType(pNode));

  QueryNode* pStmtMarker = pNode->pDeepCopy(true, nullptr);
  vPushOnOriginChain(pStmtMarker, pChains);
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a TableScanOp                                  */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionTableScanOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(TableScanOp::bIsExactType(pNode));

  QueryNode* pTableScanNode = pNode->pDeepCopy(true);

  // TableScanOps always have a location of all partitions.
  // TODO: What about replicated tables?
  PlanObject::vSetLocation(pTableScanNode, Parser::Location::LO_ALL_PARTITIONS);

  // Since TableScanOps are always a leaf node, we push them onto the
  // partitionChainStack instead of pushing them onto an existing chain.
  pChains->m_pPartitionChainStack->vPush(pTableScanNode);
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a IndexScanOp                                  */
/*----------------------------------------------------------------------------*/
//void PartitionPlan::vPartitionIndexScanOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
//{
//  ASSERT(IndexScanOp::bIsExactType(pNode));
//
//}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a AggregateOp                                  */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionAggregateOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(AggregateOp::bIsExactType(pNode));

  // AggregateOps are not post-projects and they must have an aggregation.
  // Therefore we need another AggregateOp at the origin to combine each
  // of the groups that are emitted from the partition/node levels.

  // Need to add a consolidating AggregateOp to the origin.
  QueryNode* pConsolidatingAggregateNode = ProjectOp::pCreateConsolidating(pNode);
  vPushOnOriginChain(pConsolidatingAggregateNode, pChains);

  // Modify any necessary aggregate terms in this AggregateOp and
  // add it to the partition chain
  QueryNode* pPartitionedAggregateNode = AggregateOp::pCreatePartitioned(pNode);
  ASSERT(AggregateOp::bIsExactType(pPartitionedAggregateNode));

  vPushOnPartitionChain(pPartitionedAggregateNode, pChains);
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a ExplainOp                                    */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionExplainOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(ExplainOp::bIsExactType(pNode));

  QueryNode* pExplainNode = pNode->pDeepCopy(true, nullptr);

  // ExplainOps occur in their own StatementMarker section. There must already
  // be another chain in the pStmtSectionStack that we need to join up to this one.
  ASSERT(pStmtSectionStack->nItems() > 0);
  pExplainNode->vAppendChild(pStmtSectionStack->pPop());

  vPushOnOriginChain(pExplainNode, pChains);
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a FilterOp                                     */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionFilterOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(FilterOp::bIsExactType(pNode));

  QueryNode* pExpr = FilterOp::pFilterExpr(pNode);
  b1 bHasAnAggregate = Expr::bContainsAnAggregate(pExpr);

  if (bHasAnAggregate) {
    // This represents a HAVING clause. This has to run on the origin, after
    // all partitioned aggregations have been combined.
    // We need to convert the expression to be a consolidated aggregate expression.
    QueryNode* pNewExpr = pExpr->pDeepCopy(true);
    Expr::vConsolidateAggregates(pNewExpr);
    QueryNode* pConsolidatedFilter = FilterOp::pCreate(pNewExpr);
    vPushOnOriginChain(pConsolidatedFilter, pChains);

  } else {
    // No aggregate, therefore it is a WHERE clause. These can run on each partition.
    QueryNode* pFilterNode = pNode->pDeepCopy(true, nullptr);
    vPushOnPartitionChain(pFilterNode, pChains);
  }
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a LimitOp                                      */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionLimitOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(LimitOp::bIsExactType(pNode));
  NOT_IMPLEMENTED_YET
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a OrderOp                                      */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionOrderOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(OrderOp::bIsExactType(pNode));
  NOT_IMPLEMENTED_YET
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a ProjectOp                                    */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionProjectOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(ProjectOp::bIsExactType(pNode));

  b1 bHasAnAggregate = ProjectOp::bHasAnAggregate(pNode);
  b1 bIsPostProject = PartitionPlan::bIsPostProject(pNode);

  if (bIsPostProject) {

    if (bHasAnAggregate) {
      // Post project with aggregates. Need to consolidate the aggregates before placing
      // on the origin chain.
      QueryNode* pConsolidatingProjectNode = ProjectOp::pCreateConsolidating(pNode);
      vPushOnOriginChain(pConsolidatingProjectNode, pChains);

    } else {
      // Post project with no aggregates. Therefore no need to change any
      // of the expressions. Simply place this on the origin chain.
      vPushOnOriginChain(pNode->pDeepCopy(true, nullptr), pChains);
    }

  } else {
    // Not a post-project

    // Only non-post-projects with aggregates need special handling here.
    if (bHasAnAggregate) {
      // Must only contain (grand total) aggregates.
      ASSERT(ProjectOp::bHasOnlyAggregates(pNode));

      // Need to add a consolidating (super-aggregating) projectOp to the origin.
      QueryNode* pConsolidatingProjectNode = ProjectOp::pCreateConsolidating(pNode);
      vPushOnOriginChain(pConsolidatingProjectNode, pChains);

      // Modify any necessary aggregate terms in this ProjectOp and
      // add it to the partition chain
      QueryNode* pPartitionedProjectNode = ProjectOp::pCreatePartitioned(pNode);
      vPushOnPartitionChain(pPartitionedProjectNode, pChains);

    } else {
      // Non-post-project without any aggregates
      QueryNode* pProjectNode = pNode->pDeepCopy(true, nullptr);
      vPushOnPartitionChain(pProjectNode, pChains);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a UnionOp                                      */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionUnionOp(QueryNode* pNode, GraphChains* pChains, QueryNodeStack* pStmtSectionStack)
{
  ASSERT(UnionOp::bIsExactType(pNode));
  NOT_IMPLEMENTED_YET
}


/*----------------------------------------------------------------------------*/
/* Returns true if this projectOp is occurring after an OrderOp, LimitOp or   */
/* a HAVING clause (a filterOp with aggregates)                               */
/*----------------------------------------------------------------------------*/
b1 PartitionPlan::bIsPostProject(QueryNode* pProjectOp)
{
  ASSERT(ProjectOp::bIsExactType(pProjectOp));

  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    // A projectOp occurring above a LimitOp or OrderOp of an unpartitioned plan
    // must be a post-project
    if (LimitOp::bIsOfType(pNode)) return true;
    if (OrderOp::bIsOfType(pNode)) return true;

    // A projectOp occurring above a FilterOp that contains aggregates must
    // be a post-project
    if (FilterOp::bIsOfType(pNode) &&
        Expr::bContainsAnAggregate(FilterOp::pFilterExpr(_pQueryNode(pNode)))) return true;

    // A projectOp occurring above another ProjectOp or AggregateOp
    // must be a post-project
    if (ProjectOp::bIsOfType(pNode)) return true;

    return false;
  };

  PlanNode* pItem = GraphNav::pBoundedSearch(pProjectOp->pFirstChild(), false, true, fCondition, nullptr);

  return (pItem != nullptr);
}


/*----------------------------------------------------------------------------*/
/* Push a node onto an existing partition chain                               */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPushOnPartitionChain(QueryNode* pNode, GraphChains* pChains)
{
  ASSERT(pNode);
  ASSERT(pChains);
  ASSERT(pChains->m_pPartitionChainStack->nItems() > 0);

  QueryNode* pPartitionChain = pChains->m_pPartitionChainStack->pPop();
  pNode->vAppendChild(pPartitionChain);
  pChains->m_pPartitionChainStack->vPush(pNode);
}

/*----------------------------------------------------------------------------*/
/* Push a node onto the node chain                                            */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPushOnNodeChain(QueryNode* pNode, GraphChains* pChains)
{
  ASSERT(pNode);
  ASSERT(pChains);

  if (pChains->m_pNodeChain) {
    pNode->vAppendChild(pChains->m_pNodeChain);

  } else {
    // First one in this chain, so set the location
    PlanObject::vSetLocation(pNode, Location::LO_ALL_NODES);
  }

  pChains->m_pNodeChain = pNode;
}

/*----------------------------------------------------------------------------*/
/* Push a node onto the origin chain                                          */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPushOnOriginChain(QueryNode* pNode, GraphChains* pChains)
{
  ASSERT(pNode);
  ASSERT(pChains);

  if (pChains->m_pOriginChain) {
    pNode->vAppendChild(pChains->m_pOriginChain);

  } else {
    // First one in this chain, so set the location
    PlanObject::vSetLocation(pNode, Location::LO_ORIGIN);
  }

  pChains->m_pOriginChain = pNode;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
