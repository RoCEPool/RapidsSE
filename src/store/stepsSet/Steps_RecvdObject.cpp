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
void Step::vAddSteps_RecvdObject(void)
{
  vAddStep("recvdObject[]"_Stz,          &Step::vRecvdObjectGet);
  vAddStep("recvdObject[].get"_Stz,      &Step::vRecvdObjectGet);
  vAddStep("recvdObject[].isNotNull"_Stz,&Step::vRecvdObjectIsNotNull);
  vAddStep("recvdObject[].isNull"_Stz,   &Step::vRecvdObjectIsNull);
  vAddStep("recvdObject[].set"_Stz,      &Step::vRecvdObjectSet);
}

/*----------------------------------------------------------------------------*/
/* recvdObject[].get -- get the value in recveived object                     */
/*----------------------------------------------------------------------------*/
void Step::vRecvdObjectGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpRecvdObject);
}

/*----------------------------------------------------------------------------*/
/* recvdObject[].isNotNull -- is recvdObject not null ?                       */
/*----------------------------------------------------------------------------*/
void Step::vRecvdObjectIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpRecvdObject.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* recvdObject[].isNull -- is recvdObject null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vRecvdObjectIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpRecvdObject.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* recvdObject[].set -- set the value in recveived object                     */
/*----------------------------------------------------------------------------*/
void Step::vRecvdObjectSet(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_xpRecvdObject = m_pDataStack->pPopPtr(__func__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
