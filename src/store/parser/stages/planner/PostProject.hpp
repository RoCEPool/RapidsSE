/*============================================================================*/
/* PostProject.hpp - Ensures only selected columns are returned in a resultset*/
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "QueryNodeList.hpp"
#include "GraphNav.hpp"

namespace STORE
{
  class SeApi;
}

namespace Parser
{
  class QueryNode;

  class PostProject
  {
  private:
    // This class is not intended to be constructed. Keeping it purely functional
    // to match the processing of the plan graph.
    PostProject();

  public:
    // Adds a ProjectOp to the plan if it is required to trim the projected
    // columns to only those that the user asked for
    static void vCheckPostProject(PlanNode* pRootNode);


  private:
    // Visits all nodes, looking for StatementMarkers
    static GraphNav::eWalkCtrl eStmtMarkerVisitor(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);

    // Visits all nodes within a StatementMarker subsection
    static GraphNav::eWalkCtrl eStmtSectionVisitor(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);

    // Checks if there are any expressions in sql operators that are in excess
    // of the project list and adds a post project if required
    static void vAddPostProject(void* pContainer, QueryNode* pStatementMarker, PlanNode* pRootNode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
