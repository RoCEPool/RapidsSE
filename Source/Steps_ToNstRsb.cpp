/*============================================================================*/
/* Steps_ToNstRsb.cpp - Convert rsb to normalised string row slab buffers      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ToNstRsb(void)
{
  vAddStep("toNstRsb[].create"_Stz,      &Step::vToNstRsbRegCreate);
  vAddStep("toNstRsb[].delete"_Stz,      &Step::vToNstRsbRegDelete);
}

/*----------------------------------------------------------------------------*/
/* toNstRsb[].create -- create toNstRsb                                       */
/*----------------------------------------------------------------------------*/
void Step::vToNstRsbRegCreate(void)
{
  ENTRY();
    
  ToNstRsb* pToNstRsb = new ToNstRsb;
      
  m_pRegSet->m_xpToNstRsb = pToNstRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* toNstRsb[].delete -- delete toNstRsb                                       */
/*----------------------------------------------------------------------------*/
void Step::vToNstRsbRegDelete(void)
{
  ENTRY();
  
  ToNstRsb* pToNstRsb = *m_pRegSet->m_xpToNstRsb;
  m_pRegSet->m_xpToNstRsb = nullptr;
  
  delete pToNstRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
