/*============================================================================*/
/* Task_TaskSet.cpp - task task set group                                     */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// taskset: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskSet(void)
{
  vAddPrimitive("taskSet.append",  &vTaskSetAppend,    false);
  vAddPrimitive("taskSet.dump",    &vTaskSetDump,      false);
  vAddPrimitive("taskSet.init",    &vTaskSetInit,      false);
}

/*----------------------------------------------------------------------------*/
/* taskset.append -- taskset append                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetAppend(NNP_Task nnpTask)
{
ENTRY("vTaskSetAppend");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.append");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("taskset.append");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset.dump -- taskset dump                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetDump(NNP_Task nnpTask)
{
ENTRY("vTaskSetDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.dump");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  pTaskSet->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset.init -- taskset init                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetInit(NNP_Task nnpTask)
{
ENTRY("vTaskSetInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xDataStack.bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.init");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  new(pTaskSet) TaskSet;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
