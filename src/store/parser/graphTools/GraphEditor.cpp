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
#include "GraphNav.hpp"

#include "ShadowPlan.hpp"
#include "MetadataNode.hpp"
#include "StatisticsNode.hpp"

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
  ASSERT(pPrimitives);
  ASSERT(pDefinitions);

  // Create new nodes for plan, metadata and statistics
  QueryNode*      pPlanNode = ShadowPlan::pCreate();
  QueryNode*      pMetadata = MetadataNode::pCreate();
  QueryNode*      pStatistics = StatisticsNode::pCreate();
  ASSERT(pPlanNode);
  ASSERT(pMetadata);
  ASSERT(pStatistics);

  // Create a new list of children for the new root node.
  // Add the shared primitives and definitions, but add in a new
  // plan, metadata and statistics nodes.
  PlanNodeList* pNewChildList = new PlanNodeList;
  pNewChildList->vAppend(pPrimitives);
  pNewChildList->vAppend(pDefinitions);
  pNewChildList->vAppend(pPlanNode);
  pNewChildList->vAppend(pMetadata);
  pNewChildList->vAppend(pStatistics);

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

/*----------------------------------------------------------------------------*/
/* Remove and replace pReplaceChild in parent node pParentNode with the       */
/* nodes in pReplacementList. Moves nodes from pReplacementList.              */
/* Deletes the pReplaceChild. Does not delete pReplacementList.               */
/*----------------------------------------------------------------------------*/
void GraphEditor::vReplaceChild(QueryNode* pParentNode, QueryNode* pReplaceChild,
                                QueryNodeList* pReplacementList)
{
  ASSERT(pParentNode);
  ASSERT(pParentNode->m_pChildren);
  ASSERT(pReplaceChild);
  ASSERT(pReplacementList);

  // We are going to create a new list of children and replace the existing
  // child list in the parent object.

  PlanNodeList* pOldChildList = pParentNode->m_pChildren;
  PlanNodeList* pNewChildList = new PlanNodeList();

  PlanNode* pChild;

  // Order is important here, so move all of the children we find before pReplaceChild
  // into the new list.
  while ((pChild = pOldChildList->pPop()) != pReplaceChild && pChild != nullptr) {
    pNewChildList->vAppend(pChild);
  }

  ASSERT(pChild == pReplaceChild);

  // Now move the replacements
  while ((pChild = pReplacementList->pPop()) != nullptr) {
    pNewChildList->vAppend(pChild);
  }

  // Now move any remaining children from the old list to the new list
  while ((pChild = pOldChildList->pPop()) != nullptr) {
    pNewChildList->vAppend(pChild);
  }

  // clean up
  delete pReplaceChild;
  ASSERT(pOldChildList->nItems() == 0);
  delete pOldChildList;

  // Set the new list as the children list in the parent object
  pParentNode->m_pChildren = pNewChildList;
}

/*----------------------------------------------------------------------------*/
/* Remove a child from its parent and optionally transplant that child's      */
/* children in its place. Returns the snipped child.                          */
/*----------------------------------------------------------------------------*/
QueryNode* GraphEditor::pSnipChild(QueryNode* pParent, QueryNode* pChild, b1 bTransplant)
{
  ASSERT(pParent);
  ASSERT(pParent->m_pChildren);
  ASSERT(pChild);
  ASSERT(pChild->m_pChildren);

  if (bTransplant) {
    // We need to move the children of pChild to be children of pParent.
    // We also want to put them where pChild is.
    pParent->vInsertChildrenBefore(pChild->m_pChildren, pChild);
    pChild->m_pChildren->vRemoveAll();
  }

  // Now we can remove pChild from being a child of pParent
  pParent->vRemoveChild(pChild);
  return pChild;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
