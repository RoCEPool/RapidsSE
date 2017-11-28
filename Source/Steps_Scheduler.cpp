/*============================================================================*/
/* Steps_Scheduler.cpp - task queue step group                               */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// Scheduler: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Scheduler(void)
{
  vAddPrimitive("Scheduler::list"_Stz,              &vSchedulerList,          false);
  vAddPrimitive("Scheduler::partition"_Stz,         &vSchedulerPartition,     false);
  vAddPrimitive("Scheduler::queuedTasks"_Stz,       &vSchedulerQueuedTasks,   false);
  vAddPrimitive("Scheduler::queuedThreads"_Stz,     &vSchedulerQueuedThreads, false);
  vAddPrimitive("Scheduler::reset"_Stz,             &vSchedulerReset,         false);
  vAddPrimitive("Scheduler::server"_Stz,            &vSchedulerServer,        false);
}

/*----------------------------------------------------------------------------*/
/* Scheduler::list -- trace scheduler statistics                              */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerList(MP_Task mpTask)
{
  ENTRY("vSchedulerList"_Stz);
  
  vTrace("SchEnqueueTask %lld\n"_Stz,gpAttachMap->m_iSchEnqueueTask);
  vTrace("SchEnqueueTaskWait %lld\n"_Stz,gpAttachMap->m_iSchEnqueueTaskWait);
  vTrace("SchGetTask %lld\n"_Stz,gpAttachMap->m_iSchGetTask);
  vTrace("SchGetTaskWait %lld\n"_Stz,gpAttachMap->m_iSchGetTaskWait);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::partition -- push partition scheduler                           */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerPartition(MP_Task mpTask)
{
  ENTRY("vSchedulerPartition"_Stz);
  
  mpTask->m_pDataStack->vPushObject(gpAttachMap->m_pPartitionScheduler);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::queuedTasks -- get number of queued tasks in given scheduler    */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedTasks(MP_Task mpTask)
{
  ENTRY("vSchedulerQueuedTasks"_Stz);
  
  i8 nTasks = gpAttachMap->m_pServerScheduler->iQueuedTasks();
  
  mpTask->m_pDataStack->vPushInt(nTasks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::queuedThreads -- get count of threads waiting in scheduler      */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedThreads(MP_Task mpTask)
{
  ENTRY("vSchedulerQueuedThreads"_Stz);
  
  i8 nThreads = gpAttachMap->m_pServerScheduler->iQueuedThreads();
  
  mpTask->m_pDataStack->vPushInt(nThreads);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* scheduler::reset -- reset scheduler statistics                             */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerReset(MP_Task mpTask)
{
  ENTRY("vSchedulerReset"_Stz);
  
  gpAttachMap->m_iSchEnqueueTask = 0;
  gpAttachMap->m_iSchEnqueueTaskWait = 0;
  gpAttachMap->m_iSchGetTask = 0;
  gpAttachMap->m_iSchGetTaskWait = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::server -- push server scheduler                                 */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerServer(MP_Task mpTask)
{
  ENTRY("vSchedulerServer"_Stz);
  
  mpTask->m_pDataStack->vPushObject(gpAttachMap->m_pServerScheduler);
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
