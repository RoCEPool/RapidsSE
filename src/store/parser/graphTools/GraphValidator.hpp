/*============================================================================*/
/* GraphValidator.hpp - all of the rules and logic for validating plan graphs */
/*============================================================================*/
#pragma once

#include "GraphNav.hpp"
#include "StzList.hpp"

namespace Parser
{
  class PlanNode;

  class GraphValidator
  {
  private:
    GraphValidator();

  public:
    // Validate the entire plan, including definitions
    static void vValidateAll(PlanNode* pRootNode);

    // Validate only the /plan section of the plan (ignoring definitions & primitives)
    static void vValidatePlan(PlanNode* pRootNode);

  private:
    static void                vValidateRootNode(PlanNode* pRootNode);
    static GraphNav::eWalkCtrl eValidatePrimitiveNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateDefinitionNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidatePlanNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateMetadataNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateStatisticsNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);

    static b1 bContainsChildrenNamed(PlanNode* pNode, StzList* pNames);

    // - - - - - - - - - - - - - - - - - - - - -

    static void vRoot_1_1(PlanNode* pRootNode);
    static void vRoot_2_1(PlanNode* pRootNode);
    static void vRoot_3_1(PlanNode* pRootNode);
    static void vRoot_4_1(PlanNode* pRootNode);
    static void vRoot_5_1(PlanNode* pRootNode);
    static void vRoot_6_1(PlanNode* pRootNode);
    static void vRoot_6_2(PlanNode* pRootNode);
    
    static void vPrim_1_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vPrim_2_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vPrim_3_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vPrim_4_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vPrim_5_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vPrim_6_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);

    static void vDefn_1_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_2_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_2_2(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_2(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_3(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_4(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_5(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_4_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_4_2(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_2(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_3(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_4(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_5(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_6(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_7(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_8(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_9(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_10(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);
    static void vDefn_6_1(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
