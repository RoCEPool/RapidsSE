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
  vAddStep("counter1[]"_Stz,      &Step::vRegCounter1Get);
  vAddStep("counter1[].get"_Stz,  &Step::vRegCounter1Get);
  vAddStep("counter1[].set"_Stz,  &Step::vRegCounter1Set);
  vAddStep("counter2[]"_Stz,      &Step::vRegCounter2Get);
  vAddStep("counter2[].get"_Stz,  &Step::vRegCounter2Get);
  vAddStep("counter2[].set"_Stz,  &Step::vRegCounter2Set);
}

/*----------------------------------------------------------------------------*/
/* counter0[].get -- get counter0                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegCounter0Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter0[].set -- set counter0                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegCounter0Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter0 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter1[].get -- get counter1                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegCounter1Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter1[].set -- set counter1                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegCounter1Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter1 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter2[].get -- get counter1                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegCounter2Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter2[].set -- set counter1                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegCounter2Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter2 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
