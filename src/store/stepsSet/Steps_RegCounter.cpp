/*============================================================================*/
/* Steps_RegCounter.cpp - counter[] step group                                */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegCounter(void)
{
  vAddStep("counterA[]"_Stz,      &Step::vCounterAGet);
  vAddStep("counterA[].get"_Stz,  &Step::vCounterAGet);
  vAddStep("counterA[].set"_Stz,  &Step::vCounterASet);

  vAddStep("counter0[]"_Stz,      &Step::vCounter0Get);
  vAddStep("counter0[].get"_Stz,  &Step::vCounter0Get);
  vAddStep("counter0[].set"_Stz,  &Step::vCounter0Set);

  vAddStep("counter1[]"_Stz,      &Step::vCounter1Get);
  vAddStep("counter1[].get"_Stz,  &Step::vCounter1Get);
  vAddStep("counter1[].set"_Stz,  &Step::vCounter1Set);

  vAddStep("counter2[]"_Stz,      &Step::vCounter2Get);
  vAddStep("counter2[].get"_Stz,  &Step::vCounter2Get);
  vAddStep("counter2[].set"_Stz,  &Step::vCounter2Set);

  vAddStep("counter3[]"_Stz,      &Step::vCounter3Get);
  vAddStep("counter3[].get"_Stz,  &Step::vCounter3Get);
  vAddStep("counter3[].set"_Stz,  &Step::vCounter3Set);

  vAddStep("counter4[]"_Stz,      &Step::vCounter4Get);
  vAddStep("counter4[].get"_Stz,  &Step::vCounter4Get);
  vAddStep("counter4[].set"_Stz,  &Step::vCounter4Set);

  vAddStep("counter5[]"_Stz,      &Step::vCounter5Get);
  vAddStep("counter5[].get"_Stz,  &Step::vCounter5Get);
  vAddStep("counter5[].set"_Stz,  &Step::vCounter5Set);

  vAddStep("counter6[]"_Stz,      &Step::vCounter6Get);
  vAddStep("counter6[].get"_Stz,  &Step::vCounter6Get);
  vAddStep("counter6[].set"_Stz,  &Step::vCounter6Set);

  vAddStep("counter7[]"_Stz,      &Step::vCounter7Get);
  vAddStep("counter7[].get"_Stz,  &Step::vCounter7Get);
  vAddStep("counter7[].set"_Stz,  &Step::vCounter7Set);
}

/*----------------------------------------------------------------------------*/
/* CounterA[].get -- get CounterA                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounterAGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounterA);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* CounterA[].set -- set CounterA                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounterASet(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounterA = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter0[].get -- get counter0                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter0Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter0[].set -- set counter0                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter0Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter0 = m_pDataStack->iPopIntBol(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter1[].get -- get counter1                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter1Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter1[].set -- set counter1                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter1Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter1 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter2[].get -- get counter1                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter2Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* counter2[].set -- set counter1                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter2Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter2 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter3[].get -- get Counter3                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter3Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter3);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter3[].set -- set Counter3                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter3Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter3 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter4[].get -- get Counter4                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter4Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter4);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter4[].set -- set Counter4                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter4Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter4 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter5[].get -- get Counter5                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter5Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter5);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter5[].set -- set Counter5                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter5Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter5 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter6[].get -- get Counter6                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter6Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter6);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter6[].set -- set Counter6                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter6Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter6 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter7[].get -- get Counter7                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter7Get(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_pLogicUnit->m_iCounter7);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Counter7[].set -- set Counter7                                             */
/*----------------------------------------------------------------------------*/
void Step::vCounter7Set(void)
{
  ENTRY();
    
  m_pLogicUnit->m_iCounter7 = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
