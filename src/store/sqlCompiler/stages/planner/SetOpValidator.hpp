/*============================================================================*/
/* SetOpValidator.hpp - Simplifies and validates plan segments that contain   */
/* set operations (e.g. UNION, INSERSECT, etc)                                */
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

  class SetOpValidator
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    SetOpValidator();

  public:
    // Returns true if there are any set operations in this plan.
    // If not then we can ignore these other functions.
    static b1 bHasSetOps(PlanNode* pRootNode);

    // Tries to simplify the set operators, if needed, by handling
    // the CORRESPONDING BY list.
    static void vSimplifySetOps(PlanNode* pRootNode);

    // Walk the graph and check the semantics of the set operations.
    // A SqlException will be thrown if an invalid semantic is found.
    static void vValidateSetOps(PlanNode* pRootNode);

  private:

    static void vSimplifySetOp(QueryNode* pStatementMarker, PlanNode* pRootNode);
    static void vDetermineCommonColumns(QueryNode* pSetOp, PlanNode* pRootNode);
    static void vConvertCorrByToProjectOp(QueryNode* pSetOp, PlanNode* pRootNode);
    static QueryNode* pFindProjectedColumn(QueryNodeIter* pProjColumnIter, Stz* szColumnName);
    static void vValidateSetOp(QueryNode* pStatementMarker, PlanNode* pRootNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
