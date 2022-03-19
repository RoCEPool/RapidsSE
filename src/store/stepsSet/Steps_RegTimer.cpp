/*============================================================================*/
/* Steps_RegTimer.cpp - timer[] step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node_I.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegTimer(void)
{
  vAddStep("timer[]"_Stz,      &Step::vRegTimerGet);
  vAddStep("timer[].get"_Stz,  &Step::vRegTimerGet);
  vAddStep("timer[].set"_Stz,  &Step::vRegTimerSet);
}

/*----------------------------------------------------------------------------*/
/* timer[].get -- get timer                                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegTimerGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpTimer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer[].set -- set timer                                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegTimerSet(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpTimer = _pTimer(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
