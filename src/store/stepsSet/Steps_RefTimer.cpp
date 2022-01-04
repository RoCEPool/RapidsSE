/*============================================================================*/
/* Steps_RefTimer.cpp - refTimer step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RefTimer(void)
{
  vAddStep("refTimer::millseconds"_Stz,  &Step::vRefTimerMilliseconds);
  vAddStep("refTimer::reset"_Stz,        &Step::vRefTimerReset);
}

/*----------------------------------------------------------------------------*/
/* refTimer::millseconds -- get milliseconds elapsed                          */
/*----------------------------------------------------------------------------*/
void Step::vRefTimerMilliseconds(void)
{
  ENTRY();
    
  Timer* pReferenceTimer = AM::TIMER::pReferenceTimer();
  
  pReferenceTimer->vUpdate();

  i8 nMilliseconds = pReferenceTimer->iMilliseconds();
  
  m_pDataStack->vPushInt(nMilliseconds);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* refTimer::reset -- reset reference timer                                   */
/*----------------------------------------------------------------------------*/
void Step::vRefTimerReset(void)
{
  ENTRY();
    
  Timer* pReferenceTimer = AM::TIMER::pReferenceTimer();
  
  pReferenceTimer->vStart();

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
