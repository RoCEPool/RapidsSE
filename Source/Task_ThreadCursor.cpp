/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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

// threadCursor: ///////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_ThreadCursor(void)
{
  vAddPrimitive("threadCursor.begin",  &vThreadCursorBegin,  false);
  vAddPrimitive("threadCursor.delete", &vThreadCursorDelete, false);
  vAddPrimitive("threadCursor.end",    &vThreadCursorEnd,    false);
  vAddPrimitive("threadCursor.first",  &vThreadCursorFirst,  false);
  vAddPrimitive("threadCursor.get",    &vThreadCursorGet,    false);
  vAddPrimitive("threadCursor.init",   &vThreadCursorInit,   false);
  vAddPrimitive("threadCursor.new",    &vThreadCursorNew,    false);
  vAddPrimitive("threadCursor.next",   &vThreadCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* threadCursor.begin -- begin using thread cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorBegin(NNP_Task nnpTask)
{
ENTRY("vThreadCursorBegin");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.begin");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.delete -- delete a thread cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTheadCursorDelete");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.delete");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  delete pThreadCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.end -- end using thread cursor                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorEnd(NNP_Task nnpTask)
{
ENTRY("vThreadCursorEnd");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadcursor.end");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.first -- set to first and push thread                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorFirst(NNP_Task nnpTask)
{
ENTRY("vThreadCursorFirst");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.first");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vFirst();
  
  Object* pThread = pThreadCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.get -- push current thread                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorGet(NNP_Task nnpTask)
{
ENTRY("vThreadCursorGet");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.get");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  Object* pObject = pThreadCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.init -- init a thread cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorInit(NNP_Task nnpTask)
{
ENTRY("vThreadCursorInit");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.init");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  new(pThreadCursor) ThreadCursor(gpThreadList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.new -- new a thread cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNew(NNP_Task nnpTask)
{
ENTRY("vThreadCursorNew");
  
  ThreadCursor* pThreadCursor = new ThreadCursor(gpThreadList);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  nnpTask->m_xDataStack.vPush(pThreadCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.next -- advance to next and push thread                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNext(NNP_Task nnpTask)
{
ENTRY("vThreadCursorNext");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_DSTACK("threadCursor.next");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vNext();
  
  Object* pThread = pThreadCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pThread);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
