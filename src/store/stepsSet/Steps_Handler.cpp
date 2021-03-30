/*============================================================================*/
/* Steps_Handler.cpp - handler group                                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


#include "PsxThd.hpp"  //?

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Handler(void)
{
  vAddStep("handler::serviceSleepQueue"_Stz,  &Step::vHandlerServiceSleepQueue);
}

/*----------------------------------------------------------------------------*/
/* error.msg -- get error message given the error code                        */
/*----------------------------------------------------------------------------*/
void Step::vHandlerServiceSleepQueue(void)
{
  ENTRY();
  
  m_pThread->pPsxThd()->vSleepSsec(1,CN_WORD);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
