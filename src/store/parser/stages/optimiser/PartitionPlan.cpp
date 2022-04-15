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
#include "SubqueryMarker.hpp"
#include "MainSubquery.hpp"
#include "TableScanOp.hpp"
#include "SubqueryRefOp.hpp"
#include "AggregateOp.hpp"
#include "ExplainOp.hpp"
#include "FilterOp.hpp"
#include "LimitOp.hpp"
#include "OrderOp.hpp"
#include "ProjectOp.hpp"
#include "UnionOp.hpp"
#include "JoinOp.hpp"

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
    QueryNodeStack *m_pOriginChainStack = nullptr;
    QueryNode *m_pNodeChain = nullptr;
    QueryNodeStack* m_pPartitionChainStack;

    GraphChains() : m_pOriginChainStack(new QueryNodeStack),
                    m_pNodeChain(nullptr),
                    m_pPartitionChainStack(new QueryNodeStack)
    {}

    ~GraphChains()
    {
      QueryNode *pChain;

      while ((pChain = m_pOriginChainStack->pPop()) != nullptr) {
        delete pChain;
      }

      while ((pChain = m_pPartitionChainStack->pPop()) != nullptr) {
        delete pChain;
      }

      delete m_pOriginChainStack;
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


  // Iterate each node, looking for a MainSubquery marker
  auto fVisitor = [pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!MainSubquery::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pSubqueryMarker = _pQueryNode(pNode);
    vPartitionMainSubquery(pSubqueryMarker, pRootNode);

    return GraphNav::WALK_CONTINUE;
  };

  // Iterate from the bottom up so that parent nodes can encapsulate child nodes and branches
  GraphNav::bGlobalWalkBottomUp(pPlanRoot, false, true, fVisitor, nullptr);
}

/*----------------------------------------------------------------------------*/
/* Parallelises a SubqueryMarker section.                                     */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionMainSubquery(QueryNode* pMainSubquery,
                                           PlanNode*  pRootNode)
{
  ASSERT(pMainSubquery);
  ASSERT(MainSubquery::bIsOfType(pMainSubquery));
  ASSERT(pMainSubquery->nChildren() == 1);

  QueryNodeStack* pStmtSectionStack = new QueryNodeStack;

  // Walk this subquery, looking for a StatementMarker. We need to only walk
  // the current subquery. We do this by defining a callback lambda for the
  // top-down entry of a node and the bottom up callback. The top-down entry
  // will allow us to skip subtrees if we encounter another subqueryMarker.

  auto fTopDownVisitor =
      [] (PlanNode *pNode, s2K &sPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(eEvent);
    UNUSED(pvArg);

    if (SubqueryMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    return GraphNav::WALK_CONTINUE;
  };

  auto fBottomUpVisitor = [pRootNode, pStmtSectionStack]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(eEvent);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    QueryNode* pPartitionedSection = pPartitionStatementSection(pStatementMarker, pRootNode, pStmtSectionStack);
    pStmtSectionStack->vPush(pPartitionedSection);

    return GraphNav::WALK_CONTINUE;
  };

  // We use a top-down walk but the main action occurs in the "exit" event, so
  // it behaves like a bottom-up walk from the callback's perspective.
  GraphNav::bGlobalWalk(pMainSubquery->pFirstChild(),
                        false,
                        true,
                        fTopDownVisitor,
                        fBottomUpVisitor,
                        nullptr);

  while (pStmtSectionStack->nItems() > 1) {
    QueryNode* pSection1 = pStmtSectionStack->pPop();
    QueryNode* pSection2 = pStmtSectionStack->pPop();

    GraphEditor::vConnectChains(pSection1, pSection2);
    pStmtSectionStack->vPush(pSection1);
  }

  RASSERT(pStmtSectionStack->nItems() == 1);

  // Replace this subquery's logical plan with the partitioned plan
  GraphEditor::vReplaceChild(pMainSubquery, _pQueryNode(pMainSubquery->pFirstChild()), pStmtSectionStack->pPop());

  ASSERT(pStmtSectionStack->nItems() == 0);
  delete pStmtSectionStack;
}

