/*============================================================================*/
/* Steps_PotRlu.cpp - Working pot of buffers in Rlu                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PotRlu(void)
{
  vAddStep("potRlu[].completed"_Stz,       &Step::vPotRluCompleted);
  vAddStep("potRlu[].completedPtn"_Stz,    &Step::vPotRluCompletedPtn);
  vAddStep("potRlu[].dangling"_Stz,        &Step::vPotRluDangling);
  vAddStep("potRlu[].danglingPtn"_Stz,     &Step::vPotRluDanglingPtn);
  vAddStep("potRlu[].delete"_Stz,          &Step::vPotRluDelete);
  vAddStep("potRlu[].isNull"_Stz,          &Step::vPotRluIsNull);
  vAddStep("potRlu[].new"_Stz,             &Step::vPotRluNew);
  vAddStep("potRlu[].pushBuffer"_Stz,      &Step::vPotRluPushBuffer);
  vAddStep("potRlu[].rowPix!"_Stz,         &Step::vPotRluRowPixStore);
}

/*----------------------------------------------------------------------------*/
/* potRlu[].completed -- get any completed/full buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vPotRluCompleted(void)
{
  ENTRY();
  
  Pot* pPot = _pPot(m_p_TaskLogicUnit->m_xRluCommon.m_pPot);
    
  DataBuffer* pCompletedBuffer = pPot->pCompleted(0);

  m_pDataStack->vPushPtr(pCompletedBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].completedPtn -- get any completed/full buffer for given ptn       */
/*----------------------------------------------------------------------------*/
void Step::vPotRluCompletedPtn(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);

  Pot* pPot = _pPot(m_p_TaskLogicUnit->m_xRluCommon.m_pPot);
    
  DataBuffer* pCompletedBuffer = pPot->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pCompletedBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].dangling -- get any dangling/last buffer                          */
/*----------------------------------------------------------------------------*/
void Step::vPotRluDangling(void)
{
  ENTRY();

  Pot* pPot = _pPot(m_p_TaskLogicUnit->m_xRluCommon.m_pPot);
    
  DataBuffer* pDanglingBuffer = pPot->pDangling(0);

  m_pDataStack->vPushPtr(pDanglingBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].danglingPtn -- get any dangling/last buffer for given ptn         */
/*----------------------------------------------------------------------------*/
void Step::vPotRluDanglingPtn(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);

  Pot* pPot = _pPot(m_p_TaskLogicUnit->m_xRluCommon.m_pPot);
    
  DataBuffer* pDanglingBuffer = pPot->pDangling(i_Ptn);

  m_pDataStack->vPushPtr(pDanglingBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].delete -- delete pot                                              */
/*----------------------------------------------------------------------------*/
void Step::vPotRluDelete(void)
{
  ENTRY();

  Pot* pPot = _pPot(m_p_TaskLogicUnit->m_xRluCommon.m_pPot);
  m_p_TaskLogicUnit->m_xRluCommon.m_pPot = nullptr;
  
  delete pPot;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].isNull -- is pot null                                             */
/*----------------------------------------------------------------------------*/
void Step::vPotRluIsNull(void)
{
  ENTRY();

  Pot* pPot = _pPot(m_p_TaskLogicUnit->m_xRluCommon.m_pPot);

  m_pDataStack->vPushInt(pPot == nullptr ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].new -- create new pot                                             */
/*----------------------------------------------------------------------------*/
void Step::vPotRluNew(void)
{
  ENTRY();
    
  Pot* pPot = new Pot;
      
  m_p_TaskLogicUnit->m_xRluCommon.m_pPot = pPot;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].pushBuffer -- push top buffer                                     */
/*----------------------------------------------------------------------------*/
void Step::vPotRluPushBuffer(void)
{
  ENTRY();

  Pot* pPot = _pPot(m_p_TaskLogicUnit->m_xRluCommon.m_pPot);
    
  DataBuffer* pBuffer = pPot->pDangling(0);

  m_pDataStack->vPushPtr(pBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].rowPix! -- store rowpix created from stream def out in pot        */
/*----------------------------------------------------------------------------*/
void Step::vPotRluRowPixStore(void)
{
  ENTRY();

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  i8 iAddPtnHsh = m_pDataStack->iPopIntBol(__func__);

  pStreamDef->vToRowPix(&m_p_TaskLogicUnit->m_oRowPix,(bool)iAddPtnHsh);

  Pot* pPot = _pPot(m_p_TaskLogicUnit->m_xRluCommon.m_pPot);
  pPot->vRowPix(&m_p_TaskLogicUnit->m_oRowPix);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
