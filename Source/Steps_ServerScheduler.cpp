/*============================================================================*/
/* Steps_ServerScheduler.cpp - server scheduler step group                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

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
