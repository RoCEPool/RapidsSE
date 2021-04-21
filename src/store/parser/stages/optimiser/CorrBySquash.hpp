/*============================================================================*/
/* CorrBySquash.hpp - Push down ProjectOps that implement the CORRESPONDINB BY*/
/* clause and squash any redundant ProjectOps below it                        */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace STORE
{
  class SeApi;
}

namespace Parser
{
  class QueryNode;

  class CorrBySquash
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    CorrBySquash();

  public:
    // Searches for any CORRESPONDING BY ProjectOps and squashes them if possible
    static void vSearchAndSquash(PlanNode* pRootNode);

  private:
    // Searches for any CORRESPONDING BY ProjectOps in a statement marker section
    static void vSearchStatementSection(QueryNode* pStatementMarker, PlanNode* pRootNode);

    // Pushes down and squashes a ProjectOp down a given subtree
    static void vPushDownProjectOp(QueryNode* pProjectOp, QueryNode* pStartingNode, PlanNode* pParent);

    // Creates a copy of pProjectOp, updated for the columns returned by pChild.
    // Necessary when pushing a ProjectOp through a SetOp and into the second child subtree.
    static QueryNode* pUpdateExprsFor(QueryNode* pProjectOp, QueryNode* pChild, PlanNode* pRootNode);

    // Creates and returns a ProjectOp for pushing down as the second child of a SetOp
    // where the SetOp does not have a CORRESPONDING BY clause.
    static QueryNode* pCreateProjectUsingOrdinals(QueryNode* pChild1, QueryNode* pChild2, PlanNode* pRootNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
