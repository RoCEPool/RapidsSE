/*============================================================================*/
/* Steps_Handler.cpp - handler group                                          */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// error: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Handler(void)
{
  vAddStep("handler::serviceSleepQueue"_Stz,  &Task::vHandlerServiceSleepQueue);
}

/*----------------------------------------------------------------------------*/
/* error.msg -- get error message given the error code                        */
/*----------------------------------------------------------------------------*/
void Task::vHandlerServiceSleepQueue(MP_Task mpTask)
{
  ENTRY("vHandlerServiceSleepQueue"_Stz);
  
  PosixThread::vSleepSec(1);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
