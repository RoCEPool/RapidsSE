/*============================================================================*/
/* Steps_ThreadQueue.cpp - thread list step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;

// ThreadQueue: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ThreadQueue(void)
{
  vAddPrimitive("ThreadQueue.init"_Stz,    &vThreadQueueInit,        false);
}

/*----------------------------------------------------------------------------*/
/* ThreadQueue.init -- init of ThreadQueue on data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadQueueInit(MP_Task mpTask)
{
ENTRY("vThreadQueueInit"_Stz);
  
  Object* pPtrThreadQueue;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadQueue) )
    THROW_INVALID_DSTACK("ThreadQueue.init"_Stz);
  
  ThreadQueue* pThreadQueue = _pThreadQueue(pPtrThreadQueue);
  
  new(pThreadQueue) ThreadQueue;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
