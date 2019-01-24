/*============================================================================*/
/* Steps_Handler.cpp - handler group                                          */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vHandlerServiceSleepQueue(MP_Task mpTask)
{
  ENTRY("vHandlerServiceSleepQueue"_Stz);
  
  Pthread::vSleepSec(1,CN_WORD);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
