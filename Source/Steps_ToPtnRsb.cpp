/*============================================================================*/
/* Steps_ToPtnRsb.cpp - Convert rsb buffers to partitioned row slab buffers   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToPtnRsb(void)
{
  vAddStep("toPtnRsb[].completed"_Stz, &Step::vToPtnRsbRegCompleted);
  vAddStep("toPtnRsb[].convert"_Stz,   &Step::vToPtnRsbRegConvert);
  vAddStep("toPtnRsb[].create"_Stz,    &Step::vToPtnRsbRegCreate);
  vAddStep("toPtnRsb[].dangling"_Stz,  &Step::vToPtnRsbRegDangling);
  vAddStep("toPtnRsb[].delete"_Stz,    &Step::vToPtnRsbRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toRsb[].completed -- get any completed/full buffer                         */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRsbRegCompleted(void)
{
  ENTRY();
  
  ToPtnRsb* pToPtnRsb = *m_pRegSet->m_xpToPtnRsb;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RsbBuffer* pRsbBuffer = pToPtnRsb->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRsb[].convert -- convert rsb buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRsbRegConvert(void)
{
  ENTRY();

  ToPtnRsb* pToPtnRsb = *m_pRegSet->m_xpToPtnRsb;
    
  RsbBuffer* pRsbBuffer = _pRsbBuffer(m_pDataStack->pPopPtr(__func__));

  pToPtnRsb->vConvert(pRsbBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRsb[].create -- create toPtnRsb                                       */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRsbRegCreate(void)
{
  ENTRY();
    
  ToPtnRsb* pToPtnRsb = new ToPtnRsb;
      
  m_pRegSet->m_xpToPtnRsb = pToPtnRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].danglingRsbBuffer -- get any dangling/last buffer                  */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRsbRegDangling(void)
{
  ENTRY();
  
  ToPtnRsb* pToPtnRsb = *m_pRegSet->m_xpToPtnRsb;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RsbBuffer* pRsbBuffer = pToPtnRsb->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRsb[].delete -- delete toPtnRsb                                       */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRsbRegDelete(void)
{
  ENTRY();
  
  ToPtnRsb* pToPtnRsb = *m_pRegSet->m_xpToPtnRsb;
  m_pRegSet->m_xpToPtnRsb = nullptr;
  
  delete pToPtnRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
