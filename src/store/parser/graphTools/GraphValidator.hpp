/*============================================================================*/
/* GraphValidator.hpp - all of the rules and logic for validating plan graphs */
/*============================================================================*/
#pragma once

#include "GraphNav.hpp"

namespace Parser
{
  class PlanNode;

  class GraphValidator
  {
  private:
    GraphValidator();

  public:
    // Validate the entire plan, including definitions
    static b1 bValidateAll(PlanNode* pRootNode);

    // Validate only the /plan section of the plan (ignoring definitions & primitives)
    static b1 bValidatePlan(PlanNode* pRootNode);

  private:
    static b1                  bValidateRootNode(PlanNode* pRootNode);
    static GraphNav::eWalkCtrl eValidatePrimitiveNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateDefinitionNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidatePlanNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateMetadataNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateStatisticsNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
