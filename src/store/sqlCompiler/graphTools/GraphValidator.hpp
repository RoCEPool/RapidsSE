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

    // Validate only the /plan subsection of the plan (ignoring definitions & primitives)
    static void vValidatePlan(PlanNode* pRootNode);

    // Validate a single node
    static void vValidatePlanNode(QueryNode* pQueryNode, PlanNode* pRootNode);

  private:
    static void                vValidateRootNode(PlanNode* pRootNode);
    static GraphNav::eWalkCtrl eValidatePrimitiveNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateDefinitionNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidatePlanNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateMetadataNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static GraphNav::eWalkCtrl eValidateStatisticsNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);

    static b1 bContainsChildrenNamed(PlanNode* pNode, StzList* pNames);
    static b1 bHasOptionalConstraint(PlanNode* pNode);
    static PlanNode* pGetConstraint(Stz* szConstraintDefn, DefinitionNode* pNode);
    static PlanNode* pGetConstraint(Stz* szConstraintDefn, DefinitionNode* pNode, PlanNode* pRootNode, b1 bWalkInheritance);

    // - - - - - - - - - - - - - - - - - - - - -

    static void vRoot_1_1(PlanNode* pRootNode);
    static void vRoot_2_1(PlanNode* pRootNode);
    static void vRoot_3_1(PlanNode* pRootNode);
    static void vRoot_4_1(PlanNode* pRootNode);
    static void vRoot_5_1(PlanNode* pRootNode);
    static void vRoot_6_1(PlanNode* pRootNode);
    static void vRoot_6_2(PlanNode* pRootNode);
    
    static void vPrim_1_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPrim_2_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPrim_3_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPrim_4_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPrim_5_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPrim_6_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);

    static void vDefn_1_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_2_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_2_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_4(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_3_5(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_4_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_4_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_4_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_4(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_5(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_6(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_7(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_8(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_9(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_10(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_11(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_12(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_13(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_14(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_15(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_16(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_17(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_18(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_19(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_5_20(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vDefn_6_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);

    static void vPlan_1_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    //static void vPlan_2_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_3_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_3_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_3_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_3_4(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_3_5(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    //static void vPlan_4_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_5_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_5_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_6_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_6_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_7_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_7_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_4(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_5(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_6(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_7(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_8(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_9(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_10a(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_10b(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_11(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_12(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_13(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_14(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_15(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_16(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_17(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_18(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_19(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_20a(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);
    static void vPlan_8_20b(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
