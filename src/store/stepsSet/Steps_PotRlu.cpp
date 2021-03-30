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
  vAddStep("potRlu[].completed"_Stz,       &Step::vPotCompleted);
  vAddStep("potRlu[].completedPtn"_Stz,    &Step::vPotCompletedPtn);
  vAddStep("potRlu[].dangling"_Stz,        &Step::vPotDangling);
  vAddStep("potRlu[].danglingPtn"_Stz,     &Step::vPotDanglingPtn);
  vAddStep("potRlu[].delete"_Stz,          &Step::vPotDelete);
  vAddStep("potRlu[].isNull"_Stz,          &Step::vPotIsNull);
  vAddStep("potRlu[].new"_Stz,             &Step::vPotNew);
  vAddStep("potRlu[].pushBuffer"_Stz,      &Step::vPotPushBuffer);
  vAddStep("potRlu[].useStreamDefOut"_Stz, &Step::vPotUseStreamDefOut);
}

/*----------------------------------------------------------------------------*/
/* potRlu[].completed -- get any completed/full buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vPotCompleted(void)
{
  ENTRY();
  
  Pot* pPot = _pPot(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pPot);
    
  DataBuffer* pCompletedBuffer = pPot->pCompleted(0);

  m_pDataStack->vPushPtr(pCompletedBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].completedPtn -- get any completed/full buffer for given ptn       */
/*----------------------------------------------------------------------------*/
void Step::vPotCompletedPtn(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);

  Pot* pPot = _pPot(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pPot);
    
  DataBuffer* pCompletedBuffer = pPot->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pCompletedBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].dangling -- get any dangling/last buffer                          */
/*----------------------------------------------------------------------------*/
void Step::vPotDangling(void)
{
  ENTRY();

  Pot* pPot = _pPot(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pPot);
    
  DataBuffer* pDanglingBuffer = pPot->pDangling(0);

  m_pDataStack->vPushPtr(pDanglingBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].danglingPtn -- get any dangling/last buffer for given ptn         */
/*----------------------------------------------------------------------------*/
void Step::vPotDanglingPtn(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);

  Pot* pPot = _pPot(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pPot);
    
  DataBuffer* pDanglingBuffer = pPot->pDangling(i_Ptn);

  m_pDataStack->vPushPtr(pDanglingBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].delete -- delete pot                                              */
/*----------------------------------------------------------------------------*/
void Step::vPotDelete(void)
{
  ENTRY();

  Pot* pPot = _pPot(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pPot);
  ( m_pJob->pLogicUnit_job() )->m_xRluCommon.m_pPot = nullptr;
  
  delete pPot;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].isNull -- is pot null                                             */
/*----------------------------------------------------------------------------*/
void Step::vPotIsNull(void)
{
  ENTRY();

  Pot* pPot = _pPot(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pPot);

  m_pDataStack->vPushInt(pPot == nullptr ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].new -- create new pot                                             */
/*----------------------------------------------------------------------------*/
void Step::vPotNew(void)
{
  ENTRY();
    
  Pot* pPot = new Pot;
      
  ( m_pJob->pLogicUnit_job() )->m_xRluCommon.m_pPot = pPot;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].pushBuffer -- push top buffer                                     */
/*----------------------------------------------------------------------------*/
void Step::vPotPushBuffer(void)
{
  ENTRY();

  Pot* pPot = _pPot(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pPot);
    
  DataBuffer* pBuffer = pPot->pDangling(0);

  m_pDataStack->vPushPtr(pBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* potRlu[].useStreamDefOut -- use stream def out for this pot                */
/*----------------------------------------------------------------------------*/
void Step::vPotUseStreamDefOut(void)
{
  ENTRY();
    
  i8 iAddPtnHsh = m_pDataStack->iPopInt(__func__);
  
  ( m_pJob->pLogicUnit_job() )->m_xpStreamDefOut->vToRowPix(&( m_pJob->pLogicUnit_job() )->m_oRowPix,
                                               (bool)iAddPtnHsh);
  Pot* pPot = _pPot(m_pJob->pLogicUnit_job()->m_xRluCommon.m_pPot);
  pPot->vRowPix(&( m_pJob->pLogicUnit_job() )->m_oRowPix);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
