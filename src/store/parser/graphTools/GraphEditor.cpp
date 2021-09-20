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
/* Remove and replace pExistingChild in parent node pParentNode with          */
/* pReplacementNode. Deletes pExistingChild.                                  */
/*----------------------------------------------------------------------------*/
void GraphEditor::vReplaceChild(QueryNode* pParentNode, QueryNode* pExistingChild,
                                QueryNode* pReplacementNode)
{
  ASSERT(pParentNode);
  ASSERT(pParentNode->m_pChildren);
  ASSERT(pExistingChild);
  ASSERT(pReplacementNode);

  // We are going to create a new list of children and replace the existing
  // child list in the parent object.

  PlanNodeList* pOldChildList = pParentNode->m_pChildren;
  PlanNodeList* pNewChildList = new PlanNodeList();

  PlanNode* pChild;

  while ((pChild = pOldChildList->pPop()) != nullptr) {

    if (pChild == pExistingChild) {
      // replace this child
      delete pExistingChild;
      pNewChildList->vAppend(pReplacementNode);

    } else {
      // keep this child
      pNewChildList->vAppend(pChild);
    }
  }

  // clean up
  RASSERT(pOldChildList->nItems() == 0);
  delete pOldChildList;

  // Set the new list as the children list in the parent object
  pParentNode->m_pChildren = pNewChildList;
}


/*----------------------------------------------------------------------------*/
/* Remove and replace pExistingChild in parent node pParentNode with the       */
/* nodes in pReplacementList. Moves nodes from pReplacementList.              */
/* Deletes the pExistingChild. Does not delete pReplacementList.               */
/*----------------------------------------------------------------------------*/
void GraphEditor::vReplaceChild(QueryNode* pParentNode, QueryNode* pExistingChild,
                                QueryNodeList* pReplacementList)
{
  ASSERT(pParentNode);
  ASSERT(pParentNode->m_pChildren);
  ASSERT(pExistingChild);
  ASSERT(pReplacementList);

  // We are going to create a new list of children and replace the existing
  // child list in the parent object.

  PlanNodeList* pOldChildList = pParentNode->m_pChildren;
  PlanNodeList* pNewChildList = new PlanNodeList();

  PlanNode* pChild;

  // Order is important here, so move all of the children we find before pExistingChild
  // into the new list.
  while ((pChild = pOldChildList->pPop()) != pExistingChild && pChild != nullptr) {
    pNewChildList->vAppend(pChild);
  }

  ASSERT(pChild == pExistingChild);

  // Now move the replacements
  while ((pChild = pReplacementList->pPop()) != nullptr) {
    pNewChildList->vAppend(pChild);
  }

  // Now move any remaining children from the old list to the new list
  while ((pChild = pOldChildList->pPop()) != nullptr) {
    pNewChildList->vAppend(pChild);
  }

  // clean up
  delete pExistingChild;
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


/*----------------------------------------------------------------------------*/
/* Remove child nodes from its parent if the child nodes meet some condition. */
/* Optionally transplant the child's children in its place.                   */
/* Returns nullptr if no children were replaced, otherwise a list of the      */
/* replaced children is returned. List and children need to be deleted.       */
/* The condition that is evaluated is passed the child node being tested      */
/* and must return true if the child is to be snipped, or false it it         */
/* is to be left alone.                                                       */
/*----------------------------------------------------------------------------*/
QueryNodeList* GraphEditor::pSnipChild(QueryNode* pParent, std::function<b1(QueryNode*)> fCondition, b1 bTransplant)
{
  ASSERT(pParent);

  if (pParent->nChildren() == 0) {
    return nullptr;
  }

  PlanNodeIter* pIter = pParent->pNewChildrenIter();
  QueryNodeList* pSnippedChildren = nullptr;

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());

    // Test this child
    b1 bSnip = fCondition(pChild);
    if (!bSnip) {
      continue;
    }

    if (!pSnippedChildren) {
      pSnippedChildren = new QueryNodeList();
    }

    pSnippedChildren->vAppend(pChild);
    pSnipChild(pParent, pChild, bTransplant);
  }

  delete pIter;
  return pSnippedChildren;
}


/*----------------------------------------------------------------------------*/
/* Connects two distinct chains of nodes (nodes with at most 1 child each)    */
/* by making pSecond a child of the leaf node of pFirst.                      */
/* i.e. navigate down the child hierarchy of pFirst to find the leaf node     */
/* and make pSecond a child of it.                                            */
/* e.g. if pFirst was /a/b/c and pSecond was /d/e then pFirst would end up    */
/* as /a/b/c/d/e                                                              */
/*----------------------------------------------------------------------------*/
void GraphEditor::vConnectChains(QueryNode* pFirst, QueryNode* pSecond)
{
  ASSERT(pFirst);
  ASSERT(pSecond);

  QueryNode* pNode = pFirst;
  while (pNode->nChildren() != 0) {
    ASSERT(pNode->nChildren() == 1);
    pNode = _pQueryNode(pNode->pFirstChild());
  }

  ASSERT(pNode->nChildren() == 0);
  pNode->vAppendChild(pSecond);
}


/*----------------------------------------------------------------------------*/
/* Search for pExistingNode in the graph, starting at pStartingNode, and      */
/* replace it with pNewNode. pExistingNode must exist in the graph.           */
/*----------------------------------------------------------------------------*/
void GraphEditor::vReplaceNode(PlanNode* pStartingNode, QueryNode* pExistingNode, QueryNode* pNewNode)
{
  ASSERT(pStartingNode);
  ASSERT(pExistingNode);
  ASSERT(pNewNode);

  // Get the path of pStartingNode
  s2K sPathBuf;
  b1 bFound = GraphNav::bGetPath(pExistingNode, pStartingNode, sPathBuf);
  RASSERT(bFound);

  // Get the parent object of pStartingNode
  QueryNode* pParent = _pQueryNode(GraphNav::pGetParentOfPath(sPathBuf.szStz(), pStartingNode));
  ASSERT(pParent);

  vReplaceChild(pParent, pExistingNode, pNewNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
