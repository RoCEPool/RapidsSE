/*============================================================================*/
/* Steps_Pot.cpp - Working pot of buffers                                     */
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
void Step::vAddSteps_Pot(void)
{
  vAddStep("pot[].completed"_Stz,       &Step::vPotCompleted);
  vAddStep("pot[].completedPtn"_Stz,    &Step::vPotCompletedPtn);
  vAddStep("pot[].dangling"_Stz,        &Step::vPotDangling);
  vAddStep("pot[].danglingPtn"_Stz,     &Step::vPotDanglingPtn);
  vAddStep("pot[].delete"_Stz,          &Step::vPotDelete);
  vAddStep("pot[].new"_Stz,             &Step::vPotNew);
  vAddStep("pot[].useStreamDefOut"_Stz, &Step::vPotUseStreamDefOut);
}

/*----------------------------------------------------------------------------*/
/* pot[].completed -- get any completed/full buffer                           */
/*----------------------------------------------------------------------------*/
void Step::vPotCompleted(void)
{
  ENTRY();
  
  Pot* pPot = *pLogicUnit_task()->m_xpPot;
    
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
  
  Pot* pPot = *pLogicUnit_task()->m_xpPot;
    
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
  
  Pot* pPot = *pLogicUnit_task()->m_xpPot;
    
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
  
  Pot* pPot = *pLogicUnit_task()->m_xpPot;
    
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
  
  Pot* pPot = *pLogicUnit_task()->m_xpPot;
  pLogicUnit_task()->m_xpPot = nullptr;
  
  delete pPot;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].new -- create new pot                                                */
/*----------------------------------------------------------------------------*/
void Step::vPotNew(void)
{
  ENTRY();
    
  Pot* pPot = new Pot;
      
  pLogicUnit_task()->m_xpPot = pPot;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].useStreamDefOut -- use stream def out for this pot                   */
/*----------------------------------------------------------------------------*/
void Step::vPotUseStreamDefOut(void)
{
  ENTRY();
    
  i8 iAddPtnHsh = m_pDataStack->iPopInt(__func__);
  
  pLogicUnit_task()->m_xpStreamDefOut->vToRowPix(&pLogicUnit_task()->m_oRowPix,
                                               (bool)iAddPtnHsh);
  
  pLogicUnit_task()->m_xpPot->vRowPix(&pLogicUnit_task()->m_oRowPix);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
