/*============================================================================*/
/* Steps_SendNode.cpp - send node step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node.hpp"
#include "Grid.hpp"

#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SendNode(void)
{
  vAddStep("SendNode[]"_Stz,      &Step::vSendNodeGet);
  vAddStep("SendNode[].get"_Stz,  &Step::vSendNodeGet);
  vAddStep("SendNode[].set"_Stz,  &Step::vSendNodeSet);
}

/*----------------------------------------------------------------------------*/
/* SendNode[].get -- get sendNode                                             */
/*----------------------------------------------------------------------------*/
void Step::vSendNodeGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpSendNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SendNode[].set -- set sendNode                                             */
/*----------------------------------------------------------------------------*/
void Step::vSendNodeSet(void)
{
  ENTRY();
  
  Node* pSendNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pSendNode || m_p_TaskLogicUnit->m_xpSendNode.bIsNull());
  
  m_pLogicUnit->m_xpSendNode = pSendNode;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
