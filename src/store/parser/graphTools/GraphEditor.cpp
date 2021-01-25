/*============================================================================*/
/* GraphEditor.cpp - a utility for walking plan graphs                         */
/*============================================================================*/

#include "BASE.h"
#include "GraphEditor.hpp"
#include "PlanNode.hpp"
#include "PrimitiveNode.hpp"
#include "PlanConstants.hpp"
#include "QueryNode.hpp"
#include "RootNode.hpp"
#include "MetadataNode.hpp"
#include "StatisticsNode.hpp"
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
PlanNode* GraphEditor::pNewFromTemplate(const PlanNode* pTemplateRoot)
{
  ASSERT(pTemplateRoot);

  // Ugly hack to avoid a proliferation of const_casts or marking functions
  // as const (which will infect the rest of the system).
  PlanNode* pRoot = const_cast<PlanNode*>(pTemplateRoot);

  ASSERT(pRoot->szName()->bEqual(const_cast<Stz*>(C_ROOT_NAME)));

  // Find each of the child nodes
  PlanNode*       pPrimitives  = GraphNav::pFind(const_cast<Stz*>(C_PRIMITIVES_PATH), pRoot);
  PlanNode*       pDefinitions = GraphNav::pFind(const_cast<Stz*>(C_DEFINITIONS_PATH), pRoot);
  QueryNode*      pPlanNode    = static_cast<QueryNode*>(GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRoot));
  MetadataNode*   pMetadata    = static_cast<MetadataNode*>(GraphNav::pFind(const_cast<Stz*>(C_METADATA_PATH), pRoot));
  StatisticsNode* pStatistics  = static_cast<StatisticsNode*>(GraphNav::pFind(const_cast<Stz*>(C_STATISTICS_PATH), pRoot));
  ASSERT(pPrimitives);
  ASSERT(pDefinitions);
  ASSERT(pPlanNode);
  ASSERT(pMetadata);
  ASSERT(pStatistics);

  // Create a new list of children for the new root node.
  // Add the shared primitives and definitions, but add in a new
  // plan, metadata and statistics nodes.
  PlanNodeList* pNewChildList = new PlanNodeList();
  pNewChildList->vAppend(pPrimitives);
  pNewChildList->vAppend(pDefinitions);
  pNewChildList->vAppend(new QueryNode(*pPlanNode));
  pNewChildList->vAppend(new MetadataNode(*pMetadata));
  pNewChildList->vAppend(new StatisticsNode(*pStatistics));

  // Create a new list of properties for the new root node.
  PlanNodeList* pNewPropertyList = nullptr;

  if (pRoot->bHasProperties()) {

    PlanNodeIter* pIter = pRoot->pNewPropertyIter();
    pNewPropertyList = new PlanNodeList(pIter);
    delete pIter;
  }

  // Now we can create a new copy of: /__root__
  PlanNode* pNewRoot = new RootNode(pRoot->szName(),
                                    pRoot->szType(),
                                    static_cast<Value&&>(pRoot->pValue()->xDup()),
                                    pNewPropertyList,
                                    pNewChildList,
                                    const_cast<Stz*>(C_ROOT_PATH));

  return pNewRoot;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
