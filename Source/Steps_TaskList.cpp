/*============================================================================*/
/* Steps_TaskList.cpp - task list step groups                                 */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
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

extern NP_Task         gnpRootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// tasklist: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskList(void)
{
  vAddPrimitive("taskList"_Str,        &vTaskList,         false);
  vAddPrimitive("taskList.append"_Str, &vTaskListAppend,   false);
  vAddPrimitive("taskList.dump"_Str,   &vTaskListDump,     false);
  vAddPrimitive("taskList.init"_Str,   &vTaskListInit,     false);
}

/*----------------------------------------------------------------------------*/
/* tasklist -- tasklist                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskList(MP_Task mpTask)
{
ENTRY("vTaskList"_Str);
  
  mpTask->m_pDataStack->vPushObject(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.append -- tasklist append                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskListAppend(MP_Task mpTask)
{
ENTRY("vTaskListAppend"_Str);
  
  Object* pPtrTaskList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskList) )
    THROW_INVALID_DSTACK("tasklist.append"_Str);
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskList->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.dump -- tasklist dump                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListDump(MP_Task mpTask)
{
ENTRY("vTaskListDump"_Str);
  
  Object* pPtrTaskList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskList) )
    THROW_INVALID_DSTACK("tasklist.dump"_Str);
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
 
  pTaskList->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.init -- tasklist init                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListInit(MP_Task mpTask)
{
ENTRY("vTaskListInit"_Str);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
