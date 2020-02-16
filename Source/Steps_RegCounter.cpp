/*============================================================================*/
/* Steps_RegCounter.cpp - counter[] step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegCounter(void)
{
  vAddStep("counter0[]"_Stz,      &Step::vRegCounter0Get);
  vAddStep("counter0[].get"_Stz,  &Step::vRegCounter0Get);
  vAddStep("counter0[].set"_Stz,  &Step::vRegCounter0Set);
}

/*----------------------------------------------------------------------------*/
/* counter0[].get -- get counter0                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegCounter0Get(void)
{
  ENTRY(); // ENTRY("vRegCounter0Get"_Stz);
    
  m_pDataStack->vPushInt(m_pRegSet->m_iCounter0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter0[].set -- set counter0                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegCounter0Set(void)
{
  ENTRY(); // ENTRY("vRegCounter0Set"_Stz);
    
  m_pRegSet->m_iCounter0 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
