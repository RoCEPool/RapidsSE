/*============================================================================*/
/* GraphEditor.cpp - a utility for walking plan graphs                         */
/*============================================================================*/

#include "BASE.h"
#include "GraphEditor.hpp"
#include "PlanNode.hpp"
#include "PrimitiveNode.hpp"
#include "PlanConstants.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Creates a new copy of the plan graph from a template. All the definition   */
/* and primitive nodes are reused since they won't/can't be changed.          */
/*----------------------------------------------------------------------------*/
PlanNode* GraphEditor::pNewFromTemplate(PlanNode* pRootNode)
{
  ASSERT(pRootNode);
  ASSERT(pRootNode->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME)));

  // Create a new copy of: /__root__/plan
  QueryNode* pPlanNode = (QueryNode*) GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRootNode);
  ASSERT(pPlanNode);
  ASSERT(pPlanNode->eGetClassNo() == CN_QUERYNODE);

  PlanNode* pNewPlanChild = new QueryNode(*pPlanNode);

  // Create a new list of children for the new root node. Copy all children except
  // replace the /plan child with the new one we've created.
  PlanNodeList* pNewChildList = new PlanNodeList();

  i8 nChildren = pRootNode->nChildren();

  for (i8 i=0; i<nChildren; i++) {
    PlanNode* pItem = pRootNode->pNthChild(i);

    if (pItem == pPlanNode) {
      // Copy in the new /plan child, not the old one
      pNewChildList->vAppend(pNewPlanChild);

    } else {
      pNewChildList->vAppend(pItem);
    }
  }


  // Create a new list of properties for the new root node.
  PlanNodeList* pNewPropertyList = nullptr;

  if (pRootNode->bHasProperties()) {

    pNewPropertyList = new PlanNodeList();

    i8 nProperties = pRootNode->nProperties();
    for (i8 i=0; i<nProperties; i++) {
      pNewPropertyList->vAppend(pRootNode->pNthProperty(i));
    }
  }

  // Now we can create a new copy of: /__root__
  PlanNode* pNewRoot = new PrimitiveNode(pRootNode->szName(),
                                         pRootNode->szType(),
                                         pRootNode->getValue(),
                                         pNewPropertyList,
                                         pNewChildList,
                                         const_cast<Stz*>(C_ROOT_PATH));

  return pNewRoot;
}

/*----------------------------------------------------------------------------*/
/* Validate the entire plan, including definitions                            */
/*----------------------------------------------------------------------------*/
b1 GraphEditor::bValidateAll(PlanNode *pRootNode)
{
  // Validate root
  bValidateRootNode(pRootNode);

  // Validate primitives
  PlanNode* pPrimitives = GraphNav::pFind(const_cast<Stz*>(C_PRIMITIVES_PATH), pRootNode);
  ASSERT(pPrimitives);
  GraphNav::bWalk(pPrimitives, GraphEditor::eValidatePrimitiveNode, pRootNode);

  // Validate definitions
  PlanNode* pDefinitions = GraphNav::pFind(const_cast<Stz*>(C_DEFINITIONS_PATH), pRootNode);
  ASSERT(pDefinitions);
  GraphNav::bWalk(pDefinitions, GraphEditor::eValidateDefinitionNode, pRootNode);

  // Validate plan
  PlanNode* pPlanNode = GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRootNode);
  ASSERT(pPlanNode);
  GraphNav::bWalk(pPlanNode, GraphEditor::eValidatePlanNode, pRootNode);

  // Validate metadata
  PlanNode* pMetadata = GraphNav::pFind(const_cast<Stz*>(C_METADATA_PATH), pRootNode);
  ASSERT(pMetadata);
  GraphNav::bWalk(pMetadata, GraphEditor::eValidateMetadataNode, pRootNode);

  // Validate statistics
  PlanNode* pStatistics = GraphNav::pFind(const_cast<Stz*>(C_STATISTICS_PATH), pRootNode);
  ASSERT(pStatistics);
  GraphNav::bWalk(pStatistics, GraphEditor::eValidateStatisticsNode, pRootNode);

  return true;
}

/*----------------------------------------------------------------------------*/
/* Validate only the /plan node against its definitions                       */
/*----------------------------------------------------------------------------*/
b1 GraphEditor::bValidatePlan(PlanNode* pRootNode)
{
  PlanNode* pPlanNode = GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRootNode);
  ASSERT(pPlanNode);
  GraphNav::bWalk(pPlanNode, GraphEditor::eValidatePlanNode, pRootNode);

  return true;
}

