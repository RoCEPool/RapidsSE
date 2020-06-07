/*============================================================================*/
/* Steps_ToHshCrw.cpp - Convert ptn nst rsb buffers to hashed crw buffers   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToHshCrw(void)
{
  vAddStep("toHshCrw[].completed"_Stz, &Step::vToHshCrwRegCompleted);
  vAddStep("toHshCrw[].convert"_Stz,   &Step::vToHshCrwRegConvert);
  vAddStep("toHshCrw[].create"_Stz,    &Step::vToHshCrwRegCreate);
  vAddStep("toHshCrw[].dangling"_Stz,  &Step::vToHshCrwRegDangling);
  vAddStep("toHshCrw[].delete"_Stz,    &Step::vToHshCrwRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toHshCrw[].completed -- get any completed/full buffer                      */
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
/* toHshCrw[].convert -- convert rsb buffer                                   */
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
/* toHshCrw[].create -- create toHshCrw                                       */
/*----------------------------------------------------------------------------*/
void Step::vToHshCrwRegCreate(void)
{
  ENTRY();
    
  ToHshCrw* pToHshCrw = new ToHshCrw;
      
  m_pRegSet->m_xpToHshCrw = pToHshCrw;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toHshCrw[].danglingRsbBuffer -- get any dangling/last buffer               */
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
/* toHshCrw[].delete -- delete toHshCrw                                       */
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