/*----------------------------------------------------------------------------*/
/* Parallelises a statement marker section. Returns a new subgraph            */
/*----------------------------------------------------------------------------*/
QueryNode* PartitionPlan::pPartitionStatementSection(QueryNode* pStatementMarker,
                                                     PlanNode* pRootNode,
                                                     QueryNodeStack* pStmtSectionStack)
{
  UNUSED(pRootNode);

  // The canonical representation of a SELECT query will look a bit like this:
  //              ProjectOp            (post project)
  //              LimitOp              (LIMIT)
  //              OrderOp              (ORDER BY)
  //              FilterOp             (HAVING)
  //              AggregateOp          (select list, GROUP BY)
  //              JoinOp
  //    FilterOp        FilterOp       (WHERE)
  //    TableScanOp     TableScanOp
  //
  //
  // The general approach taken here is:
  //   - Walk the statement section graph from the bottom up.
  //   - Visit each sql operator and push it onto one or more of 3 distinct
  //     chains for each level of parallelism (partition, node, origin).
  //     Some nodes may need to have their expressions modified to be super-aggregates
  //     before doing so.
  //   - Combine the partition, node and origin chains together and push this combined
  //     chain onto a chain stack.
  //   - Some sql operators (e.g. ExplainOp and SetOps) have whole statement sections
  //     as children and they can pop these chains off the chain stack and append them,
  //     knowing that the child chains are fully parallelised.
  //   - At the end there should only be one chain on the chain stack.

  GraphChains xChains;

  // A lambda that will modify/populate the pPartitionChainStack, pNodeChain and pOriginChain
  auto fVisitor = [pStmtSectionStack, &xChains, pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    QueryNode* pCurNode = _pQueryNode(pNode);

    if  (  StatementMarker::bIsOfType(pNode) )    { vPartitionStatementMarker(pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (  TableScanOp::bIsExactType(pNode) ) { vPartitionTableScanOp(    pCurNode, &xChains, pStmtSectionStack, pRootNode); }
//  else if (  IndexScanOp::bIsExactType(pNode) ) { vPartitionIndexScanOp(    pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (SubqueryRefOp::bIsExactType(pNode) ) { vPartitionSubqueryRefOp(  pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (  AggregateOp::bIsExactType(pNode) ) { vPartitionAggregateOp(    pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (    ExplainOp::bIsExactType(pNode) ) { vPartitionExplainOp(      pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (     FilterOp::bIsExactType(pNode) ) { vPartitionFilterOp(       pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (      LimitOp::bIsExactType(pNode) ) { vPartitionLimitOp(        pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (      OrderOp::bIsExactType(pNode) ) { vPartitionOrderOp(        pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (    ProjectOp::bIsExactType(pNode) ) { vPartitionProjectOp(      pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (      UnionOp::bIsExactType(pNode) ) { vPartitionUnionOp(        pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else if (       JoinOp::bIsOfType(pNode) )    { vPartitionJoinOp(         pCurNode, &xChains, pStmtSectionStack, pRootNode); }
    else                                          { NEVER_REACH }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk each node in this section from the bottom up
  GraphNav::bBoundedWalkBottomUp(pStatementMarker, false, true, fVisitor, nullptr);

  // Put together the partition-level, node-level and origin-level operators
  ASSERT(xChains.m_pOriginChainStack->nItems() > 0 ||
                  xChains.m_pNodeChain ||
                  xChains.m_pPartitionChainStack->nItems() > 0);

  QueryNode* pOriginChain    = (xChains.m_pOriginChainStack->nItems()    > 0 ? xChains.m_pOriginChainStack->pPop()    : nullptr);
  QueryNode* pPartitionChain = (xChains.m_pPartitionChainStack->nItems() > 0 ? xChains.m_pPartitionChainStack->pPop() : nullptr);

  QueryNode* pPartitionedSection = pConsolidateChains(pOriginChain, xChains.m_pNodeChain, pPartitionChain);

  ASSERT(xChains.m_pOriginChainStack->nItems() == 0);
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
void PartitionPlan::vPartitionStatementMarker(QueryNode* pNode, GraphChains* pChains,
                                              QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  UNUSED(pRootNode);
  ASSERT(StatementMarker::bIsOfType(pNode));

  QueryNode* pStmtMarker = pNode->pDeepCopy(true, nullptr);

  if (pChains->m_pOriginChainStack->nItems() > 0) {
    vPushOnOriginChain(pStmtMarker, pChains);

  } else if (pChains->m_pNodeChain) {
    vPushOnNodeChain(pStmtMarker, pChains);

  } else {
    vPushOnPartitionChain(pStmtMarker, pChains);
  }

}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a TableScanOp                                  */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionTableScanOp(QueryNode* pNode, GraphChains* pChains,
                                          QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  ASSERT(TableScanOp::bIsExactType(pNode));


  b1 bIsPartitionable = PlanObject::bIsPartitionable(pNode, pRootNode);
  if (bIsPartitionable) {
    QueryNode* pTableScanNode = pNode->pDeepCopy(true);

    // Until we support replicated tables, TableScanOps always have
    // a location of all partitions.
    PlanObject::vSetLocation(pTableScanNode, Parser::Location::LO_ALL_PARTITIONS);

    // Since TableScanOps are always a leaf node, we push them onto the
    // partitionChainStack instead of pushing them onto an existing chain.
    pChains->m_pPartitionChainStack->vPush(pTableScanNode);

  } else {
    // We don't support replicated tables yet
    NEVER_REACH

    QueryNode* pTableScanNode = pNode->pDeepCopy(true, nullptr);
    PlanObject::vSetLocation(pTableScanNode, Parser::Location::LO_ORIGIN);
    pChains->m_pOriginChainStack->vPush(pTableScanNode);
  }
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a IndexScanOp                                  */
/*----------------------------------------------------------------------------*/
//void PartitionPlan::vPartitionIndexScanOp(QueryNode* pNode, GraphChains* pChains,
//                                          QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
//{
//  ASSERT(IndexScanOp::bIsExactType(pNode));
//
//}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a TableScanOp                                  */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionSubqueryRefOp(QueryNode* pNode, GraphChains* pChains,
                                            QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  ASSERT(SubqueryRefOp::bIsExactType(pNode));

  QueryNode* pSubqueryRefOp = pNode->pDeepCopy(true, nullptr);

  b1 bIsPartitionable = PlanObject::bIsPartitionable(pNode, pRootNode);

  if (bIsPartitionable) {
    PlanObject::vSetLocation(pSubqueryRefOp, Parser::Location::LO_ALL_PARTITIONS);
    pChains->m_pPartitionChainStack->vPush(pSubqueryRefOp);

  } else {
    PlanObject::vSetLocation(pSubqueryRefOp, Parser::Location::LO_ORIGIN);
    pChains->m_pOriginChainStack->vPush(pSubqueryRefOp);
  }
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a AggregateOp                                  */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionAggregateOp(QueryNode* pNode, GraphChains* pChains,
                                          QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  ASSERT(AggregateOp::bIsExactType(pNode));

  b1 bIsPartitionable = PlanObject::bIsPartitionable(pNode, pRootNode);
  if (bIsPartitionable) {

    // AggregateOps are not post-projects and they must have an aggregation.
    // Therefore we need another AggregateOp at the origin to combine each
    // of the groups that are emitted from the partition/node levels.

    // First modify any necessary aggregate terms in this AggregateOp and
    // add it to the partition chain
    QueryNode* pPartitionAggregate = AggregateOp::pCreatePartitioned(pNode);
    ASSERT(AggregateOp::bIsExactType(pPartitionAggregate));
    vPushOnPartitionChain(pPartitionAggregate, pChains);

    // Need to add a super-aggregating AggregateOp to the node level.
    // This is based on the partitioned aggregate.
    QueryNode* pNodeAggregate = ProjectOp::pCreateSuperAggregate(pPartitionAggregate, pRootNode);
    ASSERT(AggregateOp::bIsExactType(pPartitionAggregate));
    vPushOnNodeChain(pNodeAggregate, pChains);

    // Need to add a super-aggregating AggregateOp to the origin.
    // This is based on the original logical aggregateOp
    QueryNode* pOriginAggregate = ProjectOp::pCreateDoubleSuperAggregate(pNode, pRootNode);
    ASSERT(AggregateOp::bIsExactType(pPartitionAggregate));
    vPushOnOriginChain(pOriginAggregate, pChains);

  } else {
    // Apply to origin only
    QueryNode* pAggregateOp = pNode->pDeepCopy(true, nullptr);

    PlanObject::vSetLocation(pAggregateOp, Parser::Location::LO_ORIGIN);
    vPushOnOriginChain(pAggregateOp, pChains);
  }

}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a ExplainOp                                    */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionExplainOp(QueryNode* pNode, GraphChains* pChains,
                                        QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  UNUSED(pRootNode);
  ASSERT(ExplainOp::bIsExactType(pNode));

//  QueryNode* pExplainNode = pNode->pDeepCopy(true, nullptr);
//
//  // ExplainOps occur in their own StatementMarker section. There must already
//  // be another chain in the pStmtSectionStack that we need to join up to this one.
//  ASSERT(pStmtSectionStack->nItems() > 0);
//  pExplainNode->vAppendChild(pStmtSectionStack->pPop());
//
//  vPushOnOriginChain(pExplainNode, pChains);

  QueryNode* pExplainNode = pNode->pDeepCopy(true);
  ASSERT(pStmtSectionStack->nItems() == 0);
  vPushOnOriginChain(pExplainNode, pChains);
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a FilterOp                                     */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionFilterOp(QueryNode* pNode, GraphChains* pChains,
                                       QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  ASSERT(FilterOp::bIsExactType(pNode));

  b1 bIsPartitionable = PlanObject::bIsPartitionable(pNode, pRootNode);
  if (bIsPartitionable) {

    QueryNode* pExpr = FilterOp::pFilterExpr(pNode);
    b1 bHasAnAggregate = Expr::bContainsAnAggregate(pExpr);

    if (bHasAnAggregate) {
      // This represents a HAVING clause. This has to run on the origin only, after
      // all partitioned aggregations have been combined.
      // We need to convert the expression to be a super-aggregated expression,
      // keeping in mind that a super-aggregation also occurs at the node level.
      QueryNode* pNewExpr = pExpr->pDeepCopy(true);
      Expr::vSuperAggregate(pNewExpr, pRootNode);  //  <-- node-level super-aggregation
      Expr::vSuperAggregate(pNewExpr, pRootNode);  //  <-- origin-level super-aggregation
      QueryNode* pOriginFilter = FilterOp::pCreate(pNewExpr);
      vPushOnOriginChain(pOriginFilter, pChains);

    } else {
      // No aggregate, therefore it is a WHERE clause. These can run on each partition.
      QueryNode* pPartitionFilter = pNode->pDeepCopy(true, nullptr);
      vPushOnPartitionChain(pPartitionFilter, pChains);
    }

  } else {
    // Apply to origin only
    QueryNode* pFilterOp = pNode->pDeepCopy(true, nullptr);

    PlanObject::vSetLocation(pFilterOp, Parser::Location::LO_ORIGIN);
    vPushOnOriginChain(pFilterOp, pChains);
  }

}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a LimitOp                                      */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionLimitOp(QueryNode* pNode, GraphChains* pChains,
                                      QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  ASSERT(LimitOp::bIsExactType(pNode));

  b1 bIsPartitionable = PlanObject::bIsPartitionable(pNode, pRootNode);
  if (bIsPartitionable) {

    b1 bCanPushDownLimitOp = PartitionPlan::bCanPushDownLimitOp(pNode);

    if (bCanPushDownLimitOp) {

      // Handle offset case
      i8 iOffset = LimitOp::iOffset(pNode);
      if (iOffset > 0) {
        // What gets pushed down must have a limit count increased by this offset, and a zero offset
        i8 iCount = LimitOp::iLimit(pNode);

        QueryNode* pNodeLimit = LimitOp::pCreate(iCount + iOffset, -1);
        vPushOnNodeChain(pNodeLimit, pChains);

        QueryNode* pPartitionLimit = LimitOp::pCreate(iCount + iOffset, -1);
        vPushOnPartitionChain(pPartitionLimit, pChains);

      } else {
        // No offset to deal with
        QueryNode *pNodeLimit = pNode->pDeepCopy(true, nullptr);
        vPushOnNodeChain(pNodeLimit, pChains);

        QueryNode *pPartitionLimit = pNode->pDeepCopy(true, nullptr);
        vPushOnPartitionChain(pPartitionLimit, pChains);
      }
    }

    QueryNode* pOriginLimit = pNode->pDeepCopy(true, nullptr);
    vPushOnOriginChain(pOriginLimit, pChains);

  } else {
    // Apply to origin only
    QueryNode* pLimitOp = pNode->pDeepCopy(true, nullptr);

    PlanObject::vSetLocation(pLimitOp, Parser::Location::LO_ORIGIN);
    vPushOnOriginChain(pLimitOp, pChains);
  }

}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a OrderOp                                      */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionOrderOp(QueryNode* pNode, GraphChains* pChains,
                                      QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  ASSERT(OrderOp::bIsExactType(pNode));

  b1 bIsPartitionable = PlanObject::bIsPartitionable(pNode, pRootNode);
  if (bIsPartitionable) {

    b1 bCanPushDownOrderOp = PartitionPlan::bCanPushDownOrderOp(pNode);

    if (bCanPushDownOrderOp) {
      // The ordering expression won't have an aggregate
      ASSERT(!OrderOp::bContainsAnAggregate(pNode));

      QueryNode* pPartitionOrder = pNode->pDeepCopy(true, nullptr);
      vPushOnPartitionChain(pPartitionOrder, pChains);

      QueryNode* pNodeOrder = pNode->pDeepCopy(true, nullptr);
      vPushOnNodeChain(pNodeOrder, pChains);

    }

    if (OrderOp::bContainsAnAggregate(pNode)) {
      QueryNode* pOriginOrder = OrderOp::pCreateDoubleSuperAggregate(pNode, pRootNode);
      vPushOnOriginChain(pOriginOrder, pChains);

    } else {
      QueryNode* pOriginOrder = pNode->pDeepCopy(true, nullptr);
      vPushOnOriginChain(pOriginOrder, pChains);
    }

  } else {
    // Apply to origin only
    QueryNode* pOrderOp = pNode->pDeepCopy(true, nullptr);

    PlanObject::vSetLocation(pOrderOp, Parser::Location::LO_ORIGIN);
    vPushOnOriginChain(pOrderOp, pChains);
  }

}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a ProjectOp                                    */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionProjectOp(QueryNode* pNode, GraphChains* pChains,
                                        QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  ASSERT(ProjectOp::bIsExactType(pNode));

  b1 bIsPartitionable = PlanObject::bIsPartitionable(pNode, pRootNode);
  if (bIsPartitionable) {

    b1 bHasAnAggregate = ProjectOp::bHasAnAggregate(pNode);
    b1 bIsPostProject = PartitionPlan::bIsPostProject(pNode);

    if (bIsPostProject) {

      if (bHasAnAggregate) {
        // Post project with aggregates. Need to super-aggregate the aggregates before placing
        // on the origin chain.
        QueryNode* pOriginProject = ProjectOp::pCreateDoubleSuperAggregate(pNode, pRootNode); // super-aggregating the node super-aggregate
        vPushOnOriginChain(pOriginProject, pChains);

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
        ASSERT(ProjectOp::bHasOnlyAggregates(pNode, true));

        // Modify any necessary aggregate terms in this ProjectOp and
        // add it to the partition chain
        QueryNode* pPartitionProject = ProjectOp::pCreatePartitioned(pNode);
        vPushOnPartitionChain(pPartitionProject, pChains);

        // Need to add a super-aggregating projectOp to the node and origin levels.
        // The node-level super-aggregate is based on the partitionedProject, which has
        // already split any AVG() into separate SUM()'s and COUNT()'s.
        QueryNode* pNodeProject = ProjectOp::pCreateSuperAggregate(pPartitionProject, pRootNode);
        vPushOnNodeChain(pNodeProject, pChains);

        // The origin-level super-aggregate is based on the original logical project,
        // but it also needs to factor in that there was a node-level aggregation too.
        QueryNode* pOriginProject = ProjectOp::pCreateDoubleSuperAggregate(pNode, pRootNode);
        vPushOnOriginChain(pOriginProject, pChains);

      } else {
        // Non-post-project without any aggregates
        QueryNode* pPartitionProject = pNode->pDeepCopy(true, nullptr);
        vPushOnPartitionChain(pPartitionProject, pChains);
      }
    }

  } else { // !bIsPartitionable
    // Apply to origin only
    QueryNode* pProjectOp = pNode->pDeepCopy(true, nullptr);

    PlanObject::vSetLocation(pProjectOp, Parser::Location::LO_ORIGIN);
    vPushOnOriginChain(pProjectOp, pChains);
  }
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a UnionOp                                      */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionUnionOp(QueryNode* pNode, GraphChains* pChains,
                                      QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  ASSERT(UnionOp::bIsExactType(pNode));
  UNUSED(pRootNode);

  // Unions only run on the origin
  QueryNode* pOriginUnion = pNode->pDeepCopy(true, nullptr);

  // UnionOps occur in their own StatementMarker section. There must already
  // be another two chains in the pStmtSectionStack that we need to join up to this one.
  ASSERT(pStmtSectionStack->nItems() >= 2);
  pOriginUnion->vPushChild(pStmtSectionStack->pPop());
  pOriginUnion->vPushChild(pStmtSectionStack->pPop());

  vPushOnOriginChain(pOriginUnion, pChains);
}

/*----------------------------------------------------------------------------*/
/* Handles the partitioning of a JoinOp                                       */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPartitionJoinOp(QueryNode* pNode, GraphChains* pChains,
                                     QueryNodeStack* pStmtSectionStack, PlanNode* pRootNode)
{
  UNUSED(pStmtSectionStack);
  ASSERT(JoinOp::bIsOfType(pNode));

  b1 bIsPartitionable = PlanObject::bIsPartitionable(pNode, pRootNode);
  if (bIsPartitionable) {

    QueryNode* pPartitionJoin = pNode->pDeepCopy(true, nullptr);

    // It could be the case that both children are partitionable, or it could
    // be the case that only one of them is partitionable. We need to be
    // careful with this. Also need to keep the children in the same order.

    b1 bChild1Partitionable = PlanObject::bIsPartitionable(_pQueryNode(pNode->pFirstChild()), pRootNode);
    b1 bChild2Partitionable = PlanObject::bIsPartitionable(_pQueryNode(pNode->pSecondChild()), pRootNode);

    if (bChild1Partitionable && bChild2Partitionable) {
      ASSERT(pChains->m_pPartitionChainStack->nItems() >= 2);
      pPartitionJoin->vPushChild(pChains->m_pPartitionChainStack->pPop());
      pPartitionJoin->vPushChild(pChains->m_pPartitionChainStack->pPop());

    } else if (bChild1Partitionable) {
      // push child 2 from the origin chain first
      pPartitionJoin->vPushChild(pChains->m_pOriginChainStack->pPop());

      // now push child 1 from the partition chain
      pPartitionJoin->vPushChild(pChains->m_pPartitionChainStack->pPop());

    } else if (bChild2Partitionable) {
      // push child 2 from the partition chain
      pPartitionJoin->vPushChild(pChains->m_pPartitionChainStack->pPop());

      // push child 1 from the origin chain first
      pPartitionJoin->vPushChild(pChains->m_pOriginChainStack->pPop());

    } else {
      NEVER_REACH
    }

    vPushOnPartitionChain(pPartitionJoin, pChains);

  } else {
    // Apply to origin only
    QueryNode* pJoinOp = pNode->pDeepCopy(true, nullptr);
    PlanObject::vSetLocation(pJoinOp, Parser::Location::LO_ORIGIN);

    ASSERT(pChains->m_pOriginChainStack->nItems() >= 2);
    pJoinOp->vPushChild(pChains->m_pOriginChainStack->pPop());
    pJoinOp->vPushChild(pChains->m_pOriginChainStack->pPop());

    pChains->m_pOriginChainStack->vPush(pJoinOp);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if this projectOp is occurring after an OrderOp, LimitOp or   */
/* a HAVING clause (a filterOp with aggregates)                               */
/*----------------------------------------------------------------------------*/
b1 PartitionPlan::bIsPostProject(QueryNode* pProjectOp)
{
  ASSERT(ProjectOp::bIsExactType(pProjectOp));

  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);

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
/* Returns true if this OrderOp can be pushed down to the partition level     */
/*----------------------------------------------------------------------------*/
b1 PartitionPlan::bCanPushDownOrderOp(QueryNode* pOrderOp)
{
  ASSERT(OrderOp::bIsExactType(pOrderOp));

  // An OrderOp containing an aggregation cannot be pushed down.
  if (OrderOp::bContainsAnAggregate(pOrderOp)) {
    return false;
  }

  // a lambda that returns a pointer to a node for any condition where an OrderOp
  // cannot be pushed down.
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);

    QueryNode* pOp = _pQueryNode(pNode);

    // An OrderOp occurring above:
    //   - an AggregateOp,
    //   - a FilterOp with an aggregate expr
    //   - a SetOp
    // cannot be pushed down

    if (AggregateOp::bIsExactType(pOp)) return true;

    if (FilterOp::bIsOfType(pOp)) {
      QueryNode* pExpr = FilterOp::pFilterExpr(pOp);

      return Expr::bContainsAnAggregate(pExpr);
    }

    if (SetOp::bIsOfType(pOp)) return true;
    
    return false;
  };

  PlanNode* pItem = GraphNav::pBoundedSearch(pOrderOp->pFirstChild(), false, true, fCondition, nullptr);

  return (pItem == nullptr);
}

/*----------------------------------------------------------------------------*/
/* Returns true if this LimitOp can be pushed down to the partition level     */
/*----------------------------------------------------------------------------*/
b1 PartitionPlan::bCanPushDownLimitOp(QueryNode* pLimitOp)
{
  ASSERT(LimitOp::bIsExactType(pLimitOp));

  // a lambda that returns a pointer to a node for any condition where an LimitOp
  // cannot be pushed down.
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);

    QueryNode* pOp = _pQueryNode(pNode);

    // An LimitOp occurring above:
    //   - an OrderOp that cannot be pushed down
    //   - an AggregateOp,
    //   - a ProjectOp with aggregate expressions,
    //   - a FilterOp with an aggregate expr
    //   - a SetOp
    // cannot be pushed down

    if (OrderOp::bIsOfType(pOp)) {
      return !bCanPushDownOrderOp(pOp);

    } else if (AggregateOp::bIsExactType(pOp)) {
      return true;

    } else if (ProjectOp::bIsExactType(pOp)) {
      return ProjectOp::bHasAnAggregate(pOp);

    } else if (FilterOp::bIsOfType(pOp)) {
      QueryNode* pExpr = FilterOp::pFilterExpr(pOp);
      return Expr::bContainsAnAggregate(pExpr);

    } else if (SetOp::bIsOfType(pOp)) {
      return true;
    }

    return false;
  };

  PlanNode* pItem = GraphNav::pBoundedSearch(pLimitOp->pFirstChild(), false, true, fCondition, nullptr);

  return (pItem == nullptr);
}


/*----------------------------------------------------------------------------*/
/* Push a node onto an existing partition chain                               */
/*----------------------------------------------------------------------------*/
void PartitionPlan::vPushOnPartitionChain(QueryNode* pNode, GraphChains* pChains)
{
  ASSERT(pNode);
  ASSERT(pChains);

  if (pChains->m_pPartitionChainStack->nItems() == 0) {
    PlanObject::vSetLocation(pNode, Location::LO_ALL_PARTITIONS);
    pChains->m_pPartitionChainStack->vPush(pNode);

  } else {
    QueryNode* pPartitionChain = pChains->m_pPartitionChainStack->pPop();
    pNode->vAppendChild(pPartitionChain);
    pChains->m_pPartitionChainStack->vPush(pNode);
  }
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


  if (pChains->m_pOriginChainStack->nItems() == 0) {
    PlanObject::vSetLocation(pNode, Location::LO_ORIGIN);
    pChains->m_pOriginChainStack->vPush(pNode);

  } else {
    QueryNode* pOriginChain = pChains->m_pOriginChainStack->pPop();
    pNode->vAppendChild(pOriginChain);
    pChains->m_pOriginChainStack->vPush(pNode);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
