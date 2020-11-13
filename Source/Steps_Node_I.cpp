/*============================================================================*/
/* Steps_Node_I.cpp - node iterator step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Node_I(void)
{
  vAddStep("node_I[].delete"_Stz,  &Step::vNode_IDelete);
  vAddStep("node_I[].more"_Stz,    &Step::vNode_IMore);
  vAddStep("node_I[].new"_Stz,     &Step::vNode_INew);
  vAddStep("node_I[].next"_Stz,    &Step::vNode_INext);
  vAddStep("node_I[].node"_Stz,    &Step::vNode_INode);
}

/*----------------------------------------------------------------------------*/
/* node_I[].delete -- delete a node iterator                                  */
/*----------------------------------------------------------------------------*/
void Step::vNode_IDelete(void)
{
  ENTRY();
  
  Node_I* pNode_I = *m_pLogicUnit->m_xpNode_I;
  m_pLogicUnit->m_xpNode_I = nullptr;
  
  delete pNode_I;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node_I[].more -- more nodes                                                */
/*----------------------------------------------------------------------------*/
void Step::vNode_IMore(void)
{
  ENTRY();
  
  bool bMore = m_pLogicUnit->m_xpNode_I->bMore();
  
  m_pDataStack->vPushInt(bMore);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node_I[].new -- create a new node iterator                                 */
/*----------------------------------------------------------------------------*/
void Step::vNode_INew(void)
{
  ENTRY();
  
  Node_I* pNode_I = new Node_I;
      
  m_pLogicUnit->m_xpNode_I = pNode_I;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node_I[].next -- advance to next node                                      */
/*----------------------------------------------------------------------------*/
void Step::vNode_INext(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpNode_I->vNext();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node_I[].node -- push current node                                         */
/*----------------------------------------------------------------------------*/
void Step::vNode_INode(void)
{
  ENTRY();
  
  Node* pNode = m_pLogicUnit->m_xpNode_I->pNode();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
