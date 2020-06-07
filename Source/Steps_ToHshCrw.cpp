/*============================================================================*/
/* Steps_ToHshCrw.cpp - Convert ptn nst rsb buffers to hashed crw buffers   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToHshCrw(void)
{
  vAddStep("toPtnRsb[].completed"_Stz, &Step::vToHshCrwRegCompleted);
  vAddStep("toPtnRsb[].convert"_Stz,   &Step::vToHshCrwRegConvert);
  vAddStep("toPtnRsb[].create"_Stz,    &Step::vToHshCrwRegCreate);
  vAddStep("toPtnRsb[].dangling"_Stz,  &Step::vToHshCrwRegDangling);
  vAddStep("toPtnRsb[].delete"_Stz,    &Step::vToHshCrwRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toRsb[].completed -- get any completed/full buffer                         */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegCompleted(void)
{
  ENTRY();
  
  ToHshCrw* pToHshCrw = *m_pRegSet->m_xpToHshCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RsbBuffer* pRsbBuffer = pToHshCrw->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRsb[].convert -- convert rsb buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegConvert(void)
{
  ENTRY();

  ToHshCrw* pToHshCrw = *m_pRegSet->m_xpToHshCrw;
    
  RsbBuffer* pRsbBuffer = _pRsbBuffer(m_pDataStack->pPopPtr(__func__));

  pToHshCrw->vConvert(pRsbBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRsb[].create -- create toPtnRsb                                       */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegCreate(void)
{
  ENTRY();
    
  ToHshCrw* pToHshCrw = new ToHshCrw;
      
  m_pRegSet->m_xpToHshCrw = pToHshCrw;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toRsb[].danglingRsbBuffer -- get any dangling/last buffer                  */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegDangling(void)
{
  ENTRY();
  
  ToHshCrw* pToHshCrw = *m_pRegSet->m_xpToHshCrw;
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);
  
  RsbBuffer* pRsbBuffer = pToHshCrw->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toPtnRsb[].delete -- delete toPtnRsb                                       */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegDelete(void)
{
  ENTRY();
  
  ToHshCrw* pToHshCrw = *m_pRegSet->m_xpToHshCrw;
  m_pRegSet->m_xpToHshCrw = nullptr;
  
  delete pToHshCrw;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
