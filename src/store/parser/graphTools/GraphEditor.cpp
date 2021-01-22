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

  // Create a new copy of: /__root__/plan
  QueryNode* pPlanNode = static_cast<QueryNode*>(GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRoot));
  ASSERT(pPlanNode);
  ASSERT(pPlanNode->eGetClassNo() == CN_QUERYNODE);

  PlanNode* pNewPlanChild = new QueryNode(*pPlanNode);

  // Create a new list of children for the new root node. Copy all children except
  // replace the /plan child with the new one we've created.
  PlanNodeList* pNewChildList = new PlanNodeList();

  PlanNodeIter* pIter = pRoot->pNewChildrenIter();
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
