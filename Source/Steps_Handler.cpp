/*============================================================================*/
/* Steps_Handler.cpp - handler group                                          */
/*============================================================================*/

#include "Steps.h"

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
void Step::vHandlerServiceSleepQueue(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vHandlerServiceSleepQueue"_Stz);
  
  pForth->m_pTask->m_pThread->m_pPthread->vSleepSsec(1,CN_WORD);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
