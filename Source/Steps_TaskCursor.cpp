/*============================================================================*/
/* Steps_TaskCursor.cpp - task cursor step group                              */
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

// taskCursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskCursor(void)
{
  vAddPrimitive("taskCursor.begin",  &vTaskCursorBegin,  false);
  vAddPrimitive("taskCursor.delete", &vTaskCursorDelete, false);
  vAddPrimitive("taskCursor.end",    &vTaskCursorEnd,    false);
  vAddPrimitive("taskCursor.first",  &vTaskCursorFirst,  false);
  vAddPrimitive("taskcursor.get",    &vTaskCursorGet,    false);
  vAddPrimitive("taskcursor.init",   &vTaskCursorInit,   false);
  vAddPrimitive("taskcursor.new",    &vTaskCursorNew,    false);
  vAddPrimitive("taskcursor.next",   &vTaskCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.begin -- begin using task currsor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorBegin(NNP_Task nnpTask)
{
ENTRY("vTaskCursorBegin");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.begin");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.delete -- delete an task cursor                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTaskCursorDelete");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.delete");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pTaskCursor);
  delete pTaskCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.end -- end using task cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorEnd(NNP_Task nnpTask)
{
ENTRY("vTaskCursorEnd");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.end");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.first -- set to first and get task                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorFirst(NNP_Task nnpTask)
{
ENTRY("vTaskCursorFirst");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.first");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vFirst();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_pDataStack->vPush(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.get -- get current task of cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorGet(NNP_Task nnpTask)
{
ENTRY("vTaskCursorGet");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.get");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Task* pThatTask = pTaskCursor->pObject();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_pDataStack->vPush(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.init -- init an task cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorInit(NNP_Task nnpTask)
{
ENTRY("vTaskCursorInit");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.init");
    
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  new(pTaskCursor) TaskCursor(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.new -- new a task cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNew(NNP_Task nnpTask)
{
ENTRY("vTaskCursorNew");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.next -- advance to next and get task                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNext(NNP_Task nnpTask)
{
ENTRY("vTaskCursorNext");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.next");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vNext();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_pDataStack->vPush(pPtrTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
