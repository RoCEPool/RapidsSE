/*============================================================================*/
/* Steps_IndexPtnReg.cpp - indexPtn[] step group                              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexPtnReg(void)
{
  vAddStep("IndexPtn[].get"_Stz,  &Step::vIndexPtnRegGet);
  vAddStep("IndexPtn[].set"_Stz,  &Step::vIndexPtnRegSet);
}

/*----------------------------------------------------------------------------*/
/* IndexPtn[].get -- get index tree                                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexPtnRegGet(void)
{
  ENTRY(); // ENTRY("vIndexPtnRegGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpIndexPtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexPtn[].set -- set index tree                                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexPtnRegSet(void)
{
  ENTRY(); // ENTRY("vIndexPtnRegSet"_Stz);
  
  m_pRegSet->m_xpIndexPtn = _pIndexPtn(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
