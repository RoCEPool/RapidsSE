/*============================================================================*/
/* Steps_RegTimer.cpp - timer[] step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"

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
    
  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpTimer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer[].set -- set timer                                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegTimerSet(void)
{
  ENTRY();
    
  ( m_pJob->pLogicUnit_job() )->m_xpTimer = _pTimer(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
