/*============================================================================*/
/* GraphEditor.cpp - a utility for walking plan graphs                         */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,GraphEditor,CN_GRAPHEDITOR)
__aNewFunction(GraphEditor,CN_GRAPHEDITOR)
__vDeleteFunction(GraphEditor,CN_GRAPHEDITOR)
__aFooterFunction(GraphEditor)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GraphEditor::GraphEditor()
    : Object(CN_GRAPHEDITOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHEDITOR,_FOOTER_);

  _checkInitialised_(CN_GRAPHEDITOR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_GRAPHEDITOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
GraphEditor::~GraphEditor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GRAPHEDITOR,_FOOTER_);
}

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
  QueryNode* pPlanNode = (QueryNode*) GraphNav::pFind("/__root__/plan"_Stz, pRootNode);
  ASSERT(pPlanNode);
  ASSERT(pPlanNode->eClass() == CN_QUERYNODE);

  PlanNode* pNewPlanChild = new QueryNode(*pPlanNode);

  // Create a new list of children for the new root node. Copy all children except
  // replace the /plan child with the new one we've created.
  PlanNodeList* pNewChildList = new PlanNodeList();

  PlanNode* pItem = pRootNode->pFirstChild();

  while (pItem) {
    if (pItem == pPlanNode) {
      // Copy in the new /plan child, not the old one
      pNewChildList->vAppend(pNewPlanChild);

    } else {
      pNewChildList->vAppend(pItem);
    }

    pItem = pItem->pNextItem();
  }

  // Create a new list of properties for the new root node.
  PlanNodeList* pNewPropertyList = nullptr;

  if (pRootNode->bHasProperties()) {
    pItem = pRootNode->pFirstProperty();

    while (pItem) {
      pNewPropertyList->vAppend(pItem);
      pItem = pItem->pNextItem();
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



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
