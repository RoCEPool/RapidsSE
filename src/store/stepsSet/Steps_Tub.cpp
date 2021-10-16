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
  vAddStep("tub[].completed"_Stz,     &Step::vTubCompleted);
  vAddStep("tub[].dangling"_Stz,      &Step::vTubDangling);
  vAddStep("tub[].delete"_Stz,        &Step::vTubDelete);
  vAddStep("tub[].get"_Stz,           &Step::vTubGet);
  vAddStep("tub[].isEmpty"_Stz,       &Step::vTubIsEmpty);
  vAddStep("tub[].new"_Stz,           &Step::vTubNew);
}

/*----------------------------------------------------------------------------*/
/* tub[].completed -- get a completed/full buffer                             */
/*----------------------------------------------------------------------------*/
void Step::vTubCompleted(void)
{
  ENTRY();
  
  Tub* pTub = * m_p_TaskLogicUnit->m_xpTub;
  
  DataBuffer* pDataBuffer = pTub->pCompletedBuffer();

  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].dangling -- get dangling/first partially full buffer                 */
/*----------------------------------------------------------------------------*/
void Step::vTubDangling(void)
{
  ENTRY();
  
  Tub* pTub = * m_p_TaskLogicUnit->m_xpTub;
  
  DataBuffer* pDataBuffer = pTub->pDanglingBuffer();

  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].delete -- delete the tub                                             */
/*----------------------------------------------------------------------------*/
void Step::vTubDelete(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpTub.vDeleteComponent();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].get -- get the tub                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTubGet(void)
{
  ENTRY();
  
  Tub* pTub = * m_p_TaskLogicUnit->m_xpTub;
  
  m_pDataStack->vPushPtr(pTub);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].isEmpty -- test if tub is empty                                      */
/*----------------------------------------------------------------------------*/
void Step::vTubIsEmpty(void)
{
  ENTRY();
  
  Tub* pTub = * m_p_TaskLogicUnit->m_xpTub;
  
  bool bIsEmpty = pTub->bIsEmpty();
  
  m_pDataStack->vPushInt((i8)bIsEmpty);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tub[].new -- new a tub                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTubNew(void)
{
  ENTRY();
  
  m_p_TaskLogicUnit->m_xpTub = new Tub;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
