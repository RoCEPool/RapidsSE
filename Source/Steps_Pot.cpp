/*============================================================================*/
/* Steps_Pot.cpp - Working pot of buffers                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Pot(void)
{
  vAddStep("pot[].completed"_Stz, &Step::vPotCompleted);
  vAddStep("pot[].dangling"_Stz,  &Step::vPotDangling);
  vAddStep("pot[].delete"_Stz,    &Step::vPotDelete);
  vAddStep("pot[].new"_Stz,       &Step::vPotNew);
  vAddStep("pot[].newDstRsb"_Stz, &Step::vPotNewDstRsb);
  vAddStep("pot[].setStreamPix"_Stz, &Step::vPotSetStreamPix);
}

/*----------------------------------------------------------------------------*/
/* pot[].completed -- get any completed/full buffer                           */
/*----------------------------------------------------------------------------*/
void Step::vPotCompleted(void)
{
  ENTRY();
  
  Pot* pPot = *m_pLogicUnit->m_xpPot;
    
  DataBuffer* pDataBuffer = pPot->pCompleted();

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
    
  DataBuffer* pDataBuffer = pPot->pDangling();

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
/* pot[].newDstRsb -- create new rsb dst row                                  */
/*----------------------------------------------------------------------------*/
void Step::vPotNewDstRsb(void)
{
  ENTRY();
  
  Pot* pPot = _pPot(* m_pLogicUnit->m_xpPot);
  
  m_pLogicUnit->m_aDstRsb = pPot->aNewDstRsb(&m_pLogicUnit->m_pDstRowSlab);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].setStreamPix -- set the streamPix in the pot                         */
/*----------------------------------------------------------------------------*/
void Step::vPotSetStreamPix(void)
{
  ENTRY();
  
  Pot* pPot = _pPot(* m_pLogicUnit->m_xpPot);
  
  pPot->vStreamPix(&m_pLogicUnit->m_oStreamPix);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
