/*============================================================================*/
/* Steps_ThreadQueue.cpp - thread list step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

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