/*----------------------------------------------------------------------------*/
/* Validate the root node only                                                */
/*----------------------------------------------------------------------------*/
b1 GraphEditor::bValidateRootNode(PlanNode* pRootNode)
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
  // PLAN-4: Validate the properties:
  //    ROOT-4.1: validate that the properties field is null.
  //
  // PLAN-5: Validate the constraints:
  //    Nothing needed here due to ROOT-4.1.
  //
  // PLAN-6: Validate the children:
  //    ROOT-6.1: validate that the children field is not null.
  //    ROOT-6.2: validate that the children field has 5 items named:
  //              "primitives", "definitions", "plan", "metadata" and "statistics".
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /primitives               */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphEditor::eValidatePrimitiveNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
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
  // PLAN-4: Validate the properties:
  //    PRIM-4.1: validate that the properties field is null.
  //
  // PLAN-5: Validate the constraints:
  //    Nothing needed here due to PRIM-4.1.
  //
  // PLAN-6: Validate the children:
  //    PRIM-6.1: validate that the children field is null.
  //

  return Parser::GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /definitions              */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphEditor::eValidateDefinitionNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
{
  // *********************************
  // Definition Node Validation Rules:
  // *********************************
  //
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
  //              from. The only exception here is the "__constraints__" node.
  //
  // DEFN-5: Validate the constraints:
  //    DEFN-5.1: any constraints must exist under a node named "__constraints__" which
  //              itself is a property of the current node.
  //    DEFN-5.2: if a __constraints__ container exists then it must have 1 or more children.
  //    DEFN-5.2: validate that if the any of the following constraints exist as children
  //              under a __constraints__ container, then those constraints must also have
  //              one child node (being a parameter of the constraint):
  //                - childType
  //                - valueType
  //                - childCount
  //    DEFN-5.3: if any of the constraints specified in DEFN-5.2 exist, then validate that
  //              their parameter nodes must have non-null name, type and value fields.
  //    DEFN-5.4: If the childType constraint is used then validate that its child parameter
  //              must have a value field that is a path that can be resolved to another
  //              definition node.
  //    DEFN-5.5: If the valueType constraint is used then validate that its child parameter
  //              must have a value field that is a path that can be resolved to another
  //              definition node, and that that path is a subclass of valueObject.
  //    DEFN-5.6: validate that the following combinations of constraints cannot be applied
  //              in the __constraints__ container for a given object:
  //                - (noChildren/alwaysChildren),
  //                - (nullable/nonnullable),
  //                - (optional/mandatory)
  //    DEFN-5.7: validate that the constraints combinations specified in DEFN-5.6 cannot occur
  //              with any constraints inherited by the current node.
  //    DEFN-5.8: validate that the same constraint cannot be specified more than once
  //              in the current node.
  //    DEFN-5.9: validate that the same constraint cannot be specified more than once
  //              when considering all inherited constraints of the current node.
  //
  // DEFN-6: Validate the children:
  //    DEFN-6.1: validate that all children of a given definition node must have unique names.
  //



  return Parser::GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /plan                     */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphEditor::eValidatePlanNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
{

  // *********************************
  // Plan Node Validation Rules:
  // *********************************
  //
  // PLAN-1: Validate the name field:
  //    PLAN-1.1: the name field must not be null.
  //
  // PLAN-2: Validate the value field:
  //    No validations here as it is largely dependent on constraints
  //
  // PLAN-3: Validate the type field:
  //    PLAN-3.1: the type field must not be null.
  //    PLAN-3.2: the type field must be an absolute path (start with a '/').
  //    PLAN-3.3: the path in the type field must be resolvable to another
  //              node in the system.
  //    PLAN-3.4: the path in the type field must be resolve to a node in /definitions.
  //    PLAN-3.5:
  //    PLAN-3.6:
  //    PLAN-3.7:
  //    PLAN-3.8:
  //    PLAN-3.9:
  //    PLAN-3.10:
  //
  // PLAN-4: Validate the properties:
  //    PLAN-4.1: the names of the properties must exactly match those specified in
  //              the definition of the current node.
  //    PLAN-4.2: the current node must not have any additional properties that
  //              aren't specified in its definition.
  //    PLAN-4.3: the current node has all properties that are defined in its definition.
  //
  // PLAN-5: Validate the constraints:
  //
  // PLAN-6: Validate the children:
  //    PLAN-6.1: the children must each have a unique name.


  // TODO: When validating a property, its definition may have constraints, but there might
  //       also be additional constraints when it was specified as a property.
  //       e.g. consider this definition of object "foo" and its property "bar":
  //       {
  //          name: "foo",
  //          type: "foo_defn",
  //          properties: [
  //            {
  //              name: "bar",
  //              type: "bar_defn",
  //              properties: [
  //                {
  //                  name: "__constraints__", type: "...", children: [
  //                    { name: "optional", type: "/__root__/primitives/constraints/optional" }
  //                  ]
  //                }
  //              ]
  //            }
  //          ]
  //       }
  //       Given this definition, the property "bar" may have certain constraints from its
  //       definition, but it also has a constraint defined where the property was defined.
  //       So to validate this definition we must evaluate "bar" against its definition
  //       as well as against any additional constraints applied in the declaration of the
  //       property "bar" too.



  // Check for direct definition properties
  // Check for inherited properties
  // Apply/check constraints
  // Apply/check any inherited constraints

  // leaf node should be a data source

  return Parser::GraphNav::WALK_CONTINUE;
}


/*----------------------------------------------------------------------------*/
/* Called when walking each node of the graph under /metadata                 */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl GraphEditor::eValidateMetadataNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
{
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
GraphNav::eWalkCtrl GraphEditor::eValidateStatisticsNode(PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg)
{
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
