/*============================================================================*/
/* Steps_BufferId.cpp - BufferId step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_BufferId(void)
{
  vAddStep("BufferId[]"_Stz,                &Step::vBufferIdGet);
  vAddStep("BufferId[]\\clear"_Stz,         &Step::vBufferIdGetAndClear);
  vAddStep("BufferId[].clear"_Stz,           &Step::vBufferIdClear);
  vAddStep("BufferId[].dropLock"_Stz,        &Step::vBufferIdDropLock);
  vAddStep("BufferId[].get"_Stz,            &Step::vBufferIdGet);
  vAddStep("BufferId[].getDataBuffer"_Stz,   &Step::vBufferIdGetDataBuffer);
  vAddStep("BufferId[].isNotNull"_Stz,      &Step::vBufferIdIsNotNull);
  vAddStep("BufferId[].isNull"_Stz,         &Step::vBufferIdIsNull);
  vAddStep("BufferId[].set"_Stz,            &Step::vBufferIdSet);
  vAddStep("BufferId[].deRefset"_Stz,        &Step::vBufferIdDeRefSet);
}

/*----------------------------------------------------------------------------*/
/* BufferId[].clear -- operand clear                                          */
/*----------------------------------------------------------------------------*/
void Step::vBufferIdClear(void)
{
  ENTRY();

  m_pLogicUnit->m_oBufferId.m_pvAsAddress = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BufferId[].dropLock -- drop HashMapEntry lock with this BufferId key       */
/*----------------------------------------------------------------------------*/
void Step::vBufferIdDropLock(void)
{
  ENTRY();


  AM::pHashMap()->vDropKeyLock(m_pLogicUnit->m_oBufferId.m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BufferId[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vBufferIdGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr((Object*)m_pLogicUnit->m_oBufferId.m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BufferId[].getDataBuffer -- convert bufferid to databuffer address via hashmap*/
/*----------------------------------------------------------------------------*/
void Step::vBufferIdGetDataBuffer(void)
{
  ENTRY();

  DataBuffer* pDataBuffer = _pDataBuffer(AM::pHashMap()->pValueAt(m_pLogicUnit->m_oBufferId.m_pvAsAddress));
  RASSERT(pDataBuffer != nullptr);
  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BufferId[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vBufferIdGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr((Object*)m_pLogicUnit->m_oBufferId.m_pvAsAddress);

  m_pLogicUnit->m_oBufferId.m_pvAsAddress = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BufferId[].isNotNull -- is BufferId not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vBufferIdIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_oBufferId.m_pvAsAddress == nullptr ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BufferId[].isNull -- is BufferId null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vBufferIdIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_oBufferId.m_pvAsAddress == nullptr ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BufferId[].set -- set operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vBufferIdSet(void)
{
  ENTRY();

  m_pLogicUnit->m_oBufferId.m_pvAsAddress = m_pDataStack->pPopPtr(__func__);
  if(m_pLogicUnit->m_oBufferId.m_pvAsAddress != nullptr)
    ASSERT(m_pLogicUnit->m_oBufferId.m_bIdFlag == 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* BufferId[].deRefSet -- dereference and set buffer id                       */
/*----------------------------------------------------------------------------*/
void Step::vBufferIdDeRefSet(void)
{
  ENTRY();

  BufferId* pBufferId = (BufferId*)m_pDataStack->pPopPtr(__func__);

  RASSERT(pBufferId != nullptr);
  m_pLogicUnit->m_oBufferId.m_pvAsAddress = pBufferId->m_pvAsAddress;
  if(m_pLogicUnit->m_oBufferId.m_pvAsAddress != nullptr)
      ASSERT(m_pLogicUnit->m_oBufferId.m_bIdFlag == 1);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
