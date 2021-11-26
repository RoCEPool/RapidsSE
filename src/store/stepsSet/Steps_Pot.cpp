/*============================================================================*/
/* Steps_Pot.cpp - Working pot of buffers in LU                               */
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
void Step::vAddSteps_Pot(void)
{
  vAddStep("pot[].completed"_Stz,       &Step::vPotCompleted);
  vAddStep("pot[].completedPtn"_Stz,    &Step::vPotCompletedPtn);
  vAddStep("pot[].dangling"_Stz,        &Step::vPotDangling);
  vAddStep("pot[].danglingPtn"_Stz,     &Step::vPotDanglingPtn);
  vAddStep("pot[].delete"_Stz,          &Step::vPotDelete);
  vAddStep("pot[].isNull"_Stz,          &Step::vPotIsNull);
  vAddStep("pot[].new"_Stz,             &Step::vPotNew);
  vAddStep("pot[].pushBuffer"_Stz,      &Step::vPotPushBuffer);
  vAddStep("pot[].rowPix!"_Stz,         &Step::vPotRowPixStore);
}

/*----------------------------------------------------------------------------*/
/* pot[].completed -- get any completed/full buffer                           */
/*----------------------------------------------------------------------------*/
void Step::vPotCompleted(void)
{
  ENTRY();
  
  Pot* pPot = _pPot(*m_p_TaskLogicUnit->m_xpPot);
    
  DataBuffer* pCompletedBuffer = pPot->pCompleted(0);

  m_pDataStack->vPushPtr(pCompletedBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].completedPtn -- get any completed/full buffer for given ptn          */
/*----------------------------------------------------------------------------*/
void Step::vPotCompletedPtn(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);

  Pot* pPot = _pPot(*m_p_TaskLogicUnit->m_xpPot);
    
  DataBuffer* pCompletedBuffer = pPot->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pCompletedBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].dangling -- get any dangling/last buffer                             */
/*----------------------------------------------------------------------------*/
void Step::vPotDangling(void)
{
  ENTRY();

  Pot* pPot = _pPot(*m_p_TaskLogicUnit->m_xpPot);
    
  DataBuffer* pDanglingBuffer = pPot->pDangling(0);

  m_pDataStack->vPushPtr(pDanglingBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].danglingPtn -- get any dangling/last buffer for given ptn            */
/*----------------------------------------------------------------------------*/
void Step::vPotDanglingPtn(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);

  Pot* pPot = _pPot(*m_p_TaskLogicUnit->m_xpPot);
    
  DataBuffer* pDanglingBuffer = pPot->pDangling(i_Ptn);

  m_pDataStack->vPushPtr(pDanglingBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].delete -- delete pot                                                 */
/*----------------------------------------------------------------------------*/
void Step::vPotDelete(void)
{
  ENTRY();

  Pot* pPot = _pPot(*m_p_TaskLogicUnit->m_xpPot);
  m_p_TaskLogicUnit->m_xpPot = nullptr;
  
  delete pPot;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].isNull -- is pot null                                                */
/*----------------------------------------------------------------------------*/
void Step::vPotIsNull(void)
{
  ENTRY();

  Pot* pPot = _pPot(*m_p_TaskLogicUnit->m_xpPot);

  m_pDataStack->vPushInt(pPot == nullptr ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].new -- create new pot                                                */
/*----------------------------------------------------------------------------*/
void Step::vPotNew(void)
{
  ENTRY();
    
  Pot* pPot = new Pot;
      
  m_p_TaskLogicUnit->m_xpPot = pPot;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].pushBuffer -- push top buffer                                        */
/*----------------------------------------------------------------------------*/
void Step::vPotPushBuffer(void)
{
  ENTRY();

  Pot* pPot = _pPot(*m_p_TaskLogicUnit->m_xpPot);
    
  DataBuffer* pBuffer = pPot->pDangling(0);

  m_pDataStack->vPushPtr(pBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].rowPix! -- store rowpix created from stream def out in pot           */
/*----------------------------------------------------------------------------*/
void Step::vPotRowPixStore(void)
{
  ENTRY();

  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  i8 iAddPtnHsh = m_pDataStack->iPopInt(__func__);

  pStreamDef->vToRowPix(&m_p_TaskLogicUnit->m_oRowPix,(bool)iAddPtnHsh);

  Pot* pPot = _pPot(*m_p_TaskLogicUnit->m_xpPot);
  pPot->vRowPix(&m_p_TaskLogicUnit->m_oRowPix);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
