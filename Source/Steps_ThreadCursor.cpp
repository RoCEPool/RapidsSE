/*============================================================================*/
/* Steps_ThreadCursor.cpp - thread cursor step group                          */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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

// threadCursor: ///////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ThreadCursor(void)
{
  vAddPrimitive("threadCursor.begin"_Stz,  &vThreadCursorBegin,  false);
  vAddPrimitive("threadCursor.delete"_Stz, &vThreadCursorDelete, false);
  vAddPrimitive("threadCursor.end"_Stz,    &vThreadCursorEnd,    false);
  vAddPrimitive("threadCursor.first"_Stz,  &vThreadCursorFirst,  false);
  vAddPrimitive("threadCursor.get"_Stz,    &vThreadCursorGet,    false);
  vAddPrimitive("threadCursor.init"_Stz,   &vThreadCursorInit,   false);
  vAddPrimitive("threadCursor::new"_Stz,    &vThreadCursorNew,    false);
  vAddPrimitive("threadCursor.next"_Stz,   &vThreadCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* threadCursor.begin -- begin using thread cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorBegin(MP_Task mpTask)
{
ENTRY("vThreadCursorBegin"_Stz);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.begin"_Stz);
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.delete -- delete a thread cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorDelete(MP_Task mpTask)
{
ENTRY("vTheadCursorDelete"_Stz);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.delete"_Stz);
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  delete pThreadCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.end -- end using thread cursor                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorEnd(MP_Task mpTask)
{
ENTRY("vThreadCursorEnd"_Stz);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadcursor.end"_Stz);
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.first -- set to first and push thread                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorFirst(MP_Task mpTask)
{
ENTRY("vThreadCursorFirst"_Stz);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.first"_Stz);
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vFirst();
  
  Object* pThread = pThreadCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.get -- push current thread                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorGet(MP_Task mpTask)
{
ENTRY("vThreadCursorGet"_Stz);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.get"_Stz);
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  Object* pObject = pThreadCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.init -- init a thread cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorInit(MP_Task mpTask)
{
ENTRY("vThreadCursorInit"_Stz);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.init"_Stz);
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  new(pThreadCursor) ThreadCursor(gpThreadList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor::new -- new a thread cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNew(MP_Task mpTask)
{
ENTRY("vThreadCursorNew"_Stz);
  
  ThreadCursor* pThreadCursor = new ThreadCursor(gpThreadList);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  mpTask->m_pDataStack->vPushObject(pThreadCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.next -- advance to next and push thread                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNext(MP_Task mpTask)
{
ENTRY("vThreadCursorNext"_Stz);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.next"_Stz);
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vNext();
  
  Object* pThread = pThreadCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pThread);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
