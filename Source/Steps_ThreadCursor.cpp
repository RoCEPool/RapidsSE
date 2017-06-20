/*============================================================================*/
/* Steps_ThreadCursor.cpp - thread cursor step group                          */
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
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// threadCursor: ///////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ThreadCursor(void)
{
  vAddPrimitive("threadCursor.begin"_Str,  &vThreadCursorBegin,  false);
  vAddPrimitive("threadCursor.delete"_Str, &vThreadCursorDelete, false);
  vAddPrimitive("threadCursor.end"_Str,    &vThreadCursorEnd,    false);
  vAddPrimitive("threadCursor.first"_Str,  &vThreadCursorFirst,  false);
  vAddPrimitive("threadCursor.get"_Str,    &vThreadCursorGet,    false);
  vAddPrimitive("threadCursor.init"_Str,   &vThreadCursorInit,   false);
  vAddPrimitive("threadCursor::new"_Str,    &vThreadCursorNew,    false);
  vAddPrimitive("threadCursor.next"_Str,   &vThreadCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* threadCursor.begin -- begin using thread cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorBegin(MP_Task mpTask)
{
ENTRY("vThreadCursorBegin"_Str);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.begin"_Str);
  
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
ENTRY("vTheadCursorDelete"_Str);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.delete"_Str);
  
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
ENTRY("vThreadCursorEnd"_Str);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadcursor.end"_Str);
  
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
ENTRY("vThreadCursorFirst"_Str);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.first"_Str);
  
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
ENTRY("vThreadCursorGet"_Str);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.get"_Str);
  
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
ENTRY("vThreadCursorInit"_Str);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.init"_Str);
  
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
ENTRY("vThreadCursorNew"_Str);
  
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
ENTRY("vThreadCursorNext"_Str);
  
  Object* pPtrThreadCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.next"_Str);
  
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
