/*============================================================================*/
/* Steps_TaskCursor.cpp - task cursor step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
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

// taskCursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskCursor(void)
{
  vAddPrimitive("taskCursor.begin"_Stz,  &vTaskCursorBegin,  false);
  vAddPrimitive("taskCursor.delete"_Stz, &vTaskCursorDelete, false);
  vAddPrimitive("taskCursor.end"_Stz,    &vTaskCursorEnd,    false);
  vAddPrimitive("taskCursor.first"_Stz,  &vTaskCursorFirst,  false);
  vAddPrimitive("taskcursor.get"_Stz,    &vTaskCursorGet,    false);
  vAddPrimitive("taskcursor.init"_Stz,   &vTaskCursorInit,   false);
  vAddPrimitive("taskcursor::new"_Stz,    &vTaskCursorNew,    false);
  vAddPrimitive("taskcursor.next"_Stz,   &vTaskCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.begin -- begin using task currsor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorBegin(MP_Task mpTask)
{
ENTRY("vTaskCursorBegin"_Stz);
  
  Object* pPtrTaskCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.begin"_Stz);
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.delete -- delete an task cursor                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorDelete(MP_Task mpTask)
{
ENTRY("vTaskCursorDelete"_Stz);
  
  Object* pPtrTaskCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.delete"_Stz);
  
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
void Task::vTaskCursorEnd(MP_Task mpTask)
{
ENTRY("vTaskCursorEnd"_Stz);
  
  Object* pPtrTaskCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.end"_Stz);
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.first -- set to first and get task                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorFirst(MP_Task mpTask)
{
ENTRY("vTaskCursorFirst"_Stz);
  
  Object* pPtrTaskCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.first"_Stz);
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vFirst();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.get -- get current task of cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorGet(MP_Task mpTask)
{
ENTRY("vTaskCursorGet"_Stz);
  
  Object* pPtrTaskCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.get"_Stz);
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Task* pThatTask = pTaskCursor->pObject();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.init -- init an task cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorInit(MP_Task mpTask)
{
ENTRY("vTaskCursorInit"_Stz);
  
  Object* pPtrTaskCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.init"_Stz);
    
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  new(pTaskCursor) TaskCursor(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor::new -- new a task cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNew(MP_Task mpTask)
{
ENTRY("vTaskCursorNew"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.next -- advance to next and get task                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNext(MP_Task mpTask)
{
ENTRY("vTaskCursorNext"_Stz);
  
  Object* pPtrTaskCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskCursor) )
    THROW_INVALID_DSTACK("taskCursor.next"_Stz);
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vNext();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pPtrTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
