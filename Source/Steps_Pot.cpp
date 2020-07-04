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
}

/*----------------------------------------------------------------------------*/
/* pot[].completed -- get any completed/full buffer                           */
/*----------------------------------------------------------------------------*/
void Step::vPotCompleted(void)
{
  ENTRY();
  
  Pot* pPot = *m_pRegSet->m_xpPot;
    
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
  
  Pot* pPot = *m_pRegSet->m_xpPot;
    
  DataBuffer* pDataBuffer = pPot->pCompleted();

  m_pDataStack->vPushPtr(pDataBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pot[].delete -- delete pot                                                 */
/*----------------------------------------------------------------------------*/
void Step::vPotDelete(void)
{
  ENTRY();
  
  Pot* pPot = *m_pRegSet->m_xpPot;
  m_pRegSet->m_xpPot = nullptr;
  
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
      
  m_pRegSet->m_xpPot = pPot;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
