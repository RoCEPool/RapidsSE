/*============================================================================*/
/* RootNode.cpp - An immutable node that is the top of the plan graph         */
/*============================================================================*/

#include "STORE.h"
#include "RootNode.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,RootNode,CN_ROOTNODE)
__aNewFunction(RootNode,CN_ROOTNODE)
__vDeleteFunction(RootNode,CN_ROOTNODE)
__aFooterFunction(RootNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RootNode::RootNode(Stz* szName, Stz* szType, Value xValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren,
                   Stz* szPath)
    : PlanNode(CN_ROOTNODE, szName, szType, xValue, pProperties, pChildren),
      m_sPath(szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROOTNODE,_FOOTER_);

  _checkInitialised_(CN_ROOTNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROOTNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RootNode::~RootNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROOTNODE,_FOOTER_);

  // Don't delete the /primitives and /definitions children because they
  // will be reused by other plan graphs.

  // TODO: Uncomment this when we have a list class with separate node pointers
//  PlanNode* pItem = m_pChildren->pHead();
//
//  while (pItem) {
//    if (pItem->bIsClass(CN_DEFINITIONNODE) || pItem->bIsClass(CN_PRIMITIVENODE)) {
//      PlanNode* pNext = pItem->pNextItem();
//      m_pChildren->vRemove(pItem);
//      pItem = pNext;
//      continue;
//    }
//
//    pItem = pItem->pNextItem();
//  }

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the path of this node in the hierarchy.                            */
/*----------------------------------------------------------------------------*/
Stz* RootNode::szPath(void)
{
  // Child elements are separated from parents using the '/' character.
  // Property elements are separated from their parent using the ':' character.
  return m_sPath.szStz();
}

/*----------------------------------------------------------------------------*/
/* Returns the description of this definition.                                */
/*----------------------------------------------------------------------------*/
Stz* RootNode::szDescription(void)
{
  ASSERT(m_xValue.m_pText);
  return m_xValue.m_pText->szGet();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

