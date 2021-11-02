/*============================================================================*/
/* GraphValidator.hpp - all of the rules and logic for validating plan graphs */
/*============================================================================*/

#include "BASE.h"
#include "GraphValidator.hpp"
#include "PlanNode.hpp"
#include "DefinitionNode.hpp"
#include "PrimitiveNode.hpp"
#include "PlanConstants.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

#include "ListExo_t.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Validate the entire plan, including definitions                            */
/*----------------------------------------------------------------------------*/
void GraphValidator::vValidateAll(PlanNode *pRootNode)
{
  // Validate root
  vValidateRootNode(pRootNode);

  // Validate primitives
  PlanNode* pPrimitives = GraphNav::pFind(const_cast<Stz*>(C_PRIMITIVES_PATH), pRootNode);
  ASSERT(pPrimitives);
  GraphNav::bGlobalWalk(pPrimitives, GraphValidator::eValidatePrimitiveNode, pRootNode);

  // Validate definitions
  PlanNode* pDefinitions = GraphNav::pFind(const_cast<Stz*>(C_DEFINITIONS_PATH), pRootNode);
  ASSERT(pDefinitions);
  GraphNav::bGlobalWalk(pDefinitions, GraphValidator::eValidateDefinitionNode, pRootNode);

  // Validate plan
  PlanNode* pPlanNode = GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRootNode);
  ASSERT(pPlanNode);
  GraphNav::bGlobalWalk(pPlanNode, GraphValidator::eValidatePlanNode, pRootNode);

  // Validate metadata
  PlanNode* pMetadata = GraphNav::pFind(const_cast<Stz*>(C_METADATA_PATH), pRootNode);
  ASSERT(pMetadata);
  GraphNav::bGlobalWalk(pMetadata, GraphValidator::eValidateMetadataNode, pRootNode);

  // Validate statistics
  PlanNode* pStatistics = GraphNav::pFind(const_cast<Stz*>(C_STATISTICS_PATH), pRootNode);
  ASSERT(pStatistics);
  GraphNav::bGlobalWalk(pStatistics, GraphValidator::eValidateStatisticsNode, pRootNode);

}

/*----------------------------------------------------------------------------*/
/* Validate only the /plan node against its definitions                       */
/*----------------------------------------------------------------------------*/
void GraphValidator::vValidatePlan(PlanNode* pRootNode)
{
  PlanNode* pPlanNode = GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRootNode);
  ASSERT(pPlanNode);
  GraphNav::bGlobalWalk(pPlanNode, GraphValidator::eValidatePlanNode, pRootNode);

}

