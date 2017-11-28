/*============================================================================*/
/* Steps_ServerScheduler.cpp - server scheduler step group                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;

// Scheduler: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ServerScheduler(void)
{
  vAddPrimitive("ServerScheduler.enqueueTask"_Stz, &vServerSchedulerEnqueueTask, false);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ServerScheduler.enqueueServerTask -- enqueue server task                   */
/*----------------------------------------------------------------------------*/
void Task::vServerSchedulerEnqueueTask(MP_Task mpTask)
{
  ENTRY("vServerSchedulerEnqueueTask"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("ServerScheduler.enqueueTask"_Stz);
  ServerScheduler* pServerScheduler = _pServerScheduler(pObject);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("ServerScheduler.enqueueTask"_Stz);
  Task* pTask = _pTask(pObject);
  
  pServerScheduler->vEnqueueTask_Mutex(pTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
