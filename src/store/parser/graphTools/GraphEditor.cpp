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

  PlanNodeIter* pIter = pRootNode->pNewChildrenIter();
  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (pChild == pPlanNode) {
      // Copy in the new /plan child, not the old one
      pNewChildList->vAppend(pNewPlanChild);

    } else {
      pNewChildList->vAppend(pChild);
    }
  }
  delete pIter;


  // Create a new list of properties for the new root node.
  PlanNodeList* pNewPropertyList = nullptr;

  if (pRootNode->bHasProperties()) {

    PlanNodeIter* pIter = pRootNode->pNewPropertyIter();
    pNewPropertyList = new PlanNodeList(pIter);
    delete pIter;
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
