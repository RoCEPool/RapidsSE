/*============================================================================*/
/* Steps_Object.cpp - object step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "LogicUnit.hpp"

extern SizeHeap gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RecvWL(void)
{
  vAddStep("recvWL[]"_Stz,          &Step::vRecvWLGet);
  vAddStep("recvWL[].get"_Stz,      &Step::vRecvWLGet);
  vAddStep("recvWL[].isNotNull"_Stz,&Step::vRecvWLIsNotNull);
  vAddStep("recvWL[].isNull"_Stz,   &Step::vRecvWLIsNull);
  vAddStep("recvWL[].set"_Stz,      &Step::vRecvWLSet);
}

/*----------------------------------------------------------------------------*/
/* recvWL[].get -- get the value in recveived object                     */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRecvWL);
}

/*----------------------------------------------------------------------------*/
/* recvWL[].isNotNull -- is recvWL not null ?                       */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRecvWL.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* recvWL[].isNull -- is recvWL null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRecvWL.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* recvWL[].set -- set the value in recveived object                     */
/*----------------------------------------------------------------------------*/
void Step::vRecvWLSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xpRecvWL = (RecvWL*)m_pDataStack->pPopPtr(__func__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
