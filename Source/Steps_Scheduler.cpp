/*============================================================================*/
/* Steps_Scheduler.cpp - task queue step group                               */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;

extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern PartitionScheduler* gpPartitionScheduler;
extern ServerScheduler* gpServerScheduler;

extern i8 giSchEnqueueTask;
extern i8 giSchEnqueueTaskWait;
extern i8 giSchGetTask;
extern i8 giSchGetTaskWait;

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
  
  vTrace("SchEnqueueTask %lld\n"_Stz,giSchEnqueueTask);
  vTrace("SchEnqueueTaskWait %lld\n"_Stz,giSchEnqueueTaskWait);
  vTrace("SchGetTask %lld\n"_Stz,giSchGetTask);
  vTrace("SchGetTaskWait %lld\n"_Stz,giSchGetTaskWait);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::partition -- push partition scheduler                           */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerPartition(MP_Task mpTask)
{
  ENTRY("vSchedulerPartition"_Stz);
  
  mpTask->m_pDataStack->vPushObject(gpPartitionScheduler);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::queuedTasks -- get number of queued tasks in given scheduler    */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedTasks(MP_Task mpTask)
{
  ENTRY("vSchedulerQueuedTasks"_Stz);
  
  i8 nTasks = gpServerScheduler->iQueuedTasks();
  
  mpTask->m_pDataStack->vPushInt(nTasks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::queuedThreads -- get count of threads waiting in scheduler      */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedThreads(MP_Task mpTask)
{
  ENTRY("vSchedulerQueuedThreads"_Stz);
  
  i8 nThreads = gpServerScheduler->iQueuedThreads();
  
  mpTask->m_pDataStack->vPushInt(nThreads);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* scheduler::reset -- reset scheduler statistics                             */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerReset(MP_Task mpTask)
{
  ENTRY("vSchedulerReset"_Stz);
  
  giSchEnqueueTask = 0;
  giSchEnqueueTaskWait = 0;
  giSchGetTask = 0;
  giSchGetTaskWait = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::server -- push server scheduler                                 */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerServer(MP_Task mpTask)
{
  ENTRY("vSchedulerServer"_Stz);
  
  mpTask->m_pDataStack->vPushObject(gpServerScheduler);
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