/*----------------------------------------------------------------------------*/
/* Validate a specific node under /plan against its definitions               */
/*----------------------------------------------------------------------------*/
void GraphValidator::vValidatePlanNode(QueryNode* pQueryNode, PlanNode* pRootNode)
{
  ASSERT(pQueryNode);
  ASSERT(pQueryNode->eGetClassNo() == CN_QUERYNODE);

  s2K sPathBuf;
  i8 iDepth = 0;
  b1 bFound = GraphNav::bGetPathAndDepth(pQueryNode, pRootNode, sPathBuf, iDepth, true);
  RASSERT(bFound);

  eValidatePlanNode(pQueryNode, sPathBuf, iDepth, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate the root node only                                                */
/*----------------------------------------------------------------------------*/
void GraphValidator::vValidateRootNode(PlanNode* pRootNode)
{
  // *********************************
  // Root Node Validation Rules:
  // *********************************
  //
  // ROOT-1: Validate the name field:
  //    ROOT-1.1: the name field must be "__root__".
  //
  // ROOT-2: Validate the value field:
  //    ROOT-2.1: the value field must not be null and it must contains a string.
  //
  // ROOT-3: Validate the type field:
  //    ROOT-3.1: validate that the type field is not null and has a value of
  //              /__root__/primitives/root
  //
  // ROOT-4: Validate the properties:
  //    ROOT-4.1: validate that the properties field is null.
  //
  // ROOT-5: Validate the constraints:
  //    Nothing needed here due to ROOT-4.1.
  //
  // ROOT-6: Validate the children:
  //    ROOT-6.1: validate that the children field is not null.
  //    ROOT-6.2: validate that the children field has 5 items named:
  //              "primitives", "definitions", "plan", "metadata" and "statistics".
  //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(pRootNode);
  RASSERT_TYPE(pRootNode, CN_ROOTNODE);

  // ROOT-1: Validate the name field:
  //    ROOT-1.1: the name field must be "__root__".
  vRoot_1_1(pRootNode);


  // - - - - - - - - - - - - - - - - - - - - - -

  // ROOT-2: Validate the value field:
  //    ROOT-2.1: the value field must not be null and it must contains a string.
  vRoot_2_1(pRootNode);

  // - - - - - - - - - - - - - - - - - - - - - -

  // ROOT-3: Validate the type field:
  //    ROOT-3.1: validate that the type field is not null and has a value of
  //              /__root__/primitives/root
  vRoot_3_1(pRootNode);

  // - - - - - - - - - - - - - - - - - - - - - -

  // ROOT-4: Validate the properties:
  //    ROOT-4.1: validate that the properties field is null.
  vRoot_4_1(pRootNode);

  // - - - - - - - - - - - - - - - - - - - - - -

  // ROOT-5: Validate the constraints:
  //    Nothing needed here due to ROOT-4.1.
  vRoot_5_1(pRootNode);

  // - - - - - - - - - - - - - - - - - - - - - -

  // ROOT-6: Validate the children:
  //    ROOT-6.1: validate that the children field is not null.
  vRoot_6_1(pRootNode);

  //    ROOT-6.2: validate that the children field has 5 items named:
  //              "primitives", "definitions", "plan", "metadata" and "statistics".
  vRoot_6_2(pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /primitives               */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphValidator::eValidatePrimitiveNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // *********************************
  // Primitive Node Validation Rules:
  // *********************************
  //
  // PRIM-1: Validate the name field:
  //    PRIM-1.1: the name field must not be null.
  //
  // PRIM-2: Validate the value field:
  //    PRIM-2.1: the value field must not be null and it contains a string.
  //
  // PRIM-3: Validate the type field:
  //    PRIM-3.1: validate that the type field is null.
  //
  // PRIM-4: Validate the properties:
  //    PRIM-4.1: validate that the properties field is null.
  //
  // PRIM-5: Validate the constraints:
  //    Nothing needed here due to PRIM-4.1.
  //
  // PRIM-6: Validate the children:
  //    PRIM-6.1: validate that the children field is either null or has 1 or more children.
  //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(pNode);
  RASSERT_TYPE(pNode, CN_PRIMITIVENODE);

  // PRIM-1: Validate the name field:
  //    PRIM-1.1: the name field must not be null.
  vPrim_1_1(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PRIM-2: Validate the value field:
  //    PRIM-2.1: the value field must not be null and it contains a string.
  vPrim_2_1(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PRIM-3: Validate the type field:
  //    PRIM-3.1: validate that the type field is empty.
  vPrim_3_1(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PRIM-4: Validate the properties:
  //    PRIM-4.1: validate that the properties field is null.
  vPrim_4_1(pNode, sPath, iDepth, pvArg);


  // - - - - - - - - - - - - - - - - - - - - - -

  // PRIM-5: Validate the constraints:
  //    Nothing needed here due to PRIM-4.1.
  vPrim_5_1(pNode, sPath, iDepth, pvArg);


  // - - - - - - - - - - - - - - - - - - - - - -

  // PRIM-6: Validate the children:
  //    PRIM-6.1: validate that the children field is either null or has 1 or more children.
  vPrim_6_1(pNode, sPath, iDepth, pvArg);



  return Parser::GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /definitions              */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphValidator::eValidateDefinitionNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-1: Validate the name field:
  //    DEFN-1.1: the name field must not be null.
  //
  // DEFN-2: Validate the value field:
  //    DEFN-2.1: has a non-null string value if the type field is null.
  //    DEFN-2.2: has a null or non-null value if the type field is not null.
  //
  // DEFN-3: Validate the type field:
  //    DEFN-3.1: has a null type field when the node is not a direct property of another node.
  //              i.e., the node is on a direct inheritence path back to /definitions.
  //    DEFN-3.2: has a non-null type field when the node (or any parent above it) is
  //              a property of a parent node. (i.e., the node is on a "property path").
  //    DEFN-3.3: when the type field is non-null, it must be an absolute path.
  //    DEFN-3.4: when the type field is non-null, the path must be resolvable to another
  //              node in the system.
  //    DEFN-3.5: when the type field is non-null, the node that it resolves to must be
  //              another definition or a primitive node.
  //
  // DEFN-4: Validate the properties:
  //    DEFN-4.1: all properties of the node must have a unique name with respect to
  //              other properties defined for the node.
  //    DEFN-4.2: all properties of the node must have a unique name with respect to
  //              other properties defined in parent nodes that the current node inherits
  //              from. The only exception here is the "__constraints" node.
  //    DEFN-4.3: all properties (except "__constraints") must have a non-null type field.
  //
  // DEFN-5: Validate the constraints:
  //    DEFN-5.1: any constraints must exist under a node named "__constraints" which
  //              itself is a property of the current node.
  //    DEFN-5.2: if a __constraints container exists then it must have 1 or more children.
  //    DEFN-5.3: validate that if any of the following constraints exist as children
  //              under a __constraints container, then those constraints must also have
  //              one or more child nodes (being a parameter of the constraint):
  //                - childType
  //                - valueType
  //                - childCount
  //    DEFN-5.4: if any of the constraints specified in DEFN-5.3 exist, then validate that
  //              their parameter nodes must have non-null name, type and value fields.
  //    DEFN-5.5: If the childType constraint is used then validate that its child parameter
  //              must have a value field that is a path that can be resolved to another
  //              definition node.
  //    DEFN-5.6: If the valueType constraint is used then validate that its child parameter
  //              must have a value field that is a path that can be resolved to another
  //              definition node, and that that path is a subclass of valueObject.
  //    DEFN-5.7: validate that the following combinations of constraints cannot be applied
  //              in the __constraints container for a given object:
  //                - (noChildren/alwaysChildren),
  //                - (nullable/nonnullable),
  //                - (optional/mandatory)
  //    DEFN-5.8: validate that the constraints combinations specified in DEFN-5.7 cannot occur
  //              with any constraints inherited by the current node.
  //    DEFN-5.9: validate that the same constraint cannot be specified more than once
  //              in the current node.
  //    DEFN-5.10: validate that the same constraint cannot be specified more than once
  //               when considering all inherited constraints of the current node.
  //
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.11: If an object definition has a property where the property's type field 
  //               is non null, validate that any constraints of that property must exist
  //               under a node named "__constraints" which itself is a property of the
  //               property in question.
  //    DEFN-5.12: If an object definition has a property where the property's type field
  //               is non null, validate that if a constraints container exists in that
  //               property then it must have one or more children.
  //    DEFN-5.13: If an object definition has a property where the property's type field
  //               is non null, validate that if any of the following constraints exist as
  //               children under a __constraints container of that property, then those
  //               constraints must also have one or more child nodes (being a parameter of
  //               the constraint):
  //                - childType
  //                - valueType
  //                - childCount
  //    DEFN-5.14: If an object definition has a property where the property's type field
  //               is non null, validate that if any of the constraints specified in
  //               DEFN-5.13 are specified for the property in question, then validate that
  //               their parameter nodes must have non-null name, type and value fields.
  //    DEFN-5.15: If an object definition has a property where the property's type field
  //               is non null, validate that if the childType constraint is used on the property
  //               that its child parameter must have a value field that is a path that can be
  //               resolved to another definition node.
  //    DEFN-5.16: If an object definition has a property where the property's type field
  //               is non null, validate that if the valueType constraint is used on the property
  //               that its child parameter must have a value field that is a path that can be resolved
  //               to another definition node, and that that path is a subclass of valueObject.
  //    DEFN-5.17: If an object definition has a property where the property's type field
  //               is non null, and that property has constraints then validate that the following
  //               combinations of constraints cannot occur in the __constraints container
  //               for that property:
  //                - (noChildren/alwaysChildren),
  //                - (nullable/nonnullable),
  //                - (optional/mandatory)
  //    DEFN-5.18: If an object definition has a property where the property's type field is
  //               non null and that property has a __constraint container, then validate that
  //               any constraints specified are not incompatible with any constraints already
  //               specified in the definition of the property (including that definitions
  //               inheritance hierarchy).
  //    DEFN-5.19: If an object definition has a property where the property's type field
  //               is non null and that property has a __constraint container, then validate
  //               that any constraints specified are not specified more than once in this
  //               constraints container for the property in question.
  //    DEFN-5.20: If an object definition has a property where the property's type field
  //               is non null and that property has a __constraint container, then validate
  //               that any constraints specified are not already specified in the definition
  //               of the property (including that definitions inheritance hierarchy).
  //
  // DEFN-6: Validate the children:
  //    DEFN-6.1: validate that all children of a given definition node must have unique names.
  //
  // NOTE 1:
  //   When validating a property of a node, the definition of that property may have
  //   constraints (let's call these "direct" constraints). But there might also be additional
  //   constraints specified when that property was specified as a property of the node (let's
  //   call these "indirect" constraints).
  //   e.g. consider this _definition_ of object "foo" and its property "bar":
  //   {
  //      name: "foo",
  //      type: "foo_defn",
  //      properties: [
  //        {
  //          name: "bar",
  //          type: "bar_defn",
  //          properties: [
  //            {
  //              name: "__constraints", type: "...", children: [
  //                { name: "optional", type: "/__root__/primitives/constraints/optional" }
  //              ]
  //            }
  //          ]
  //        }
  //      ]
  //   }
  //   Given this definition, the property "bar" may have certain direct constraints from its
  //   definition (bar_defn), but it also has an indirect constraint specified where the property
  //   was defined (at foo:bar).
  //   So to validate this definition we must evaluate "bar" against its definition (bar_defn,
  //   and therefore its direct constraints) as well as against any additional indirect constraints
  //   applied in the declaration of the property "bar" too.
  //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(pNode);
  RASSERT(pNode->eGetClassNo() == CN_DEFINITIONNODE);
  RASSERT(pvArg);

  PlanNode* pRootNode = (PlanNode*) pvArg;
  pRootNode->vAssertHeader();
  RASSERT_TYPE(pRootNode, CN_ROOTNODE);

  // DEFN-1: Validate the name field:
  //    DEFN-1.1: the name field must not be null.
  vDefn_1_1(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // DEFN-2: Validate the value field:
  //    DEFN-2.1: has a non-null string value if the type field is null.
  vDefn_2_1(pNode, sPath, iDepth, pvArg);

  //    DEFN-2.2: has a null or non-null value if the type field is not null.
  // Bit of a noop here
  vDefn_2_2(pNode, sPath, iDepth, pvArg);

    // - - - - - - - - - - - - - - - - - - - - - -

  // DEFN-3: Validate the type field:
  //    DEFN-3.1: has a null type field when the node is not a direct or indirect property
  //              of another node.
  //              i.e., the node is on a direct inheritence path back to /definitions.
  vDefn_3_1(pNode, sPath, iDepth, pvArg);

  //    DEFN-3.2: has a non-null type field when the node (or any parent above it) is
  //              a direct or indirect property of a parent node. (i.e., the node is
  //              on a "property path").
  vDefn_3_2(pNode, sPath, iDepth, pvArg);

  //    DEFN-3.3: when the type field is non-null, it must be an absolute path.
  vDefn_3_3(pNode, sPath, iDepth, pvArg);

  //    DEFN-3.4: when the type field is non-null, the path must be resolvable to another
  //              node in the system.
  vDefn_3_4(pNode, sPath, iDepth, pvArg);

  //    DEFN-3.5: when the type field is non-null, the node that it resolves to must be
  //              another definition or a primitive node.
  vDefn_3_5(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // DEFN-4: Validate the properties:
  //    DEFN-4.1: all properties of the node must have a unique name with respect to
  //              other properties defined for the node.
  vDefn_4_1(pNode, sPath, iDepth, pvArg);

  //    DEFN-4.2: all properties of the node must have a unique name with respect to
  //              other properties defined in parent nodes that the current node inherits
  //              from. The only exception here is the "__constraints" node.
  vDefn_4_2(pNode, sPath, iDepth, pvArg);

  //    DEFN-4.3: all properties (except "__constraints") must have a non-null type field.
  vDefn_4_3(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // DEFN-5: Validate the constraints:
  //    DEFN-5.1: any constraints must exist under a node named "__constraints" which
  //              itself is a property of the current node.
  vDefn_5_1(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.2: if a __constraints container exists then it must have 1 or more children.
  vDefn_5_2(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.3: validate that if any of the following constraints exist as children
  //              under a __constraints container, then those constraints must also have
  //              one child node (being a parameter of the constraint):
  //                - childType
  //                - valueType
  //                - childCount
  vDefn_5_3(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.4: if any of the constraints specified in DEFN-5.3 exist, then validate that
  //              their parameter nodes must have non-null name, type and value fields.
  vDefn_5_4(pNode, sPath, iDepth, pvArg);


  //    DEFN-5.5: If the childType constraint is used then validate that its child parameter
  //              must have a value field that is a path that can be resolved to another
  //              definition node.
  vDefn_5_5(pNode, sPath, iDepth, pvArg);


  //    DEFN-5.6: If the valueType constraint is used then validate that its child parameter
  //              must have a value field that is a path that can be resolved to another
  //              definition node, and that that path is a subclass of valueObject.
  vDefn_5_6(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.7: validate that the following combinations of constraints cannot be applied
  //              in the __constraints container for a given object:
  //                - (noChildren/alwaysChildren),
  //                - (nullable/nonnullable),
  //                - (optional/mandatory)
  vDefn_5_7(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.8: validate that the constraints combinations specified in DEFN-5.7 cannot occur
  //              with any constraints inherited by the current node.
  vDefn_5_8(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.9: validate that the same constraint cannot be specified more than once
  //              in the current node.
  vDefn_5_9(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.10: validate that the same constraint cannot be specified more than once
  //               when considering all inherited constraints of the current node.
  vDefn_5_10(pNode, sPath, iDepth, pvArg);

  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.11: If an object definition has a property where the property's type field
  //               is non null, validate that any constraints of that property must exist
  //               under a node named "__constraints" which itself is a property of the
  //               property in question.
  vDefn_5_11(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.12: If an object definition has a property where the property's type field
  //               is non null, validate that if a constraints container exists in that
  //               property then it must have one or more children.
  vDefn_5_12(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.13: If an object definition has a property where the property's type field
  //               is non null, validate that if any of the following constraints exist as
  //               children under a __constraints container of that property, then those
  //               constraints must also have one or more child nodes (being a parameter of
  //               the constraint):
  //                - childType
  //                - valueType
  //                - childCount
  vDefn_5_13(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.14: If an object definition has a property where the property's type field
  //               is non null, validate that if any of the constraints specified in
  //               DEFN-5.13 are specified for the property in question, then validate that
  //               their parameter nodes must have non-null name, type and value fields.
  vDefn_5_14(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.15: If an object definition has a property where the property's type field
  //               is non null, validate that if the childType constraint is used on the property
  //               that its child parameter must have a value field that is a path that can be
  //               resolved to another definition node.
  vDefn_5_15(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.16: If an object definition has a property where the property's type field
  //               is non null, validate that if the valueType constraint is used on the property
  //               that its child parameter must have a value field that is a path that can be resolved
  //               to another definition node, and that that path is a subclass of valueObject.
  vDefn_5_16(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.17: If an object definition has a property where the property's type field
  //               is non null, and that property has constraints then validate that the following
  //               combinations of constraints cannot occur in the __constraints container
  //               for that property:
  //                - (noChildren/alwaysChildren),
  //                - (nullable/nonnullable),
  //                - (optional/mandatory)
  vDefn_5_17(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.18: If an object definition has a property where the property's type field is
  //               non null and that property has a __constraint container, then validate that
  //               any constraints specified are not incompatible with any constraints already
  //               specified in the definition of the property (including that definitions
  //               inheritance hierarchy).
  vDefn_5_18(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.19: If an object definition has a property where the property's type field
  //               is non null and that property has a __constraint container, then validate
  //               that any constraints specified are not specified more than once in this
  //               constraints container for the property in question.
  vDefn_5_19(pNode, sPath, iDepth, pvArg);

  //    DEFN-5.20: If an object definition has a property where the property's type field
  //               is non null and that property has a __constraint container, then validate
  //               that any constraints specified are not already specified in the definition
  //               of the property (including that definitions inheritance hierarchy).
  vDefn_5_20(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // DEFN-6: Validate the children:
  //    DEFN-6.1: validate that all children of a given definition node must have unique names.
  vDefn_6_1(pNode, sPath, iDepth, pvArg);


  return Parser::GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /plan                     */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphValidator::eValidatePlanNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{

  // *********************************
  // Plan Node Validation Rules:
  // *********************************
  //
  // There are some basic, independent rules that can be validated first up.
  // The rest of the rules are all about ensuring that the object matches
  // the definition it is instantiating.
  //
  // To validate an object against its definition we really only need to handle two aspects -
  //   1.) validate that the current node has all of the necessary properties
  //   that its definition defines (recursively), and
  //   2.) validate that the current node meets all of the constraints that
  //   its definition defines (recursively).
  //
  // PLAN-1: Validate the name field:
  //    PLAN-1.1: the name field must not be null.
  //
  // PLAN-2: Validate the value field:
  //    No validations here as it is largely dependent on constraints
  //
  // PLAN-3: Validate the type field:
  // (NB: we will explicitly validate the object against the definition in PLAN-7 and PLAN-8)
  //    PLAN-3.1: the type field must not be null.
  //    PLAN-3.2: the type field must be an absolute path (start with a '/').
  //    PLAN-3.3: the path in the type field must be resolvable to another
  //              node in the system.
  //    PLAN-3.4: the path in the type field must be resolve to a node in /__root__/definitions.
  //    PLAN-3.5: the path in the type field must not be a property path
  //              (i.e. it must not have a ':' in the path).
  //
  // PLAN-4: Validate the properties:
  // (NB: we will explicitly validate the properties against the definition in PLAN-7 and PLAN-8)
  //    No validations here as it is largely dependent on constraints
  //
  // PLAN-5: Validate the constraints:
  // (NB: we will explicitly validate the constraints against the definition in PLAN-7 and PLAN-8)
  //    PLAN-5.1: Validate that there is no __constraint container in this objects properties.
  //    PLAN-5.2: Validate that there are no properties that derive from a constraint definition.
  //
  // PLAN-6: Validate the children:
  // (NB: we will explicitly validate the children against the definition in PLAN-7 and PLAN-8)
  //    PLAN-6.1: the children must each have a unique name (so they have a unique path).
  //    PLAN-6.2: the leaf nodes on a non-property path must be data sources
  //
  // PLAN-7: Validate the object against its definition properties (excluding constraints):
  //    PLAN-7.1: the node must have properties with names and types that represent all the properties
  //              defined in its definition (including all parents of the definition), excluding any
  //              properties that are marked as optional.
  //    PLAN-7.2: the current node must not have any additional properties that
  //              aren't specified in its definition (including the definition's parents recursively).
  //
  // PLAN-8: Validate the object against its definition constraints:
  // Please refer to NODE 1 below for additional complexities surrounding the validation
  // of constraints and the definition of the terms "direct" constraints and "indirect" constraints.
  //
  //    (Validating direct constraints):
  //    PLAN-8.1:  If a node's definition contains the "noChildren" constraint
  //               then validate that the node has no children.
  //    PLAN-8.2:  If a node's definition contains the "alwaysChildren" constraint
  //               then validate that the node must have at least one child.
  //    PLAN-8.3:  If a node's definition contains the "nullable" constraint
  //               then validate that the value field of the node can be null.
  //    PLAN-8.4:  If a node's definition contains the "nonNullable" constraint
  //               then validate that the value field of the node cannot be null.
  //    PLAN-8.5:  NB: validation of the "optional" constraint is achieved in PLAN-7.1.
  //    PLAN-8.6:  NB: validation of the "mandatory" constraint is achieved in PLAN-7.1.
  //    PLAN-8.7:  If a node's definition contains the "childType" constraint
  //               then validate that any and all children of the node must be of a type indicated
  //               by the constraint's parameter.
  //    PLAN-8.8:  If a node's definition contains the "valueType" constraint
  //               then validate that the value field of the node must be of a type indicated
  //               by the constraint's parameter.
  //    PLAN-8.9:  If a node's definition contains the "childCount" constraint
  //               then validate that the number of children of the node must equal that indicated
  //               by any of the constraint's parameters.
  //    PLAN-8.10a: If a node's definition contains the "minChildCount" constraint
  //               then validate that the number of children of the node must not be less than
  //               that indicated by any of the constraint's parameters.
  //    PLAN-8.10b: If a node's definition contains the "maxChildCount" constraint
  //               then validate that the number of children of the node must not be greater than
  //               that indicated by any of the constraint's parameters.
  //    (Validating indirect constraints):
  //    PLAN-8.11: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "noChildren" constraint then validate
  //               that the property of the current node has no children.
  //    PLAN-8.12: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "alwaysChildren" constraint then validate
  //               that the property of the current node must have at least one child.
  //    PLAN-8.13: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "nullable" constraint then validate
  //               that the value field of the property of the current node can be null.
  //    PLAN-8.14: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "nonNullable" constraint then validate
  //               that the value field of the property of the current node cannot be null.
  //    PLAN-8.15: NB: validation of the "optional" constraint is achieved in PLAN-7.1.
  //    PLAN-8.16: NB: validation of the "mandatory" constraint is achieved in PLAN-7.1.
  //    PLAN-8.17: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "childType" constraint then validate
  //               that any and all children of the property of the current node must be of
  //               a type indicated by the constraint's parameter.
  //    PLAN-8.18: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "valueType" constraint then validate
  //               that the value field of the property of the current node must be of a type
  //               indicated by the constraint's parameter.
  //    PLAN-8.19: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "childCount" constraint then validate
  //               that the number of children of the property of the current node must equal
  //               that indicated by any of the constraint's parameters.
  //    PLAN-8.20a: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "minChildCount" constraint then validate
  //               that the number of children of the property of the current node must not
  //               be less than that indicated by the constraint's parameter.
  //    PLAN-8.20b: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "maxChildCount" constraint then validate
  //               that the number of children of the property of the current node must not
  //               be greater than that indicated by the constraint's parameter.


  // NOTE 1:
  //   When validating a property of a node, the definition of that property may have
  //   constraints (let's call these "direct" constraints). But there might also be additional
  //   constraints specified when that property was specified as a property of the node (let's
  //   call these "indirect" constraints).
  //   e.g. consider this _definition_ of object "foo" and its property "bar":
  //   {
  //      name: "foo",
  //      type: "foo_defn",
  //      properties: [
  //        {
  //          name: "bar",
  //          type: "bar_defn",
  //          properties: [
  //            {
  //              name: "__constraints", type: "...", children: [
  //                { name: "optional", type: "/__root__/primitives/constraints/optional" }
  //              ]
  //            }
  //          ]
  //        }
  //      ]
  //   }
  //   Given this definition, the property "bar" may have certain direct constraints from its
  //   definition (bar_defn), but it also has an indirect constraint specified where the property
  //   was defined (at foo:bar).
  //   So to validate this definition we must evaluate "bar" against its definition (bar_defn,
  //   and therefore its direct constraints) as well as against any additional indirect constraints
  //   applied in the declaration of the property "bar" too.

  RASSERT(pNode);
  RASSERT(pNode->eGetClassNo() == CN_QUERYNODE);
  RASSERT(pvArg);

  PlanNode* pRootNode = (PlanNode*) pvArg;
  pRootNode->vAssertHeader();
  RASSERT_TYPE(pRootNode, CN_ROOTNODE);

  // PLAN-1: Validate the name field:
  //    PLAN-1.1: the name field must not be null.
  vPlan_1_1(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PLAN-2: Validate the value field:
  //    No validations here as it is largely dependent on constraints
  //vPlan_2_1(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PLAN-3: Validate the type field:
  // (NB: we will explicitly validate the object against the definition in PLAN-7 and PLAN-8)
  //    PLAN-3.1: the type field must not be null.
  vPlan_3_1(pNode, sPath, iDepth, pvArg);

  //    PLAN-3.2: the type field must be an absolute path (start with a '/').
  vPlan_3_2(pNode, sPath, iDepth, pvArg);

  //    PLAN-3.3: the path in the type field must be resolvable to another node in the system.
  vPlan_3_3(pNode, sPath, iDepth, pvArg);

  //    PLAN-3.4: the path in the type field must be resolve to a node in /__root__/definitions.
  vPlan_3_4(pNode, sPath, iDepth, pvArg);

  //    PLAN-3.5: the path in the type field must not be a property path
  //              (i.e. it must not have a ':' in the path).
  vPlan_3_5(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PLAN-4: Validate the properties:
  // (NB: we will explicitly validate the properties against the definition in PLAN-7 and PLAN-8)
  //    No validations here as it is largely dependent on constraints
  //vPlan_4_1(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PLAN-5: Validate the constraints:
  // (NB: we will explicitly validate the constraints against the definition in PLAN-7 and PLAN-8)
  //    PLAN-5.1: Validate that there is no __constraint container in this objects properties.
  vPlan_5_1(pNode, sPath, iDepth, pvArg);

  //    PLAN-5.2: Validate that there are no properties that derive from a constraint definition.
  vPlan_5_2(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PLAN-6: Validate the children:
  // (NB: we will explicitly validate the children against the definition in PLAN-7 and PLAN-8)
  //    PLAN-6.1: the children must each have a unique name.
  vPlan_6_1(pNode, sPath, iDepth, pvArg);

  //    PLAN-6.2: the leaf nodes on a non-property path must be data sources
  vPlan_6_2(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PLAN-7: Validate the object against its definition properties (excluding constraints):
  //    PLAN-7.1: the node must have properties with names and types that represent all the properties
  //              defined in its definition (including all parents of the definition), excluding any
  //              properties that are marked as optional.
  vPlan_7_1(pNode, sPath, iDepth, pvArg);

  //    PLAN-7.2: the current node must not have any additional properties that
  //              aren't specified in its definition (including the definition's parents recursively).
  vPlan_7_2(pNode, sPath, iDepth, pvArg);

  // - - - - - - - - - - - - - - - - - - - - - -

  // PLAN-8: Validate the object against its definition constraints:
  // Please refer to NODE 1 above for additional complexities surrounding the validation
  // of constraints and the definition of the terms "direct" constraints and "indirect" constraints.
  //
  //    (Validating direct constraints):
  //    PLAN-8.1: If a node's definition contains the "noChildren" constraint
  //              then validate that the node has no children.
  vPlan_8_1(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.2: If a node's definition contains the "alwaysChildren" constraint
  //              then validate that the node must have at least one child.
  vPlan_8_2(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.3: If a node's definition contains the "nullable" constraint
  //              then validate that the value field of the node can be null.
  vPlan_8_3(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.4: If a node's definition contains the "nonNullable" constraint
  //              then validate that the value field of the node cannot be null.
  vPlan_8_4(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.5: NB: validation of the "optional" constraint is achieved in PLAN-7.1.
  vPlan_8_5(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.6: NB: validation of the "mandatory" constraint is achieved in PLAN-7.1.
  vPlan_8_6(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.7: If a node's definition contains the "childType" constraint
  //              then validate that any and all children of the node must be of a type indicated
  //              by the constraint's parameter.
  vPlan_8_7(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.8: If a node's definition contains the "valueType" constraint
  //              then validate that the value field of the node must be of a type indicated
  //              by the constraint's parameter.
  vPlan_8_8(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.9: If a node's definition contains the "childCount" constraint
  //              then validate that the number of children of the node must equal that indicated
  //              by any of the constraint's parameters.
  vPlan_8_9(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.10a: If a node's definition contains the "minChildCount" constraint
  //               then validate that the number of children of the node must not be less than
  //               that indicated by any of the constraint's parameters.
  vPlan_8_10a(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.10b: If a node's definition contains the "maxChildCount" constraint
  //               then validate that the number of children of the node must not be greater than
  //               that indicated by any of the constraint's parameters.
  vPlan_8_10b(pNode, sPath, iDepth, pvArg);

  //    (Validating indirect constraints):
  //    PLAN-8.11: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "noChildren" constraint then validate
  //               that the property of the current node has no children.
  vPlan_8_11(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.12: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "alwaysChildren" constraint then validate
  //               that the property of the current node must have at least one child.
  vPlan_8_12(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.13: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "nullable" constraint then validate
  //               that the value field of the property of the current node can be null.
  vPlan_8_13(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.14: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "nonNullable" constraint then validate
  //               that the value field of the property of the current node cannot be null.
  vPlan_8_14(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.15: NB: validation of the "optional" constraint is achieved in PLAN-7.1.
  vPlan_8_15(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.16: NB: validation of the "mandatory" constraint is achieved in PLAN-7.1.
  vPlan_8_16(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.17: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "childType" constraint then validate
  //               that any and all children of the property of the current node must be of
  //               a type indicated by the constraint's parameter.
  vPlan_8_17(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.18: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "valueType" constraint then validate
  //               that the value field of the property of the current node must be of a type
  //               indicated by the constraint's parameter.
  vPlan_8_18(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.19: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "childCount" constraint then validate
  //               that the number of children of the property of the current node must equal
  //               that indicated by any of the constraint's parameters.
  vPlan_8_19(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.20a: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "minChildCount" constraint then validate
  //               that the number of children of the property of the current node must not
  //               be less than that indicated by the constraint's parameter.
  vPlan_8_20a(pNode, sPath, iDepth, pvArg);

  //    PLAN-8.20b: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "maxChildCount" constraint then validate
  //               that the number of children of the property of the current node must not
  //               be greater than that indicated by the constraint's parameter.
  vPlan_8_20b(pNode, sPath, iDepth, pvArg);

  return Parser::GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /metadata                 */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphValidator::eValidateMetadataNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  // *********************************
  // Metadata Node Validation Rules:
  // *********************************
  //
  // META-1: Validate the name field:
  //
  // META-2: Validate the value field:
  //
  // META-3: Validate the type field:
  //
  // META-4: Validate the properties:
  //
  // META-5: Validate the constraints:
  //
  // META-6: Validate the children:
  //

  return Parser::GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /statistics               */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphValidator::eValidateStatisticsNode(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  // *********************************
  // Statistics Node Validation Rules:
  // *********************************
  //
  // STATS-1: Validate the name field:
  //
  // STATS-2: Validate the value field:
  //
  // STATS-3: Validate the type field:
  //
  // STATS-4: Validate the properties:
  //
  // STATS-5: Validate the constraints:
  //
  // STATS-6: Validate the children:
  //

  return Parser::GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* returns true if a given node has children with all of the names in the list*/
/*----------------------------------------------------------------------------*/
b1 GraphValidator::bContainsChildrenNamed(PlanNode* pNode, StzList* pNames)
{
  ASSERT(pNode);
  ASSERT(pNames);

  StzIter* pIter = pNames->pNewIter();
  while (pIter->bHasNext()) {
    Stz* szChildName = pIter->pNext();

    if (!GraphNav::pFindChild(szChildName, pNode)) {
      delete pIter;
      return false;
    }
  }

  delete pIter;
  return true;
}

/*----------------------------------------------------------------------------*/
/* returns true if a given node has the Optional constraint as a property     */
/*----------------------------------------------------------------------------*/
b1 GraphValidator::bHasOptionalConstraint(PlanNode* pNode)
{
  RASSERT(pNode);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return false;   // no constraints
  }

  // Iterate the children of this node and look for a node with a type
  // that matches the Optional constraint.
  b1 bResult = false;
  PlanNodeIter* pIter = pConstraints->pNewChildrenIter();

  while (pIter->bHasNext()) {
    PlanNode* pItem = pIter->pNext();

    if (pItem->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_OPTIONAL_PATH))) {
      bResult = true;
      break;
    }
  }

  delete pIter;
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* Looks up and retrieves a constraint of a node if it exists. The node must  */
/* have a constraints container and the constraint must be a child of that.   */
/* Otherwise this function returns nullptr.                                   */
/*----------------------------------------------------------------------------*/
PlanNode* GraphValidator::pGetConstraint(Stz* szConstraintDefn, DefinitionNode* pNode)
{
  return pGetConstraint(szConstraintDefn, pNode, nullptr, false);
}

/*----------------------------------------------------------------------------*/
/* Looks up and retrieves a constraint of a node if it exists. The node must  */
/* have a constraints container and the constraint must be a child of that.   */
/* If bWalkInheritance is true then this function will keep looking up the    */
/* inheritance hierarchy before giving up and returning nullptr.              */
/*----------------------------------------------------------------------------*/
PlanNode* GraphValidator::pGetConstraint(Stz* szConstraintDefn, DefinitionNode* pDefn, PlanNode* pRootNode, b1 bWalkInheritance)
{
  RASSERT(pDefn);
  RASSERT_TYPE(pDefn, CN_DEFINITIONNODE);
  RASSERT(szConstraintDefn);
  RASSERT(szConstraintDefn->nLength() > 0);
  RASSERT((bWalkInheritance && pRootNode) || !bWalkInheritance);

  PlanNode* pResult = nullptr;

  RASSERT(!pDefn->szType() || !bWalkInheritance);

  do {
    if (pDefn->nProperties() == 0) {
      continue;
    }

    PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pDefn);
    if (!pConstraints || pConstraints->nChildren() == 0) {
      continue;
    }

    PlanNodeIter* pIter = pConstraints->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pConstraint = pIter->pNext();

      if (pConstraint->szType()->bEqual(szConstraintDefn)) {
        pResult = pConstraint;
        break;
      }
    }

    delete pIter;

  } while ( bWalkInheritance &&
            !pResult &&
            (pDefn = static_cast<DefinitionNode*>(GraphNav::pGetParent(pDefn, pRootNode))) &&
            !pDefn->szPath()->bEqual(const_cast<Stz*>(C_DEFINITIONS_PATH)) );

  return pResult;
}


/*============================================================================*/
/* Individual validation functions below here                                 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                ROOT-1.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vRoot_1_1(PlanNode* pRootNode)
{
  // ROOT-1: Validate the name field:
  // ROOT-1.1: the name field must be "__root__".
  RASSERT(pRootNode->szName());
  RASSERT(pRootNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME)));
}

/*----------------------------------------------------------------------------*/
/*                                ROOT-2.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vRoot_2_1(PlanNode* pRootNode)
{
  // ROOT-2: Validate the value field:
  // ROOT-2.1: the value field must not be null and it must contains a string.
  RASSERT(pRootNode->szValue());
  RASSERT(pRootNode->xValue().m_pText->bIsClassNo(CN_TEXT));
  RASSERT(pRootNode->xValue().m_bMustFree);
  RASSERT(pRootNode->szValue()->nLength() > 0);
}

/*----------------------------------------------------------------------------*/
/*                                ROOT-3.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vRoot_3_1(PlanNode* pRootNode)
{
  // ROOT-3: Validate the type field:
  // ROOT-3.1: validate that the type field is not null and has a value of
  //           /__root__/primitives/root
  RASSERT(pRootNode->szType());
  RASSERT(pRootNode->szType()->bEqual(const_cast<Stz*>(C_ROOT_DEFN_PATH)));
}

/*----------------------------------------------------------------------------*/
/*                                ROOT-4.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vRoot_4_1(PlanNode* pRootNode)
{
  // ROOT-4: Validate the properties:
  // ROOT-4.1: validate that the properties field is null.
  RASSERT(!pRootNode->bHasProperties());
  RASSERT(!pRootNode->m_pProperties);
}

/*----------------------------------------------------------------------------*/
/*                                ROOT-5.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vRoot_5_1(PlanNode* pRootNode)
{
  // ROOT-5: Validate the constraints:
  //    Nothing needed here due to ROOT-4.1.
  UNUSED(pRootNode);
}

/*----------------------------------------------------------------------------*/
/*                                ROOT-6.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vRoot_6_1(PlanNode* pRootNode)
{
  // ROOT-6: Validate the children:
  // ROOT-6.1: validate that the children field is not null.
  RASSERT(pRootNode->m_pChildren);
}

/*----------------------------------------------------------------------------*/
/*                                ROOT-6.2                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vRoot_6_2(PlanNode* pRootNode)
{
  // ROOT-6: Validate the children:
  // ROOT-6.2: validate that the children field has 5 items named:
  //           "primitives", "definitions", "plan", "metadata" and "statistics".
  RASSERT(pRootNode->nChildren() == 5);

  StzList* pRootChildrenList = new StzList;
  i8 i = 0;
  Stz* szName;
  while ((szName = const_cast<Stz*>(C_ROOT_CHILDREN_NAMES[i++])) != nullptr) {
    pRootChildrenList->vAppend(szName);
  }

  RASSERT(bContainsChildrenNamed(pRootNode, pRootChildrenList));
  delete pRootChildrenList;
}

/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                PRIM-1.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPrim_1_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PRIM-1: Validate the name field:
  // PRIM-1.1: the name field must not be null.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(pNode->szName());
  RASSERT(pNode->szName()->nLength() > 0);
}

/*----------------------------------------------------------------------------*/
/*                                PRIM-2.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPrim_2_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PRIM-2: Validate the value field:
  // PRIM-2.1: the value field must not be null and it contains a string.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(pNode->xValue().m_pText);
  RASSERT(pNode->xValue().m_pText->bIsClassNo(CN_TEXT));
  RASSERT(pNode->xValue().m_bMustFree);
  RASSERT(pNode->szValue());
  RASSERT(pNode->szValue()->nLength());
}

/*----------------------------------------------------------------------------*/
/*                                PRIM-3.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPrim_3_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PRIM-3: Validate the type field:
  // PRIM-3.1: validate that the type field is empty.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(!pNode->szType() ||
          (
              pNode->szName()->bEqual(const_cast<Stz*>(C_PRIMITIVES_NAME)) &&
              pNode->szType()->bEqual(const_cast<Stz*>(C_TRUNK_DEFN_PATH))
          ) ||
          (
              pNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME)) &&
              pNode->szType()->bEqual(const_cast<Stz*>(C_ROOT_DEFN_PATH))
          )
  );
}

/*----------------------------------------------------------------------------*/
/*                                PRIM-4.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPrim_4_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PRIM-4: Validate the properties:
  // PRIM-4.1: validate that the properties field is null.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(!pNode->m_pProperties);
  RASSERT(!pNode->bHasProperties());
}

/*----------------------------------------------------------------------------*/
/*                                PRIM-5.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPrim_5_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PRIM-5: Validate the constraints:
  //    Nothing needed here due to PRIM-4.1.

  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);
}

/*----------------------------------------------------------------------------*/
/*                                PRIM-6.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPrim_6_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PRIM-6: Validate the children:
  // PRIM-6.1: validate that the children field is either null or has 1 or more children.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(!pNode->m_pChildren || pNode->nChildren() > 0);
}

/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                DEFN-1.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_1_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-1: Validate the name field:
  // DEFN-1.1: the name field must not be null.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(pNode->szName());
  RASSERT(pNode->szName()->nLength() > 0);
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-2.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_2_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-2: Validate the value field:
  // DEFN-2.1: has a non-null string value if the type field is null.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (!pNode->szType()) {
    RASSERT(
        pNode->xValue().m_pText &&
            pNode->xValue().m_pText->eGetClassNo() == CN_TEXT &&
            pNode->szValue() &&
            pNode->szValue()->nLength() > 0
    );
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-2.2                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_2_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-2: Validate the value field:
  // DEFN-2.2: has a null or non-null value if the type field is not null.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->szType()) {
    // Bit of a noop here
    RASSERT(pNode->szType()->nLength() > 0);
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-3.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_3_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-3: Validate the type field:
  // DEFN-3.1: has a null type field when the node is not a direct or indirect property
  //           of another node.
  //           i.e., the node is on a direct inheritence path back to /definitions.

  UNUSED(iDepth);
  UNUSED(pvArg);

  b1 bIsPropertyPath = sPath.bContains(":"_Stz);

  if (!bIsPropertyPath) {
    RASSERT(!pNode->szType() || sPath.bEqual(const_cast<Stz*>(C_DEFINITIONS_NAME)));
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-3.3                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_3_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-3: Validate the type field:
  // DEFN-3.2: has a non-null type field when the node (or any parent above it) is
  //           a direct or indirect property of a parent node. (i.e., the node is
  //           on a "property path").

  UNUSED(iDepth);
  UNUSED(pvArg);

  b1 bIsPropertyPath = sPath.bContains(":"_Stz);

  if (bIsPropertyPath) {
    RASSERT(pNode->szType());
    RASSERT(pNode->szType()->nLength() > 0);
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-3.3                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_3_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-3: Validate the type field:
  // DEFN-3.3: when the type field is non-null, it must be an absolute path.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->szType()) {
    RASSERT(pNode->szType()->bBeginsWith("/"_Stz));
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-3.4                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_3_4(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-3: Validate the type field:
  // DEFN-3.4: when the type field is non-null, the path must be resolvable to another
  //           node in the system.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  if (pNode->szType()) {
    PlanNode* pItem = GraphNav::pFind(pNode->szType(), pRootNode);
    RASSERT(pItem);
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-3.5                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_3_5(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-3: Validate the type field:
  // DEFN-3.5: when the type field is non-null, the node that it resolves to must be
  //           another definition or a primitive node.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  if (pNode->szType()) {
    PlanNode* pItem = GraphNav::pFind(pNode->szType(), pRootNode);
    RASSERT(pItem);

    RASSERT(pNode->szType()->bBeginsWith(const_cast<Stz*>(C_DEFINITIONS_PATH)) ||
            pNode->szType()->bBeginsWith(const_cast<Stz*>(C_PRIMITIVES_PATH)) );
    RASSERT(pItem->eGetClassNo() == CN_DEFINITIONNODE ||
            pItem->eGetClassNo() == CN_PRIMITIVENODE);
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-4.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_4_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-4: Validate the properties:
  // DEFN-4.1: all properties of the node must have a unique name with respect to
  //           other properties defined for the node.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() > 1) {
    PlanNodeIter* pIter1 = pNode->pNewPropertyIter();

    while (pIter1->bHasNext()) {
      PlanNode* pItem1 = pIter1->pNext();

      // Horrible O(N^2) loop
      PlanNodeIter* pIter2 = pNode->pNewPropertyIter();
      while (pIter2->bHasNext()) {
        PlanNode *pItem2 = pIter2->pNext();

        if (pItem1 == pItem2) {
          continue;
        }

        RASSERT(!pItem1->szName()->bEqual(pItem2->szName()));
      }
      delete pIter2;
    }

    delete pIter1;
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-4.2                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_4_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-4: Validate the properties:
  // DEFN-4.2: all properties of the node must have a unique name with respect to
  //           other properties defined in parent nodes that the current node inherits
  //           from. The only exception here is the "__constraints" node.
  //
  // e.g. node /a/b/c has a property :d. We need to check that there isn't a property named :d
  // in /a/b or /a either.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  if (pNode->nProperties() > 1) {
    PlanNodeIter* pPropIter = pNode->pNewPropertyIter();

    while (pPropIter->bHasNext()) {
      PlanNode* pProp = pPropIter->pNext();
      Stz* pPropName = pProp->szName();

      if (pPropName->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME))) {
        // ignore this
        continue;
      }

      DefinitionNode* pInheritenceNode = static_cast<DefinitionNode*>(pNode);

      // I don't think there can be a case where the type is not null.
      RASSERT(!pNode->szType());

      while (!pInheritenceNode->szPath()->bEqual(const_cast<Stz*>(C_DEFINITIONS_PATH))) {
        // Get the parent
        DefinitionNode* pParentDefn = static_cast<DefinitionNode*>(GraphNav::pGetParent(pInheritenceNode, pRootNode));
        RASSERT(pParentDefn);

        if (pParentDefn->bHasProperties()) {

          // Check the properties of this node to make sure we don't have one with the same name
          PlanNodeIter *pParentPropIter = pParentDefn->pNewPropertyIter();
          while (pParentPropIter->bHasNext()) {
            PlanNode* pParentProp = pParentPropIter->pNext();
            RASSERT(!pPropName->bEqual(pParentProp->szName()));
          }
          delete pParentPropIter;
        }

        pInheritenceNode = pParentDefn;
      }
    }

    delete pPropIter;
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-4.3                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_4_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-4.3: all properties (except "__constraints") must have a non-null type field.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  PlanNodeIter *pPropIter = pNode->pNewPropertyIter();

  while (pPropIter->bHasNext()) {
    PlanNode *pProp = pPropIter->pNext();
    Stz *pPropName = pProp->szName();

    if (pPropName->bEqual(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_NAME))) {
      // ignore this
      continue;
    }

    RASSERT(pProp->szType());
  }
  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.1: any constraints must exist under a node named "__constraints" which
  //           itself is a property of the current node.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  // First check that there are no constraints that are a child of a property that is not
  // the constraints container.
  // Iterate each property
  {
    PlanNodeIter *pPropIter = pNode->pNewPropertyIter();
    while (pPropIter->bHasNext()) {
      PlanNode *pPropNode = pPropIter->pNext();

      if (pPropNode->szName()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_NAME))) {
        // Ignore the constraints container property
        continue;
      }

      // Got a non-contraints container property. Ensure it doesn't have any children that
      // are constraints.
      if (!pPropNode->bHasChildren()) {
        continue;
      }

      PlanNodeIter *pChildIter = pPropNode->pNewChildrenIter();
      while (pChildIter->bHasNext()) {
        PlanNode *pChildNode = pChildIter->pNext();

        // The type of this child cannot be a constraint.
        if (pChildNode->szType()) {
          RASSERT(!pChildNode->szType()->bBeginsWith(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_PATH)))
        }
      }
      delete pChildIter;
    }
    delete pPropIter;
  }

  // Seconly, check that all the children under the constraints container are actually constraints.
  {
    RASSERT(pConstraints);
    if (pConstraints->bHasChildren()) {

      PlanNodeIter *pConstraintIter = pConstraints->pNewChildrenIter();
      while (pConstraintIter->bHasNext()) {
        PlanNode *pConstraintsChild = pConstraintIter->pNext();

        // The type of this child must be a constraint.
        if (pConstraintsChild->szType()) {
          RASSERT(pConstraintsChild->szType()->bBeginsWith(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_PATH)))
        }
      }
      delete pConstraintIter;
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.2                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.2: if a __constraints container exists then it must have 1 or more children.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  RASSERT(pConstraints->nChildren() > 0);
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.3                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.3: validate that if any of the following constraints exist as children
  //           under a __constraints container, then those constraints must also have
  //           one or more child nodes (being a parameter of the constraint):
  //             - childType
  //             - valueType
  //             - childCount

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

  while (pConstraintsIter->bHasNext()) {
    PlanNode* pConstraint = pConstraintsIter->pNext();

    RASSERT(pConstraint->szType());

    if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH)) ||
        pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_VALUETYPE_PATH))  ||
        pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDCOUNT_PATH))
        ) {

      // The constraint must have at least one child
      RASSERT(pConstraint->nChildren() >= 1);
    }
  }

  delete pConstraintsIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.4                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_4(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.4: if any of the constraints specified in DEFN-5.3 exist, then validate that
  //           their parameter (child) nodes must have non-null name, type and value fields.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

  while (pConstraintsIter->bHasNext()) {
    PlanNode* pConstraint = pConstraintsIter->pNext();

    RASSERT(pConstraint->szType());
    if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH)) ||
        pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_VALUETYPE_PATH)) ||
        pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDCOUNT_PATH)) ) {

      RASSERT(pConstraint->szName());
      RASSERT(pConstraint->szName()->nLength() > 0);

      RASSERT(pConstraint->szType());
      RASSERT(pConstraint->szType()->nLength() > 0);

      // Check the children of these constraints, which are the constraint parameters.
      RASSERT(pConstraint->nChildren() > 0);

      PlanNodeIter* pParamIter = pConstraint->pNewChildrenIter();
      while (pParamIter->bHasNext()) {
        PlanNode* pParamNode = pParamIter->pNext();

        RASSERT(pParamNode->szName());
        RASSERT(pParamNode->szName()->nLength() > 0);

        RASSERT(pParamNode->szType());
        RASSERT(pParamNode->szType()->nLength() > 0);

        if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH)) ||
            pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_VALUETYPE_PATH))) {
          // Value must be a string
          RASSERT(pParamNode->xValue().m_pText);
          RASSERT(pParamNode->xValue().m_bMustFree);
          RASSERT(pParamNode->szValue());
          RASSERT(pParamNode->szValue()->nLength() > 0);

        } else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDCOUNT_PATH))) {
          // Value must be an integer
          RASSERT(pParamNode->iValue() >= 0);

        } else {
          NEVER_REACH
        }
      }
      delete pParamIter;
    }
  }

  delete pConstraintsIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.5                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_5(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.5: If the childType constraint is used then validate that its child parameter
  //           must have a value field that is a path that can be resolved to another
  //           definition node.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

  while (pConstraintsIter->bHasNext()) {
    PlanNode* pConstraint = pConstraintsIter->pNext();

    RASSERT(pConstraint->szType());
    if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH)) ) {

      RASSERT(pConstraint->nChildren() > 0);

      PlanNodeIter* pParamIter = pConstraint->pNewChildrenIter();
      while (pParamIter->bHasNext()) {
        PlanNode* pParamNode = pParamIter->pNext();

        Stz* szPath = pParamNode->szType();
        RASSERT(szPath);
        RASSERT(szPath->nLength() > 0);
        RASSERT(szPath->bBeginsWith(const_cast<Stz*>(C_DEFINITIONS_PATH)));

        PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
        PlanNode* pItem = GraphNav::pFind(szPath, pRootNode);

        RASSERT(pItem);
        RASSERT_TYPE(pItem, CN_DEFINITIONNODE);
      }
      delete pParamIter;
    }
  }

  delete pConstraintsIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.6                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_6(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.6: If the valueType constraint is used then validate that its child parameter
  //           must have a value field that is a path that can be resolved to a
  //           primitive node, and that that type field is a subclass of valueObject.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

  while (pConstraintsIter->bHasNext()) {
    PlanNode* pConstraint = pConstraintsIter->pNext();

    RASSERT(pConstraint->szType());
    if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_VALUETYPE_PATH)) ) {

      RASSERT(pConstraint->nChildren() == 1);

      // Iterate the children
      PlanNodeIter* pParamIter = pConstraint->pNewChildrenIter();
      while (pParamIter->bHasNext()) {
        PlanNode* pParamNode = pParamIter->pNext();

        RASSERT(pParamNode->xValue().m_pText);
        RASSERT(pParamNode->xValue().m_bMustFree);
        RASSERT(pParamNode->szValue());
        RASSERT(pParamNode->szValue()->nLength() > 0);
        RASSERT(pParamNode->szValue()->bBeginsWith(const_cast<Stz*>(C_PRIMITIVES_PATH)));

        RASSERT(pParamNode->szType());
        RASSERT(pParamNode->szType()->bBeginsWith(const_cast<Stz*>(C_VALUE_OBJ_DEFN)));

        PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
        PlanNode* pItem = GraphNav::pFind(pParamNode->szValue(), pRootNode);

        RASSERT(pItem);
        RASSERT_TYPE(pItem, CN_PRIMITIVENODE);
      }
      delete pParamIter;
    }
  }

  delete pConstraintsIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.7                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_7(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.7: validate that the following combinations of constraints cannot be applied
  //           in the __constraints container for a given object:
  //             - (noChildren/alwaysChildren),
  //             - (nullable/nonnullable),
  //             - (optional/mandatory)

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  b1 bGotNoChildren     = false;
  b1 bGotAlwaysChildren = false;
  b1 bGotNullable       = false;
  b1 bGotNonNullable    = false;
  b1 bGotOptional       = false;
  b1 bGotMandatory      = false;

  PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

  while (pConstraintsIter->bHasNext()) {
    PlanNode* pConstraint = pConstraintsIter->pNext();

    RASSERT(pConstraint->szType());
    if      (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_NOCHILDREN_PATH)))     { bGotNoChildren = true;     }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_ALWAYSCHILDREN_PATH))) { bGotAlwaysChildren = true; }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_NULLABLE_PATH)))       { bGotNullable = true;       }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_NONNULLABLE_PATH)))    { bGotNonNullable = true;    }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_OPTIONAL_PATH)))       { bGotOptional = true;       }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_MANDATORY_PATH)))      { bGotMandatory = true;      }
  }

  delete pConstraintsIter;

  RASSERT(!(bGotNoChildren && bGotAlwaysChildren));
  RASSERT(!(bGotNullable && bGotNonNullable));
  RASSERT(!(bGotOptional && bGotMandatory));
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.8                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_8(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.8: validate that the constraints combinations specified in DEFN-5.7 cannot occur
  //           with any constraints inherited by the current node.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  b1 bGotNoChildren     = false;
  b1 bGotAlwaysChildren = false;
  b1 bGotNullable       = false;
  b1 bGotNonNullable    = false;
  b1 bGotOptional       = false;
  b1 bGotMandatory      = false;

  PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

  while (pConstraintsIter->bHasNext()) {
    PlanNode* pConstraint = pConstraintsIter->pNext();

    RASSERT(pConstraint->szType());
    if      (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_NOCHILDREN_PATH)))     { bGotNoChildren     = true; }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_ALWAYSCHILDREN_PATH))) { bGotAlwaysChildren = true; }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_NULLABLE_PATH)))       { bGotNullable       = true; }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_NONNULLABLE_PATH)))    { bGotNonNullable    = true; }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_OPTIONAL_PATH)))       { bGotOptional       = true; }
    else if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_MANDATORY_PATH)))      { bGotMandatory      = true; }
  }

  delete pConstraintsIter;

  if (!(bGotNoChildren && bGotAlwaysChildren && bGotNullable &&
        bGotNonNullable && bGotOptional && bGotMandatory) ) {
    // We don't have any of these constraints so we don't need to check up the hierarchy tree
    return;
  }

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
  DefinitionNode* pDefnParent = static_cast<DefinitionNode*>(pNode);

  // Get the first parent. If we have a non-null type then the first parent is
  // this resolved type. If the type is null then the first parent is the
  // the logical parent node of this node.
  if (pDefnParent->szType()) {
    pDefnParent = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));

  } else {
    // No type specified, so it inherits from its parent node
    pDefnParent = static_cast<DefinitionNode*>(GraphNav::pGetParent(pDefnParent, pRootNode));
  }

  RASSERT(pDefnParent);

  while (pDefnParent && !pDefnParent->szPath()->bEqual(const_cast<Stz*>(C_DEFINITIONS_PATH))) {

    // does this parent node have any constraints?
    PlanNode* pParentConstraints = GraphNav::pFindChild(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pDefnParent);

    if (pParentConstraints) {
      // Iterate the constraints of the parent node
      pConstraintsIter = pParentConstraints->pNewChildrenIter();

      while (pConstraintsIter->bHasNext()) {
        PlanNode *pConstraint = pConstraintsIter->pNext();

        // Check the invalid combinations of constraints

        if (bGotNoChildren &&
            pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_ALWAYSCHILDREN_PATH))) {
          RASSERT(0);
        }

        if (bGotAlwaysChildren &&
            pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NOCHILDREN_PATH))) {
          RASSERT(0);
        }

        if (bGotNullable && pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NONNULLABLE_PATH))) {
          RASSERT(0);
        }

        if (bGotNonNullable && pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NULLABLE_PATH))) {
          RASSERT(0);
        }

        if (bGotOptional && pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_MANDATORY_PATH))) {
          RASSERT(0);
        }

        if (bGotMandatory && pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_OPTIONAL_PATH))) {
          RASSERT(0);
        }
      }

      delete pConstraintsIter;
    }

    // get the parent node
    pDefnParent = static_cast<DefinitionNode*>(GraphNav::pGetParent(pDefnParent, pRootNode));
  }
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.9                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_9(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.9: validate that the same constraint cannot be specified more than once
  //           in the current node.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  if (pConstraints->nChildren() <= 1) {
    return;
  }

  PlanNodeIter* pConstraintsIter1 = pConstraints->pNewChildrenIter();

  while (pConstraintsIter1->bHasNext()) {
    PlanNode* pConstraint1 = pConstraintsIter1->pNext();

    RASSERT(pConstraint1->szType());

    // Iterate the constraints again looking for a second copy of this constraint.
    // Yes, this is an ugly O(N^2) loop, but it should be small.
    PlanNodeIter* pConstraintsIter2 = pConstraints->pNewChildrenIter();

    while (pConstraintsIter2->bHasNext()) {
      PlanNode* pConstraint2 = pConstraintsIter2->pNext();

      RASSERT(pConstraint2->szType());

      if (pConstraint1 == pConstraint2) {
        // ignore
        continue;
      }

      // Their types should not match
      RASSERT(!pConstraint1->szType()->bEqual(pConstraint2->szType()));
    }
    delete pConstraintsIter2;


  }
  delete pConstraintsIter1;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.10                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_10(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  // DEFN-5.10: validate that the same constraint cannot be specified more than once
  //            when considering all inherited constraints of the current node.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);

  if (!pConstraints) {
    return;
  }

  if (pConstraints->nChildren() <= 1) {
    return;
  }

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
  RASSERT_TYPE(pRootNode, CN_ROOTNODE);

  PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

  // Iterate each constraint in the target node
  while (pConstraintsIter->bHasNext()) {
    PlanNode* pConstraint = pConstraintsIter->pNext();

    RASSERT(pConstraint->szType());

    // Go back through the inheritence tree and check that this constraint hasn't
    // already been specified.
    // Note: If the current node has a non-null type then we need to resolve
    // that type and go back through its tree, not the current node's parent.

    DefinitionNode* pParent;

    // Get the first parent
    if (pNode->szType()) {
      // resolve the type first
      pParent = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
      RASSERT(pParent);

    } else {
      // No type specified, so it inherits from its parent node
      DefinitionNode* pCurrentNode = static_cast<DefinitionNode*>(pNode);
      pParent = static_cast<DefinitionNode*>(GraphNav::pGetParent(pCurrentNode, pRootNode));
    }

    RASSERT(pParent);
    RASSERT_TYPE(pParent, CN_DEFINITIONNODE);

    while (!pParent->szPath()->bEqual(const_cast<Stz*>(C_DEFINITIONS_PATH))) {

      // get the constraint container of this node
      PlanNode* pParentConstraints = GraphNav::pFindChild(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pParent);

      if (pParentConstraints && pConstraints->nChildren() > 0) {
        // Iterate the constraints of this node and compare to pConstraint->szType() above.
        // They should not be the same.
        PlanNodeIter *pParentConstraintIter = pParentConstraints->pNewChildrenIter();

        while (pParentConstraintIter->bHasNext()) {
          PlanNode *pParentConstraint = pParentConstraintIter->pNext();
          RASSERT(pParentConstraint->szType());
          RASSERT(!pParentConstraint->szType()->bEqual(pConstraint->szType()));
        }
        delete pParentConstraintIter;
      }

      // get the next parent
      pParent = static_cast<DefinitionNode*>(GraphNav::pGetParent(pParent, pRootNode));
    }
  }
  delete pConstraintsIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.11                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_11(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.11: If an object definition has a property where the property's type field
  //               is non null, validate that any constraints of that property must exist
  //               under a node named "__constraints" which itself is a property of the
  //               property in question.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property of the current node. We're only interested in those
  // properties that have a non-null type field.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode* pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    if (pProp->nProperties() == 0) {
      continue;
    }

    // Get the constraint container of this property, if it exists.
    PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pProp);

    // Check each property of this property to ensure that it doesn't have a
    // type field deriving from a constraint type when it is outside of the
    // constraint container.
    PlanNodeIter* pPropPropIter = pProp->pNewPropertyIter();
    while (pPropPropIter->bHasNext()) {
      PlanNode* pPropProp = pPropPropIter->pNext();

      if (pPropProp == pConstraints) {
        continue;   // ignore the constraints container
      }

      // We shouldn't have a constraint outside of its container.
      RASSERT(pPropProp->szType());
      RASSERT(!pPropProp->szType()->bBeginsWith(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_PATH)));
    }
    delete pPropPropIter;
  }
  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.12                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_12(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.12: If an object definition has a property where the property's type field
  //               is non null, validate that if a constraints container exists in that
  //               property then it must have one or more children.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode* pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pProp);
    if (!pConstraints) {
      continue;   // ignore
    }

    RASSERT(pConstraints->nChildren() > 0);
  }
  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.13                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_13(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.13: If an object definition has a property where the property's type field
  //               is non null, validate that if any of the following constraints exist as
  //               children under a __constraints container of that property, then those
  //               constraints must also have one or more child nodes (being a parameter of
  //               the constraint):
  //                - childType
  //                - valueType
  //                - childCount

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode* pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pProp);
    if (!pConstraints) {
      continue;   // ignore
    }

    RASSERT(pConstraints->nChildren() > 0);
    // Iterate the constraints in the container
    PlanNodeIter* pConstraintIter = pConstraints->pNewChildrenIter();

    while (pConstraintIter->bHasNext()) {
      PlanNode* pConstraint = pConstraintIter->pNext();

      if (pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH)) ||
          pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_VALUETYPE_PATH))  ||
          pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDCOUNT_PATH))
          ) {

        // The constraint must have at least one child
        RASSERT(pConstraint->nChildren() >= 1);
      }
    }
    delete pConstraintIter;
  }
  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.14                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_14(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.14: If an object definition has a property where the property's type field
  //               is non null, validate that if any of the constraints specified in
  //               DEFN-5.13 are specified for the property in question, then validate that
  //               their parameter nodes must have non-null name, type and value fields.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode* pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pProp);
    if (!pConstraints) {
      continue;   // ignore
    }

    RASSERT(pConstraints->nChildren() > 0);
    // Iterate the constraints in the container
    PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

    while (pConstraintsIter->bHasNext()) {
      PlanNode* pConstraint = pConstraintsIter->pNext();

      if ( pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH)) ||
           pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_VALUETYPE_PATH)) ||
           pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDCOUNT_PATH)) ) {

        RASSERT(pConstraint->nChildren() > 0);
        PlanNodeIter* pParamIter = pConstraint->pNewChildrenIter();

        // Iterate each param of the constraint
        while (pParamIter->bHasNext()) {
          PlanNode* pParam = pParamIter->pNext();
          RASSERT(pParam->szName());
          RASSERT(pParam->szName()->nLength() > 0);
          RASSERT(pParam->szType());
          RASSERT(pParam->szType()->nLength() > 0);
          RASSERT(!pParam->bIsNullValue());
          RASSERT(pParam->eValueType() != Value::VT_UNKNOWN);
        }
        delete pParamIter;
      }
    }
    delete pConstraintsIter;
  }
  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.15                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_15(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.15: If an object definition has a property where the property's type field
  //               is non null, validate that if the childType constraint is used on the property
  //               that its child parameter must have a value field that is a path that can be
  //               resolved to another definition node.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode* pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pProp);
    if (!pConstraints) {
      continue;   // ignore
    }

    RASSERT(pConstraints->nChildren() > 0);
    // Iterate the constraints in the container
    PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

    while (pConstraintsIter->bHasNext()) {
      PlanNode* pConstraint = pConstraintsIter->pNext();

      if ( pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH)) ) {

        RASSERT(pConstraint->nChildren() > 0);
        PlanNodeIter* pParamIter = pConstraint->pNewChildrenIter();

        // Iterate each param of the constraint
        while (pParamIter->bHasNext()) {
          PlanNode* pParam = pParamIter->pNext();

          RASSERT(pParam->m_xValue.m_pText);
          RASSERT(pParam->m_xValue.m_pText->bIsClassNo(CN_TEXT));
          RASSERT(pParam->m_xValue.m_bMustFree);

          Stz* szVal = pParam->szValue();
          RASSERT(szVal);
          RASSERT(szVal->nLength() > 0);
          RASSERT(szVal->bBeginsWith(const_cast<Stz*>(C_DEFINITIONS_PATH)));

          PlanNode* pChildDef = GraphNav::pFind(szVal, pRootNode);
          RASSERT(pChildDef);
          RASSERT_TYPE(pChildDef, CN_DEFINITIONNODE);
        }
        delete pParamIter;
      }
    }
    delete pConstraintsIter;
  }
  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.16                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_16(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.16: If an object definition has a property where the property's type field
  //               is non null, validate that if the valueType constraint is used on the property
  //               that its child parameter must have a value field that is a path that can be resolved
  //               to another definition node, and that that path is a subclass of valueObject.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode* pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pProp);
    if (!pConstraints) {
      continue;   // ignore
    }

    RASSERT(pConstraints->nChildren() > 0);
    // Iterate the constraints in the container
    PlanNodeIter* pConstraintsIter = pConstraints->pNewChildrenIter();

    while (pConstraintsIter->bHasNext()) {
      PlanNode* pConstraint = pConstraintsIter->pNext();

      if ( pConstraint->szType()->bEqual(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH)) ) {

        RASSERT(pConstraint->nChildren() > 0);
        PlanNodeIter* pParamIter = pConstraint->pNewChildrenIter();

        // Iterate each param of the constraint
        while (pParamIter->bHasNext()) {
          PlanNode* pParam = pParamIter->pNext();

          RASSERT(pParam->m_xValue.m_pText);
          RASSERT(pParam->m_xValue.m_pText->bIsClassNo(CN_TEXT));
          RASSERT(pParam->m_xValue.m_bMustFree);

          Stz* szVal = pParam->szValue();
          RASSERT(szVal);
          RASSERT(szVal->nLength() > 0);
          RASSERT(szVal->bBeginsWith(const_cast<Stz*>(C_VALUE_OBJ_DEFN)));

          PlanNode* pChildDef = GraphNav::pFind(szVal, pRootNode);
          RASSERT(pChildDef);
          RASSERT_TYPE(pChildDef, CN_DEFINITIONNODE);
        }
        delete pParamIter;
      }
    }
    delete pConstraintsIter;
  }
  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.17                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_17(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.17: If an object definition has a property where the property's type field
  //               is non null, and that property has constraints then validate that the following
  //               combinations of constraints cannot occur in the __constraints container
  //               for that property:
  //                - (noChildren/alwaysChildren),
  //                - (nullable/nonnullable),
  //                - (optional/mandatory)

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode *pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    // Get the constraints container for this property
    PlanNode *pConstraints = GraphNav::pFindProperty(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_NAME), pProp);

    if (!pConstraints) {
      continue;
    }

    b1 bGotNoChildren = false;
    b1 bGotAlwaysChildren = false;
    b1 bGotNullable = false;
    b1 bGotNonNullable = false;
    b1 bGotOptional = false;
    b1 bGotMandatory = false;

    // Iterate the constraints in this constraint container
    PlanNodeIter *pConstraintsIter = pConstraints->pNewChildrenIter();

    while (pConstraintsIter->bHasNext()) {
      PlanNode *pConstraint = pConstraintsIter->pNext();

      RASSERT(pConstraint->szType());

      if      (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NOCHILDREN_PATH)))     { bGotNoChildren = true;     }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_ALWAYSCHILDREN_PATH))) { bGotAlwaysChildren = true; }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NULLABLE_PATH)))       { bGotNullable = true;       }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NONNULLABLE_PATH)))    { bGotNonNullable = true;    }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_OPTIONAL_PATH)))       { bGotOptional = true;       }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_MANDATORY_PATH)))      { bGotMandatory = true;      }
    }

    delete pConstraintsIter;

    // Check for the invalid combinations
    RASSERT(!(bGotNoChildren && bGotAlwaysChildren));
    RASSERT(!(bGotNullable && bGotNonNullable));
    RASSERT(!(bGotOptional && bGotMandatory));
  }

  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.18                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_18(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.18: If an object definition has a property where the property's type field is
  //               non null and that property has a __constraint container, then validate that
  //               any constraints specified are not incompatible with any constraints already
  //               specified in the definition of the property (including that definitions
  //               inheritance hierarchy).

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode *pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    // Get the constraints container for this property
    PlanNode *pConstraints = GraphNav::pFindProperty(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_NAME), pProp);

    if (!pConstraints) {
      continue;
    }

    b1 bGotNoChildren     = false;
    b1 bGotAlwaysChildren = false;
    b1 bGotNullable       = false;
    b1 bGotNonNullable    = false;
    b1 bGotOptional       = false;
    b1 bGotMandatory      = false;

    // Iterate the constraints in this constraint container
    PlanNodeIter *pConstraintsIter = pConstraints->pNewChildrenIter();

    while (pConstraintsIter->bHasNext()) {
      PlanNode *pConstraint = pConstraintsIter->pNext();

      RASSERT(pConstraint->szType());

      if      (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NOCHILDREN_PATH)))     { bGotNoChildren = true;     }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_ALWAYSCHILDREN_PATH))) { bGotAlwaysChildren = true; }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NULLABLE_PATH)))       { bGotNullable = true;       }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NONNULLABLE_PATH)))    { bGotNonNullable = true;    }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_OPTIONAL_PATH)))       { bGotOptional = true;       }
      else if (pConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_MANDATORY_PATH)))      { bGotMandatory = true;      }
    }

    delete pConstraintsIter;

    // Now we need to look for these constraints after dereferencing the definition string of the property
    DefinitionNode* pPropDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pProp->szType(), pRootNode));
    RASSERT(pPropDefn);
    RASSERT(pPropDefn->szPath()->bBeginsWith(const_cast<Stz*>(C_DEFINITIONS_PATH)));
    RASSERT(!pPropDefn->szType());

    // Walk up the definition's inheritance hierarchy
    do {
      if (pPropDefn->nProperties() == 0) {
        continue;
      }

      // Get the constraint container for this definition, if any
      PlanNode* pDefnConstraints = GraphNav::pFindProperty(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_NAME), pPropDefn);
      if (!pDefnConstraints) {
        continue;
      }

      // Iterate the constraints in this definition constraint container
      PlanNodeIter *pDefnConstraintsIter = pDefnConstraints->pNewChildrenIter();

      while (pDefnConstraintsIter->bHasNext()) {
        PlanNode *pDefnConstraint = pDefnConstraintsIter->pNext();

        RASSERT(pDefnConstraint->szType());

        if      (pDefnConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NOCHILDREN_PATH)))     { bGotNoChildren = true;     }
        else if (pDefnConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_ALWAYSCHILDREN_PATH))) { bGotAlwaysChildren = true; }
        else if (pDefnConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NULLABLE_PATH)))       { bGotNullable = true;       }
        else if (pDefnConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_NONNULLABLE_PATH)))    { bGotNonNullable = true;    }
        else if (pDefnConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_OPTIONAL_PATH)))       { bGotOptional = true;       }
        else if (pDefnConstraint->szType()->bEqual(const_cast<Stz *>(C_CONSTRAINTS_MANDATORY_PATH)))      { bGotMandatory = true;      }
      }
      delete pDefnConstraintsIter;

    } while( pPropDefn &&
             (pPropDefn = static_cast<DefinitionNode*>(GraphNav::pGetParent(pPropDefn, pRootNode))) &&
             !pPropDefn->szPath()->bEqual(const_cast<Stz*>(C_DEFINITIONS_PATH)) );

    // Check for the invalid combinations
    RASSERT(!(bGotNoChildren && bGotAlwaysChildren));
    RASSERT(!(bGotNullable && bGotNonNullable));
    RASSERT(!(bGotOptional && bGotMandatory));
  } // while (pPropIter->bHasNext())

  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.19                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_19(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.19: If an object definition has a property where the property's type field
  //               is non null and that property has a __constraint container, then validate
  //               that any constraints specified are not specified more than once in this
  //               constraints container for the property in question.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode *pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    // Get the constraints container for this property
    PlanNode *pConstraints = GraphNav::pFindProperty(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_NAME), pProp);

    if (!pConstraints) {
      continue;
    }

    // Iterate the constraints in this constraint container
    PlanNodeIter *pConstraintsIter1 = pConstraints->pNewChildrenIter();
    PlanNodeIter *pConstraintsIter2 = pConstraints->pNewChildrenIter();

    while (pConstraintsIter1->bHasNext()) {
      PlanNode *pConstraint1 = pConstraintsIter1->pNext();
      RASSERT(pConstraint1->szType());

      // Now iterate it again to find any constraints of the same type in the same container
      pConstraintsIter2->vBeforeFirst();

      while (pConstraintsIter2->bHasNext()) {
        PlanNode *pConstraint2 = pConstraintsIter2->pNext();
        RASSERT(pConstraint2->szType());

        if (pConstraint1 == pConstraint2) {
          continue; // ignore self-comparison
        }

        // Cannot have the same type of constraint specified twice
        RASSERT(!pConstraint1->szType()->bEqual(pConstraint2->szType()));
      }
    }

    delete pConstraintsIter1;
    delete pConstraintsIter2;
  }

  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-5.20                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_5_20(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-5: Validate the constraints:
  //    (Indirect constraints follow. Refer to Note1 below)
  //    DEFN-5.20: If an object definition has a property where the property's type field
  //               is non null and that property has a __constraint container, then validate
  //               that any constraints specified are not already specified in the definition
  //               of the property (including that definitions inheritance hierarchy).

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  // Iterate each property. We're only interested in those properties that
  // have a non-null type field and a constraints container.
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode *pProp = pPropIter->pNext();

    if (!pProp->szType()) {
      continue;   // ignore
    }

    // Get the constraints container for this property
    PlanNode *pConstraints = GraphNav::pFindProperty(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_NAME), pProp);

    if (!pConstraints) {
      continue;
    }

    // Get the definition of the property
    RASSERT(pProp->szType()->bBeginsWith(const_cast<Stz*>(C_DEFINITIONS_PATH)));
    DefinitionNode* pPropDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pProp->szType(), pRootNode));
    RASSERT(pPropDefn);
    RASSERT_TYPE(pPropDefn, CN_DEFINITIONNODE);

    // Iterate the constraints in this constraint container
    PlanNodeIter *pConstraintsIter = pConstraints->pNewChildrenIter();

    while (pConstraintsIter->bHasNext()) {
      PlanNode *pConstraint = pConstraintsIter->pNext();
      RASSERT(pConstraint->szType());

      // Now iterate the definition of the property all the way up the inheritance
      // hierarchy and look for the same constraint (by type).
      do {
        if (pPropDefn->nProperties() == 0) {
          continue;
        }

        // Get the constraint container in this definition node
        PlanNode* pDefnConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pPropDefn);
        if (!pDefnConstraints) {
          continue;
        }

        // Iterate the constraints in this definition node
        PlanNodeIter* pDefnConstraintsIter = pDefnConstraints->pNewChildrenIter();
        while (pDefnConstraintsIter->bHasNext()) {
          PlanNode* pDefnConstraint = pDefnConstraintsIter->pNext();

          // Compare the constraint types
          RASSERT(pDefnConstraint->szType());
          RASSERT(!pDefnConstraint->szType()->bEqual(pConstraint->szType()));
        }
        delete pDefnConstraintsIter;

      } while (pPropDefn &&
               (pPropDefn = static_cast<DefinitionNode*>(GraphNav::pGetParent(pPropDefn, pRootNode))) &&
               !pPropDefn->szPath()->bEqual(const_cast<Stz*>(C_DEFINITIONS_PATH)) );

    }

    delete pConstraintsIter;
  }

  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                DEFN-6.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vDefn_6_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // DEFN-6: Validate the children:
  // DEFN-6.1: validate that all children of a given definition node must have unique names.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nChildren() <= 1) {
    return;
  }

  // Horrible O(N^2) lookup, but there shouldn't be too many children.
  PlanNodeIter* pChildIter1 = pNode->pNewChildrenIter();

  while (pChildIter1->bHasNext()) {
    PlanNode* pChild1 = pChildIter1->pNext();

    PlanNodeIter* pChildIter2 = pNode->pNewChildrenIter();
    while (pChildIter2->bHasNext()) {
      PlanNode *pChild2 = pChildIter2->pNext();

      if (pChild1 == pChild2) {
        continue;
      }

      // Names of the children cannot be the same
      RASSERT(!pChild1->szName()->bEqual(pChild2->szName()))
    }
    delete pChildIter2;
  }

  delete pChildIter1;
}


/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                PLAN-1.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_1_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-1: Validate the name field:
  //    PLAN-1.1: the name field must not be null.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(pNode->szName());
  RASSERT(pNode->szName()->nLength() > 0);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-2.1                                    */
/*----------------------------------------------------------------------------*/
//void GraphValidator::vPlan_2_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
//{
//  // PLAN-2: Validate the value field:
//  //    No validations here as it is largely dependent on constraints
//}

/*----------------------------------------------------------------------------*/
/*                                PLAN-3.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_3_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-3: Validate the type field:
  //    PLAN-3.1: the type field must not be null.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(pNode->szType());
  RASSERT(pNode->szType()->nLength() > 0);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-3.2                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_3_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-3: Validate the type field:
  //    PLAN-3.2: the type field must be an absolute path (start with a '/').

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(pNode->szType()->bBeginsWith(StrLib::szCharToStr(C_CHILD_SEPARATOR)));
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-3.3                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_3_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-3: Validate the type field:
  //    PLAN-3.3: the path in the type field must be resolvable to another node in the system.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
  PlanNode* pDefnNode = GraphNav::pFind(pNode->szType(), pRootNode);

  RASSERT(pDefnNode);
  RASSERT_TYPE(pDefnNode, CN_DEFINITIONNODE);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-3.4                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_3_4(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-3: Validate the type field:
  //    PLAN-3.4: the path in the type field must be resolve to a node in /__root__/definitions.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
  DefinitionNode* pDefnNode = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));

  RASSERT(pDefnNode);
  RASSERT_TYPE(pDefnNode, CN_DEFINITIONNODE);
  RASSERT(pDefnNode->szPath()->bBeginsWith(const_cast<Stz*>(C_DEFINITIONS_PATH)));
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-3.5                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_3_5(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-3: Validate the type field:
  //    PLAN-3.5: the path in the type field must not be a property path
  //              (i.e. it must not have a ':' in the path).

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  RASSERT(!pNode->szType()->bContains(StrLib::szCharToStr(C_PROPERTY_SEPARATOR)));
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-4.1                                    */
/*----------------------------------------------------------------------------*/
//void GraphValidator::vPlan_4_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
//{
//  // PLAN-4: Validate the properties:
//  //    No validations here as it is largely dependent on constraints
//}

/*----------------------------------------------------------------------------*/
/*                                PLAN-5.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_5_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-5: Validate the constraints:
  //    PLAN-5.1: Validate that there is no __constraint container in this objects properties.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  PlanNode* pConstraints = GraphNav::pFindProperty(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_NAME), pNode);
  RASSERT(!pConstraints);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-5.2                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_5_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-5: Validate the constraints:
  //    PLAN-5.2: Validate that there are no properties that derive from a constraint definition.

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();
  while (pPropIter->bHasNext()) {
    PlanNode* pProp = pPropIter->pNext();

    RASSERT(pProp->szType());
    RASSERT(!pProp->szType()->bBeginsWith(const_cast<Stz*>(C_CONSTRAINTS_CONTAINER_PATH)));
  }
  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-6.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_6_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-6: Validate the children:
  //    PLAN-6.1: the children must each have a unique name (so they have a unique path).

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nChildren() <= 1) {
    return;
  }

  // Horrible O(N^2) approach here, but hopefully we shouldn't have too many children.
  PlanNodeIter* pChildIter1 = pNode->pNewChildrenIter();
  while (pChildIter1->bHasNext()) {
    PlanNode* pChild1 = pChildIter1->pNext();

    PlanNodeIter* pChildIter2 = pNode->pNewChildrenIter();
    while (pChildIter2->bHasNext()) {
      PlanNode *pChild2 = pChildIter2->pNext();

      if (pChild1 == pChild2) {
        continue; // same object
      }

      // Two children cannot have the same name
      RASSERT(!pChild1->szName()->bEqual(pChild2->szName()));
    }
    delete pChildIter2;

  }
  delete pChildIter1;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-6.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_6_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-6: Validate the children:
  //    PLAN-6.2: the leaf nodes on a non-property path must be data sources

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

  if (pNode->nChildren() > 0 || sPath.bEqual(const_cast<Stz*>(C_PLAN_NAME)) ||
      sPath.bContains(StrLib::szCharToStr(C_PROPERTY_SEPARATOR))) {
    return;
  }

  RASSERT(pNode->szType()->bBeginsWith(const_cast<Stz*>(C_DATASOURCE_DEFN)));
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-7.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_7_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-7: Validate the object against its definition properties (excluding constraints):
  //    PLAN-7.1: the node must have properties with names and types that represent all the properties
  //              defined in its definition (including all parents of the definition), excluding any
  //              properties that are marked as optional.

  UNUSED(sPath);
  UNUSED(iDepth);

  RASSERT(pNode->szType());

  // Get the root node
  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition node
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  while (!pDefn->szPath()->bEqual(const_cast<Stz*>(C_DEFINITIONS_PATH))) {

    if (pDefn->nProperties() > 0) {

      // Is there a constraints container in this definition?
      PlanNode *pDefnConstraints = GraphNav::pFindProperty(const_cast<Stz *>(C_CONSTRAINTS_CONTAINER_NAME), pDefn);

      // Iterate each property of this definition
      PlanNodeIter *pPropIter = pDefn->pNewPropertyIter();

      while (pPropIter->bHasNext()) {
        DefinitionNode *pDefnProp = static_cast<DefinitionNode *>(pPropIter->pNext());

        if (pDefnProp == pDefnConstraints) {
          continue; // ignore
        }

        // Check if this property is optional. If it is then we don't care if the
        // current node has this property.
        if (bHasOptionalConstraint(pDefnProp)) {
          continue;
        }

        // Non-optional property. The current node must have it.
        RASSERT(pDefnProp->szName());
        RASSERT(pDefnProp->szType());

        // The name of the pNode property must match the name of the definition property.
        // If the definition has a null type then the type of the pNode must be derived
        // from the path of the definition property (i.e. its type must be a superset of the
        // path of the definition property). Otherwise if the defintion property has a
        // non-null type then the type of the pNode must be derived from this definition type.
        PlanNode *pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
        RASSERT(pProp);
        RASSERT(pProp->szName()->bEqual(pDefnProp->szName()));

        if (pDefnProp->szType()) {
          RASSERT(pProp->szType()->bBeginsWith(pDefnProp->szType()));

        } else {
          RASSERT(pProp->szType()->bBeginsWith(pDefnProp->szPath()));
        }
      }

      delete pPropIter;
    }

    // Get the parent of this node.
    pDefn = static_cast<DefinitionNode*>(GraphNav::pGetParent(pDefn, pRootNode));
  }

}

/*----------------------------------------------------------------------------*/
/*                                PLAN-7.2                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_7_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-7: Validate the object against its definition properties (excluding constraints):
  //    PLAN-7.2: the current node must not have any additional properties that
  //              aren't specified in its definition (including the definition's parents recursively).

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nProperties() == 0) {
    return;
  }

  RASSERT(pNode->szType());

  // Get the root node
  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Iterate the properties of pNode
  PlanNodeIter* pPropIter = pNode->pNewPropertyIter();

  while (pPropIter->bHasNext()) {
    PlanNode* pProp = pPropIter->pNext();
    RASSERT(pProp->szName());

    // Get the definition node of pNode
    DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
    RASSERT(pDefn);
    RASSERT(!pDefn->szType());
    RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

    // Now walk the definition hierarchy of pNode and match this property's name
    // to a definition. It will be an error if we cannot find it.
    b1 bFoundDefn = false;

    while (!bFoundDefn && pDefn && !pDefn->szPath()->bEqual(const_cast<Stz*>(C_DEFINITIONS_PATH))) {

      if (pDefn->nProperties() > 0) {
        PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

        while (pDefnPropIter->bHasNext()) {
          PlanNode* pDefnProp = pDefnPropIter->pNext();

          if (pProp->szName()->bEqual(pDefnProp->szName())) {
            bFoundDefn = true;
            break;
          }
        }
        delete pDefnPropIter;
      }

      // get the parent definition
      pDefn = static_cast<DefinitionNode*>(GraphNav::pGetParent(pDefn, pRootNode));
    }

    // We should have found the definition of the property in question.
    RASSERT(bFoundDefn);
  }

  delete pPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.1                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_1(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) {
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.1: If a node's definition contains the "noChildren" constraint
  //              then validate that the node has no children.

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nChildren() == 0) {
    return;
  }

  // Get the root node
  PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

  // Get the definition node of pNode
  DefinitionNode *pDefn = static_cast<DefinitionNode *>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);
  RASSERT(!pDefn->szType());
  RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

  // Search for this constraint in the inheritance hierarchy
  PlanNode *pConstraint = pGetConstraint(const_cast<Stz *>(C_CONSTRAINTS_NOCHILDREN_PATH),
                                         pDefn,
                                         pRootNode,
                                         true);

  if (!pConstraint) {
    // We don't have this constraint in the inheritance hierarchy
    return;
  }

  // We have the noChildren constraint, so we shouldn't have any children
  RASSERT(pNode->nChildren() == 0);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.2                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_2(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.2: If a node's definition contains the "alwaysChildren" constraint
  //              then validate that the node must have at least one child.

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nChildren() > 0) {
    return;
  }

  // Get the root node
  PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

  // Get the definition node of pNode
  DefinitionNode *pDefn = static_cast<DefinitionNode *>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);
  RASSERT(!pDefn->szType());
  RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

  // Search for this constraint in the inheritance hierarchy
  PlanNode *pConstraint = pGetConstraint(const_cast<Stz *>(C_CONSTRAINTS_ALWAYSCHILDREN_PATH),
                                         pDefn,
                                         pRootNode,
                                         true);

  if (!pConstraint) {
    // We don't have this constraint in the inheritance hierarchy
    return;
  }

  // We have the alwaysChildren constraint, so we must have children
  RASSERT(pNode->nChildren() > 0);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.3                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_3(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.3: If a node's definition contains the "nullable" constraint
  //              then validate that the value field of the node can be null.

  // NB: the value field of all nodes can be made null. This is a default.
  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.4                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_4(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.4: If a node's definition contains the "nonNullable" constraint
  //              then validate that the value field of the node cannot be null.

  UNUSED(sPath);
  UNUSED(iDepth);

  // Get the root node
  PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

  // Get the definition node of pNode
  DefinitionNode *pDefn = static_cast<DefinitionNode *>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);
  RASSERT(!pDefn->szType());
  RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

  // Search for this constraint in the inheritance hierarchy
  PlanNode *pConstraint = pGetConstraint(const_cast<Stz *>(C_CONSTRAINTS_NONNULLABLE_PATH),
                                         pDefn,
                                         pRootNode,
                                         true);

  if (!pConstraint) {
    // We don't have this constraint in the inheritance hierarchy
    return;
  }

  // We have the nonNullable constraint, so the value must not be null
  RASSERT(!pNode->bIsNullValue());
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.5                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_5(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.5: NB: validation of the "optional" constraint is achieved in PLAN-7.1.

  // Noop
  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.6                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_6(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.6: NB: validation of the "mandatory" constraint is achieved in PLAN-7.1.

  // Noop
  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.7                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_7(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.7: If a node's definition contains the "childType" constraint
  //              then validate that any and all children of the node must be of a type indicated
  //              by the constraint's parameter.

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nChildren() == 0) {
    return;
  }

  // Get the root node
  PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

  // Get the definition node of pNode
  DefinitionNode *pDefn = static_cast<DefinitionNode *>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);
  RASSERT(!pDefn->szType());
  RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

  // Search for this constraint in the inheritance hierarchy
  PlanNode *pConstraint = pGetConstraint(const_cast<Stz *>(C_CONSTRAINTS_CHILDTYPE_PATH),
                                         pDefn,
                                         pRootNode,
                                         true);

  if (!pConstraint) {
    // We don't have this constraint in the inheritance hierarchy
    return;
  }

  // We have the childType constraint. It must have at least one child
  // where each child's value field contains the definition that pNode's
  // children must adhere to. Iterate each child of pNode and compare their
  // definition paths to those of the children of the constraint.
  RASSERT(pConstraint->nChildren() > 0);

  PlanNodeIter* pChildIter = pNode->pNewChildrenIter();
  PlanNodeIter* pConstraintParamIter = pConstraint->pNewChildrenIter();

  while (pChildIter->bHasNext()) {
    PlanNode* pChild = pChildIter->pNext();
    pConstraintParamIter->vBeforeFirst();
    b1 bMatchedType = false;

    while (pConstraintParamIter->bHasNext()) {
      PlanNode* pConstraintParam = pConstraintParamIter->pNext();

      RASSERT(pConstraintParam->m_xValue.m_pText);
      RASSERT(pConstraintParam->m_xValue.m_bMustFree);
      RASSERT(pConstraintParam->szValue()->nLength() > 0);

      if (pChild->szType()->bBeginsWith(pConstraintParam->szValue())) {
        bMatchedType = true;
        break;
      }
    }

    RASSERT(bMatchedType);
  }

  delete pConstraintParamIter;
  delete pChildIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.8                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_8(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.8: If a node's definition contains the "valueType" constraint
  //              then validate that the value field of the node must be of a type indicated
  //              by the constraint's parameter.

  UNUSED(sPath);
  UNUSED(iDepth);

  // Get the root node
  PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

  // Get the definition node of pNode
  DefinitionNode *pDefn = static_cast<DefinitionNode *>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);
  RASSERT(!pDefn->szType());
  RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

  // Search for this constraint in the inheritance hierarchy
  PlanNode *pConstraint = pGetConstraint(const_cast<Stz *>(C_CONSTRAINTS_VALUETYPE_PATH),
                                         pDefn,
                                         pRootNode,
                                         true);

  if (!pConstraint) {
    // We don't have this constraint in the inheritance hierarchy
    return;
  }

  // Get the parameter of this constraint. There could be parameters options provided.
  RASSERT(pConstraint->nChildren() > 0);

  b1 bMatch = false;
  PlanNodeIter* pIter = pConstraint->pNewChildrenIter();

  while (pIter->bHasNext()) {
    PlanNode* pConstraintParam = pIter->pNext();
    RASSERT(pConstraintParam->szType());
    RASSERT(pConstraintParam->szType()->bBeginsWith(const_cast<Stz*>(C_STRING_OBJ_DEFN)));

    RASSERT(!pConstraintParam->bIsNullValue());
    Stz* szConstraintVal = pConstraintParam->szValue();
    RASSERT(szConstraintVal->bBeginsWith(const_cast<Stz*>(C_SCALAR_PRIMITIVE)));

    if (
        (szConstraintVal->bEqual(const_cast<Stz*>(C_STRING_PRIMITIVE))    && pNode->eValueType() == Value::VT_STRING)     ||
        (szConstraintVal->bEqual(const_cast<Stz*>(C_INTEGER_PRIMITIVE))   && pNode->eValueType() == Value::VT_INT)        ||
        (szConstraintVal->bEqual(const_cast<Stz*>(C_FLOAT_PRIMITIVE))     && pNode->eValueType() == Value::VT_FLOAT)      ||
        (szConstraintVal->bEqual(const_cast<Stz*>(C_BOOLEAN_PRIMITIVE))   && pNode->eValueType() == Value::VT_BOOLEAN)    ||
        (szConstraintVal->bEqual(const_cast<Stz*>(C_DATE_PRIMITIVE))      && pNode->eValueType() == Value::VT_DATE)       ||
        (szConstraintVal->bEqual(const_cast<Stz*>(C_TIME_PRIMITIVE))      && pNode->eValueType() == Value::VT_TIME)       ||
        (szConstraintVal->bEqual(const_cast<Stz*>(C_TIMESTAMP_PRIMITIVE)) && pNode->eValueType() == Value::VT_TIMESTAMP)  ||
        (szConstraintVal->bEqual(const_cast<Stz*>(C_INTERVAL_PRIMITIVE))  && pNode->eValueType() == Value::VT_INTERVAL)   ||
        (szConstraintVal->bEqual(const_cast<Stz*>(C_UNKNOWN_PRIMITIVE))   && pNode->eValueType() == Value::VT_UNKNOWN))   {

      bMatch = true;
      continue;
    }
  }

  delete pIter;

  // We should have gotten a match
  RASSERT(bMatch);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.9                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_9(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.9: If a node's definition contains the "childCount" constraint
  //              then validate that the number of children of the node must equal that indicated
  //              by any of the constraint's parameters.

  UNUSED(sPath);
  UNUSED(iDepth);

  // Get the root node
  PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

  // Get the definition node of pNode
  DefinitionNode *pDefn = static_cast<DefinitionNode *>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);
  RASSERT(!pDefn->szType());
  RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

  // Search for this constraint in the inheritance hierarchy
  PlanNode *pConstraint = pGetConstraint(const_cast<Stz *>(C_CONSTRAINTS_CHILDCOUNT_PATH),
                                         pDefn,
                                         pRootNode,
                                         true);

  if (!pConstraint) {
    // We don't have this constraint in the inheritance hierarchy
    return;
  }

  // We have the childCount constraint. It must have only one child
  // where the child's value field contains an integer with the number
  // of children that pNode must adhere to.
  RASSERT(pConstraint->nChildren() > 0);

  b1 bGotMatch = false;
  PlanNodeIter* pParamIter = pConstraint->pNewChildrenIter();
  while (pParamIter->bHasNext()) {
    PlanNode* pConstraintParam = pParamIter->pNext();
    RASSERT(pConstraintParam);
    RASSERT(pConstraintParam->iValue() >= 0);

    if (pNode->nChildren() == pConstraintParam->iValue()) {
      bGotMatch = true;
      break;
    }
  }
  delete pParamIter;

  RASSERT(bGotMatch);
}


/*----------------------------------------------------------------------------*/
/*                                PLAN-8.10                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_10a(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  //    PLAN-8.10a: If a node's definition contains the "minChildCount" constraint
  //               then validate that the number of children of the node must not be less than
  //               that indicated by any of the constraint's parameters.
  UNUSED(sPath);
  UNUSED(iDepth);

  // Get the root node
  PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

  // Get the definition node of pNode
  DefinitionNode *pDefn = static_cast<DefinitionNode *>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);
  RASSERT(!pDefn->szType());
  RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

  // Search for this constraint in the inheritance hierarchy
  PlanNode *pConstraint = pGetConstraint(const_cast<Stz *>(C_CONSTRAINTS_MINCHILDCOUNT_PATH),
                                         pDefn,
                                         pRootNode,
                                         true);

  if (!pConstraint) {
    // We don't have this constraint in the inheritance hierarchy
    return;
  }

  // We have the minChildCount constraint. It must have only one child
  // where the child's value field contains an integer with the number
  // of children that pNode must adhere to.
  RASSERT(pConstraint->nChildren() == 1);

  PlanNode* pConstraintParam = pConstraint->pFirstChild();
  RASSERT(pConstraintParam);
  RASSERT(pConstraintParam->iValue() >= 0);
  RASSERT(pNode->nChildren() >= pConstraintParam->iValue());
}

//    PLAN-8.10b: If a node's definition contains the "maxChildCount" constraint
//               then validate that the number of children of the node must not be greater than
//               that indicated by any of the constraint's parameters.
/*----------------------------------------------------------------------------*/
/*                                PLAN-8.10                                    */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_10b(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  //    PLAN-8.10b: If a node's definition contains the "maxChildCount" constraint
  //               then validate that the number of children of the node must not be greater than
  //               that indicated by any of the constraint's parameters.
  UNUSED(sPath);
  UNUSED(iDepth);

  // Get the root node
  PlanNode *pRootNode = static_cast<PlanNode *>(pvArg);

  // Get the definition node of pNode
  DefinitionNode *pDefn = static_cast<DefinitionNode *>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);
  RASSERT(!pDefn->szType());
  RASSERT(pDefn->szPath() && pDefn->szPath()->nLength() > 0);

  // Search for this constraint in the inheritance hierarchy
  PlanNode *pConstraint = pGetConstraint(const_cast<Stz *>(C_CONSTRAINTS_MAXCHILDCOUNT_PATH),
                                         pDefn,
                                         pRootNode,
                                         true);

  if (!pConstraint) {
    // We don't have this constraint in the inheritance hierarchy
    return;
  }

  // We have the minChildCount constraint. It must have only one child
  // where the child's value field contains an integer with the number
  // of children that pNode must adhere to.
  RASSERT(pConstraint->nChildren() == 1);

  PlanNode* pConstraintParam = pConstraint->pFirstChild();
  RASSERT(pConstraintParam);
  RASSERT(pConstraintParam->iValue() >= 0);
  RASSERT(pNode->nChildren() <= pConstraintParam->iValue());
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.11                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_11(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.11: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "noChildren" constraint then validate
  //               that the property of the current node has no children.

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nProperties() == 0) {
    return;
  }

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition of this node
  RASSERT(pNode->szType());
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  if (pDefn->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

  while (pDefnPropIter->bHasNext()) {
    DefinitionNode* pDefnProp = static_cast<DefinitionNode*>(pDefnPropIter->pNext());

    // We're only interested in definition properties that have a non-null type field,
    // because they could also have additional constraints applied to them
    // in addition to any constraints in its (dereferenced) definition of that property.
    if (!pDefnProp->szType()) {
      continue;
    }

    // Does this property definition have a "noChildren" constraint"?
    PlanNode* pConstraint = pGetConstraint(const_cast<Stz*>(C_CONSTRAINTS_NOCHILDREN_PATH), pDefnProp);

    if (!pConstraint) {
      continue;
    }

    // Now we can check if the instantiation of the property meets this constraint.
    // First get the instantiated property.
    PlanNode* pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
    RASSERT(pProp);

    // Validate the constraint
    RASSERT(pProp->nChildren() == 0);
  }

  delete pDefnPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.12                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_12(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.12: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "alwaysChildren" constraint then validate
  //               that the property of the current node must have at least one child.

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nProperties() == 0) {
    return;
  }

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition of this node
  RASSERT(pNode->szType());
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  if (pDefn->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

  while (pDefnPropIter->bHasNext()) {
    DefinitionNode* pDefnProp = static_cast<DefinitionNode*>(pDefnPropIter->pNext());

    // We're only interested in definition properties that have a non-null type field,
    // because they could also have additional constraints applied to them
    // in addition to any constraints in its (dereferenced) definition of that property.
    if (!pDefnProp->szType()) {
      continue;
    }

    // Does this property definition have a "alwaysChildren" constraint"?
    PlanNode* pConstraint = pGetConstraint(const_cast<Stz*>(C_CONSTRAINTS_ALWAYSCHILDREN_PATH), pDefnProp);

    if (!pConstraint) {
      continue;
    }

    // Now we can check if the instantiation of the property meets this constraint.
    // First get the instantiated property.
    PlanNode* pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
    RASSERT(pProp);

    // Validate the constraint
    RASSERT(pProp->nChildren() > 0);
  }

  delete pDefnPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.13                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_13(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.13: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "nullable" constraint then validate
  //               that the value field of the property of the current node can be null.

  // Noop, all values are nullable by default.
  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.14                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_14(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.14: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "nonNullable" constraint then validate
  //               that the value field of the property of the current node cannot be null.

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nProperties() == 0) {
    return;
  }

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition of this node
  RASSERT(pNode->szType());
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  if (pDefn->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

  while (pDefnPropIter->bHasNext()) {
    DefinitionNode* pDefnProp = static_cast<DefinitionNode*>(pDefnPropIter->pNext());

    // We're only interested in definition properties that have a non-null type field,
    // because they could also have additional constraints applied to them
    // in addition to any constraints in its (dereferenced) definition of that property.
    if (!pDefnProp->szType()) {
      continue;
    }

    // Does this property definition have a "nonNullable" constraint"?
    PlanNode* pConstraint = pGetConstraint(const_cast<Stz*>(C_CONSTRAINTS_NONNULLABLE_PATH), pDefnProp);

    if (!pConstraint) {
      continue;
    }

    // Now we can check if the instantiation of the property meets this constraint.
    // First get the instantiated property.
    PlanNode* pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
    RASSERT(pProp);

    // Validate the constraint
    RASSERT(!pProp->bIsNullValue());
  }

  delete pDefnPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.15                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_15(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.15: NB: validation of the "optional" constraint is achieved in PLAN-7.1.

  // Noop
  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.16                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_16(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.16: NB: validation of the "mandatory" constraint is achieved in PLAN-7.1.

  // Noop. All properties are mandatory by default.

  UNUSED(pNode);
  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.17                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_17(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.17: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "childType" constraint then validate
  //               that any and all children of the property of the current node must be of
  //               a type indicated by the constraint's parameter.

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nProperties() == 0) {
    return;
  }

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition of this node
  RASSERT(pNode->szType());
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  if (pDefn->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

  while (pDefnPropIter->bHasNext()) {
    DefinitionNode* pDefnProp = static_cast<DefinitionNode*>(pDefnPropIter->pNext());

    // We're only interested in definition properties that have a non-null type field,
    // because they could also have additional constraints applied to them
    // in addition to any constraints in its (dereferenced) definition of that property.
    if (!pDefnProp->szType()) {
      continue;
    }

    // Does this property definition have a "childType" constraint"?
    PlanNode* pConstraint = pGetConstraint(const_cast<Stz*>(C_CONSTRAINTS_CHILDTYPE_PATH), pDefnProp);

    if (!pConstraint) {
      continue;
    }

    // Get the parameter of this constraint and extract out its value
    PlanNode* pConstraintParam = pConstraint->pFirstChild();
    RASSERT(pConstraintParam);
    RASSERT(pConstraintParam->m_xValue.m_pText);
    RASSERT(pConstraintParam->m_xValue.m_bMustFree);
    RASSERT(pConstraintParam->szValue());
    RASSERT(pConstraintParam->szValue()->nLength() > 0);

    Stz* szChildTypeDefn = pConstraintParam->szValue();

    // Now we can check if the instantiation of the property meets this constraint.
    // First get the instantiated property.
    PlanNode* pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
    RASSERT(pProp);

    // Validate the constraint
    if (pProp->nChildren() == 0) {
      continue;
    }

    PlanNodeIter* pPropChildIter = pProp->pNewChildrenIter();
    while (pPropChildIter->bHasNext()) {
      PlanNode* pChild = pPropChildIter->pNext();

      // The definition of this child must match the constraint definition
      RASSERT(pChild->szType());
      RASSERT(pChild->szType()->bBeginsWith(szChildTypeDefn));
    }
    delete pPropChildIter;
  }

  delete pDefnPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.18                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_18(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.18: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "valueType" constraint then validate
  //               that the value field of the property of the current node must be of a type
  //               indicated by the constraint's parameter.

  UNUSED(sPath);
  UNUSED(iDepth);

  if (pNode->nProperties() == 0) {
    return;
  }

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition of this node
  RASSERT(pNode->szType());
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  if (pDefn->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

  while (pDefnPropIter->bHasNext()) {
    DefinitionNode* pDefnProp = static_cast<DefinitionNode*>(pDefnPropIter->pNext());

    // We're only interested in definition properties that have a non-null type field,
    // because they could also have additional constraints applied to them
    // in addition to any constraints in its (dereferenced) definition of that property.
    if (!pDefnProp->szType()) {
      continue;
    }

    // Does this property definition have a "valueType" constraint"?
    PlanNode* pConstraint = pGetConstraint(const_cast<Stz*>(C_CONSTRAINTS_VALUETYPE_PATH), pDefnProp);

    if (!pConstraint) {
      continue;
    }

    // Now we can check if the instantiation of the property meets this constraint.
    // First get the instantiated property.
    PlanNode* pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
    RASSERT(pProp);
    b1 bMatch = false;

    // Iterate the parameters of this constraint and extract out their value
    RASSERT(pConstraint->nChildren() > 0);

    PlanNodeIter* pConstraintParamIter = pConstraint->pNewChildrenIter();
    while (pConstraintParamIter->bHasNext()) {
      PlanNode* pConstraintParam = pConstraintParamIter->pNext();

      RASSERT(pConstraintParam);
      RASSERT(pConstraintParam->eValueType() == Value::VT_STRING);
      RASSERT(!pConstraintParam->bIsNullValue());
      RASSERT(pConstraintParam->m_xValue.m_pText);
      RASSERT(pConstraintParam->m_xValue.m_bMustFree);
      RASSERT(pConstraintParam->szValue());
      RASSERT(pConstraintParam->szValue()->nLength() > 0);

      Stz* szValueTypeDefn = pConstraintParam->szValue();

      // Compare to the property in question
      if (
          (szValueTypeDefn->bEqual(const_cast<Stz*>(C_STRING_PRIMITIVE))    && pProp->eValueType() == Value::VT_STRING)     ||
          (szValueTypeDefn->bEqual(const_cast<Stz*>(C_INTEGER_PRIMITIVE))   && pProp->eValueType() == Value::VT_INT)        ||
          (szValueTypeDefn->bEqual(const_cast<Stz*>(C_FLOAT_PRIMITIVE))     && pProp->eValueType() == Value::VT_FLOAT)      ||
          (szValueTypeDefn->bEqual(const_cast<Stz*>(C_BOOLEAN_PRIMITIVE))   && pProp->eValueType() == Value::VT_BOOLEAN)    ||
          (szValueTypeDefn->bEqual(const_cast<Stz*>(C_DATE_PRIMITIVE))      && pProp->eValueType() == Value::VT_DATE)       ||
          (szValueTypeDefn->bEqual(const_cast<Stz*>(C_TIME_PRIMITIVE))      && pProp->eValueType() == Value::VT_TIME)       ||
          (szValueTypeDefn->bEqual(const_cast<Stz*>(C_TIMESTAMP_PRIMITIVE)) && pProp->eValueType() == Value::VT_TIMESTAMP)  ||
          (szValueTypeDefn->bEqual(const_cast<Stz*>(C_INTERVAL_PRIMITIVE))  && pProp->eValueType() == Value::VT_INTERVAL))   {

        bMatch = true;
        continue;
      }
    }
    delete pConstraintParamIter;

    RASSERT(bMatch);
  }

  delete pDefnPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.19                                   */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_19(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.19: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "childCount" constraint then validate
  //               that the number of children of the property of the current node must equal
  //               that indicated by the constraint's parameter.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition of this node
  RASSERT(pNode->szType());
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  if (pDefn->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

  while (pDefnPropIter->bHasNext()) {
    DefinitionNode* pDefnProp = static_cast<DefinitionNode*>(pDefnPropIter->pNext());

    // We're only interested in definition properties that have a non-null type field,
    // because they could also have additional constraints applied to them
    // in addition to any constraints in its (dereferenced) definition of that property.
    if (!pDefnProp->szType()) {
      continue;
    }

    // Does this property definition have a "childCount" constraint"?
    PlanNode* pConstraint = pGetConstraint(const_cast<Stz*>(C_CONSTRAINTS_CHILDCOUNT_PATH), pDefnProp);

    if (!pConstraint) {
      continue;
    }

    // First get the instantiated property.
    PlanNode* pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
    RASSERT(pProp);

    // Get the parameter of this constraint and extract out its value
    b1 bGotMatch = false;
    PlanNodeIter* pParamIter = pConstraint->pNewChildrenIter();
    while (pParamIter->bHasNext()) {
      PlanNode* pConstraintParam = pParamIter->pNext();
      RASSERT(pConstraintParam);
      RASSERT(pConstraintParam->iValue() >= 0);

      if (pProp->nChildren() == pConstraintParam->iValue()) {
        bGotMatch = true;
        break;
      }
    }
    delete pParamIter;

    RASSERT(bGotMatch);
  }

  delete pDefnPropIter;
}



/*----------------------------------------------------------------------------*/
/*                                PLAN-8.20a                                  */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_20a(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.20a: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "minChildCount" constraint then validate
  //               that the number of children of the property of the current node must not
  //               be less than that indicated by the constraint's parameter.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition of this node
  RASSERT(pNode->szType());
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  if (pDefn->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

  while (pDefnPropIter->bHasNext()) {
    DefinitionNode* pDefnProp = static_cast<DefinitionNode*>(pDefnPropIter->pNext());

    // We're only interested in definition properties that have a non-null type field,
    // because they could also have additional constraints applied to them
    // in addition to any constraints in its (dereferenced) definition of that property.
    if (!pDefnProp->szType()) {
      continue;
    }

    // Does this property definition have a "minChildCount" constraint"?
    PlanNode* pConstraint = pGetConstraint(const_cast<Stz*>(C_CONSTRAINTS_MINCHILDCOUNT_PATH), pDefnProp);

    if (!pConstraint) {
      continue;
    }

    // First get the instantiated property.
    PlanNode* pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
    RASSERT(pProp);

    // Get the parameter of this constraint and extract out its value
    PlanNode* pConstraintParam = pConstraint->pFirstChild();
    RASSERT(pConstraintParam);
    RASSERT(pConstraintParam->iValue() >= 0);

    RASSERT(pProp->nChildren() >= pConstraintParam->iValue());
  }

  delete pDefnPropIter;
}

/*----------------------------------------------------------------------------*/
/*                                PLAN-8.20b                                  */
/*----------------------------------------------------------------------------*/
void GraphValidator::vPlan_8_20b(PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  // PLAN-8: Validate the object against its definition constraints:
  //    PLAN-8.20b: If a node's definition has a property with a non-null type field, and that
  //               property definition also has the "maxChildCount" constraint then validate
  //               that the number of children of the property of the current node must not
  //               be greater than that indicated by the constraint's parameter.

  UNUSED(sPath);
  UNUSED(iDepth);

  PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

  // Get the definition of this node
  RASSERT(pNode->szType());
  DefinitionNode* pDefn = static_cast<DefinitionNode*>(GraphNav::pFind(pNode->szType(), pRootNode));
  RASSERT(pDefn);

  if (pDefn->nProperties() == 0) {
    return;
  }

  PlanNodeIter* pDefnPropIter = pDefn->pNewPropertyIter();

  while (pDefnPropIter->bHasNext()) {
    DefinitionNode* pDefnProp = static_cast<DefinitionNode*>(pDefnPropIter->pNext());

    // We're only interested in definition properties that have a non-null type field,
    // because they could also have additional constraints applied to them
    // in addition to any constraints in its (dereferenced) definition of that property.
    if (!pDefnProp->szType()) {
      continue;
    }

    // Does this property definition have a "maxChildCount" constraint"?
    PlanNode* pConstraint = pGetConstraint(const_cast<Stz*>(C_CONSTRAINTS_MAXCHILDCOUNT_PATH), pDefnProp);

    if (!pConstraint) {
      continue;
    }

    // First get the instantiated property.
    PlanNode* pProp = GraphNav::pFindProperty(pDefnProp->szName(), pNode);
    RASSERT(pProp);

    // Get the parameter of this constraint and extract out its value
    PlanNode* pConstraintParam = pConstraint->pFirstChild();
    RASSERT(pConstraintParam);
    RASSERT(pConstraintParam->iValue() >= 0);

    RASSERT(pProp->nChildren() <= pConstraintParam->iValue());
  }

  delete pDefnPropIter;
}

/*============================================================================*/
/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
