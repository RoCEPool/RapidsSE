/*============================================================================*/
/* Steps_Tub.cpp - Working tub of buffers                                     */
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
void Step::vAddSteps_Tub(void)
{
  vAddStep("tub[]"_Stz,               &Step::vTubGet);
  vAddStep("tub[].append"_Stz,        &Step::vTubAppend);
  vAddStep("tub[].completed"_Stz,     &Step::vTubCompleted);
  vAddStep("tub[].dangling"_Stz,      &Step::vTubDangling);
  vAddStep("tub[].delete"_Stz,        &Step::vTubDelete);
  vAddStep("tub[].get"_Stz,           &Step::vTubGet);
  vAddStep("tub[].isEmpty"_Stz,       &Step::vTubIsEmpty);
  vAddStep("tub[].isNull"_Stz,        &Step::vTubIsNull);
  vAddStep("tub[].new"_Stz,           &Step::vTubNew);
}

/*----------------------------------------------------------------------------*/
/* tub[].append -- append a buffer to the tub                                 */
/*----------------------------------------------------------------------------*/
void Step::vTubAppend(void)
{
  ENTRY();

  Tub* pTub = * m_pLogicUnit->m_xpTub;

  DataBuffer* pDataBuffer = _pDataBuffer(m_oDataStack.pPopPtr(__func__));

  pTub->vAddBuffer(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].completed -- get a completed/full buffer                             */
/*----------------------------------------------------------------------------*/
void Step::vTubCompleted(void)
{
  ENTRY();
  
  Tub* pTub = * m_pLogicUnit->m_xpTub;
  
  DataBuffer* pDataBuffer = pTub->pCompletedBuffer();

  m_oDataStack.vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].dangling -- get dangling/first partially full buffer                 */
/*----------------------------------------------------------------------------*/
void Step::vTubDangling(void)
{
  ENTRY();
  
  Tub* pTub = * m_pLogicUnit->m_xpTub;
  
  DataBuffer* pDataBuffer = pTub->pDanglingBuffer();

  m_oDataStack.vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].delete -- delete the tub                                             */
/*----------------------------------------------------------------------------*/
void Step::vTubDelete(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpTub.vDeleteComponent();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].get -- get the tub                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTubGet(void)
{
  ENTRY();
  
  Tub* pTub = * m_pLogicUnit->m_xpTub;
  
  m_oDataStack.vPushPtr(pTub);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].isEmpty -- test if tub is empty                                      */
/*----------------------------------------------------------------------------*/
void Step::vTubIsEmpty(void)
{
  ENTRY();
  
  Tub* pTub = * m_pLogicUnit->m_xpTub;
  
  bool bIsEmpty = pTub->bIsEmpty();
  
  m_oDataStack.vPushInt((i8)bIsEmpty);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].isNull -- test if tub is null                                        */
/*----------------------------------------------------------------------------*/
void Step::vTubIsNull(void)
{
  ENTRY();

  m_oDataStack.vPushInt(m_pLogicUnit->m_xpTub.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].new -- new a tub                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTubNew(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpTub = new Tub;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
