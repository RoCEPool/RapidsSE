/*============================================================================*/
/* Steps_ToNstRsb.cpp - Convert rsb to normalised string row slab buffers      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToNstRsb(void)
{
  vAddStep("toNstRsb[].convert"_Stz,     &Step::vToNstRsbRegConvert);
  vAddStep("toNstRsb[].create"_Stz,      &Step::vToNstRsbRegCreate);
  vAddStep("toNstRsb[].delete"_Stz,      &Step::vToNstRsbRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toNstRsb[].convert -- convert rsb buffer                                   */
/*----------------------------------------------------------------------------*/
void Step::vToNstRsbRegConvert(void)
{
  ENTRY();

  ToNstRsb* pToNstRsb = *m_pLogicUnit->m_xpToNstRsb;
  
  RowBuffer* pRsbBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  pToNstRsb->vConvert(pRsbBuffer);
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toNstRsb[].create -- create toNstRsb                                       */
/*----------------------------------------------------------------------------*/
void Step::vToNstRsbRegCreate(void)
{
  ENTRY();
    
  ToNstRsb* pToNstRsb = new ToNstRsb;
      
  m_pLogicUnit->m_xpToNstRsb = pToNstRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toNstRsb[].delete -- delete toNstRsb                                       */
/*----------------------------------------------------------------------------*/
void Step::vToNstRsbRegDelete(void)
{
  ENTRY();
  
  ToNstRsb* pToNstRsb = *m_pLogicUnit->m_xpToNstRsb;
  
  m_pLogicUnit->m_xpToNstRsb = nullptr;
  
  delete pToNstRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
