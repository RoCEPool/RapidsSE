/*============================================================================*/
/* Steps_Pot.cpp - Working pot of buffers                                     */
/*============================================================================*/

#include "Steps.h"

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
  
  Pot* pPot = *m_pLogicUnit->m_xpPot;
    
  DataBuffer* pDataBuffer = pPot->pCompleted(0);

  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].completedPtn -- get any completed/full buffer for given ptn          */
/*----------------------------------------------------------------------------*/
void Step::vPotCompletedPtn(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  Pot* pPot = *m_pLogicUnit->m_xpPot;
    
  DataBuffer* pDataBuffer = pPot->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].danglingRsbBuffer -- get any dangling/last buffer                    */
/*----------------------------------------------------------------------------*/
void Step::vPotDangling(void)
{
  ENTRY();
  
  Pot* pPot = *m_pLogicUnit->m_xpPot;
    
  DataBuffer* pDataBuffer = pPot->pDangling(0);

  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].danglingPtn -- get any dangling/last buffer for given ptn            */
/*----------------------------------------------------------------------------*/
void Step::vPotDanglingPtn(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  Pot* pPot = *m_pLogicUnit->m_xpPot;
    
  DataBuffer* pDataBuffer = pPot->pDangling(i_Ptn);

  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].delete -- delete pot                                                 */
/*----------------------------------------------------------------------------*/
void Step::vPotDelete(void)
{
  ENTRY();
  
  Pot* pPot = *m_pLogicUnit->m_xpPot;
  m_pLogicUnit->m_xpPot = nullptr;
  
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
      
  m_pLogicUnit->m_xpPot = pPot;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].usesStreamDefOut -- use stream def out for this pot                  */
/*----------------------------------------------------------------------------*/
void Step::vPotUseStreamDefOut(void)
{
  ENTRY();
    
 m_pLogicUnit->m_xpStreamDefOut->vToStreamPix(&m_pLogicUnit->m_oStreamPix,true);
 m_pLogicUnit->m_xpPot->vStreamPix(&m_pLogicUnit->m_oStreamPix);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
