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
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegAggTimer(void)
{
  vAddStep("aggTimer[]"_Stz,      &Step::vRegAggTimerGet);
  vAddStep("aggTimer[].get"_Stz,  &Step::vRegAggTimerGet);
  vAddStep("aggTimer[].set"_Stz,  &Step::vRegAggTimerSet);
}

/*----------------------------------------------------------------------------*/
/* aggTimer[].get -- get timer                                                */
/*----------------------------------------------------------------------------*/
void Step::vRegAggTimerGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpAggTimer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* aggTimer[].set -- set timer                                                */
/*----------------------------------------------------------------------------*/
void Step::vRegAggTimerSet(void)
{
  ENTRY();
    
  ( m_pJob->pLogicUnit_job() )->m_xpAggTimer = _pAggTimer(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
