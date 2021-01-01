/*============================================================================*/
/* Steps_Node_I.cpp - node iterator step group                                */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


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
  
  Node_I* pNode_I = *pLogicUnit_task()->m_xpNode_I;
  pLogicUnit_task()->m_xpNode_I = nullptr;
  
  delete pNode_I;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node_I[].more -- more nodes                                                */
/*----------------------------------------------------------------------------*/
void Step::vNode_IMore(void)
{
  ENTRY();
  
  bool bMore = pLogicUnit_task()->m_xpNode_I->bMore();
  
  m_pDataStack->vPushInt(bMore);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node_I[].new -- create a new node iterator                                 */
/*----------------------------------------------------------------------------*/
void Step::vNode_INew(void)
{
  ENTRY();
  
  i8 iStipulate = m_pDataStack->iPopInt(__func__);
  
  Node_I* pNode_I = new Node_I(0,iStipulate);
      
  pLogicUnit_task()->m_xpNode_I = pNode_I;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node_I[].next -- advance to next node                                      */
/*----------------------------------------------------------------------------*/
void Step::vNode_INext(void)
{
  ENTRY();
  
  pLogicUnit_task()->m_xpNode_I->vNext();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node_I[].node -- push current node                                         */
/*----------------------------------------------------------------------------*/
void Step::vNode_INode(void)
{
  ENTRY();
  
  Node* pNode = pLogicUnit_task()->m_xpNode_I->pNode();

  m_pDataStack->vPushPtr(pNode);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
