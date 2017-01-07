/*============================================================================*/
/* Task_TaskList.cpp - task task list groups                                  */
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

// tasklist: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskList(void)
{
  vAddPrimitive("taskList",        &vTaskList,         false);
  vAddPrimitive("taskList.append", &vTaskListAppend,   false);
  vAddPrimitive("taskList.dump",   &vTaskListDump,     false);
  vAddPrimitive("taskList.init",   &vTaskListInit,     false);
}

/*----------------------------------------------------------------------------*/
/* tasklist -- tasklist                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskList(NNP_Task nnpTask)
{
ENTRY("vTaskList");
  
  nnpTask->m_xDataStack.vPush(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.append -- tasklist append                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskListAppend(NNP_Task nnpTask)
{
ENTRY("vTaskListAppend");
  
  Object* pPtrTaskList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskList) )
    THROW_INVALID_DSTACK("tasklist.append");
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xDataStack.bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskList->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.dump -- tasklist dump                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListDump(NNP_Task nnpTask)
{
ENTRY("vTaskListDump");
  
  Object* pPtrTaskList;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTaskList) )
    THROW_INVALID_DSTACK("tasklist.dump");
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
 
  pTaskList->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.init -- tasklist init                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListInit(NNP_Task nnpTask)
{
ENTRY("vTaskListInit");
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
