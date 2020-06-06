/*============================================================================*/
/* Steps_ToPtnRsb.cpp - Convert rsb buffers to partitioned row slab buffers   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToPtnRsb(void)
{
  vAddStep("toPtnRsb[].convert"_Stz,   &Step::vToPtnRsbRegConvert);
  vAddStep("toPtnRsb[].create"_Stz,    &Step::vToPtnRsbRegCreate);
  vAddStep("toPtnRsb[].delete"_Stz,    &Step::vToPtnRsbRegDelete);
  
  vAddStep("toPtnRsb[].dangling"_Stz,  &Step::vToPtnRsbRegDangling);
  vAddStep("toPtnRsb[].full"_Stz,      &Step::vToPtnRsbRegFull);
}

/*----------------------------------------------------------------------------*/
/* toPtnRsb[].convert -- convert rsb buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRsbRegConvert(void)
{
  ENTRY();

  ToNstRsb* pToNstRsb = *m_pRegSet->m_xpToNstRsb;
    
  //bool bResult = pToRsb->bConvert(azSrc);
  
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


/*----------------------------------------------------------------------------*/
/* toRsb[].danglingRsbBuffer -- fetch rsb buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRsbRegDangling(void)
{
  ENTRY();
  
  ToPtnRsb* pToPtnRsb = *m_pRegSet->m_xpToPtnRsb;
  
  RsbBuffer* pRsbBuffer = nullptr; // pToPtnRsb->pDangling();
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].full -- full rsb buffer                                            */
/*----------------------------------------------------------------------------*/
void Step::vToPtnRsbRegFull(void)
{
  ENTRY();
  
  ToPtnRsb* pToPtnRsb = *m_pRegSet->m_xpToPtnRsb;
  
  RsbBuffer* pRsbBuffer = nullptr; // pToPtnRsb->pFull();

  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
