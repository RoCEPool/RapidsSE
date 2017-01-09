/*============================================================================*/
/* Task_SEitfCursor.cpp - task SEitf cursor group                             */
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

// seitfcursor: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitfCursor(void)
{
  vAddPrimitive("seitfCursor.begin",  &vSEitfCursorBegin,  false);
  vAddPrimitive("seitfCursor.delete", &vSEitfCursorDelete, false);
  vAddPrimitive("seitfCursor.end",    &vSEitfCursorEnd,    false);
  vAddPrimitive("seitfCursor.first",  &vSEitfCursorFirst,  false);
  vAddPrimitive("seitfCursor.get",    &vSEitfCursorGet,    false);
  vAddPrimitive("seitfCursor.init",   &vSEitfCursorInit,   false);
  vAddPrimitive("seitfCursor.new",    &vSEitfCursorNew,    false);
  vAddPrimitive("seitfCursor.next",   &vSEitfCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.begin -- begin using seitf currsor                             */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorBegin(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorBegin");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.begin");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());

  pSEitfCursor->vBegin();
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_pDataStack->vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.delete -- delete an seitf cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorDelete(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorDelete");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.delete");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pSEitfCursor);
  delete pSEitfCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.end -- end using seitf cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorEnd(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorEnd");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.end");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.first -- set to first and get seitf                            */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorFirst(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorFirst");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.first");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_pDataStack->vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.get -- get current seitf                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorGet(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorGet");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.get");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_pDataStack->vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.init -- init an seitf cursor                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorInit(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorInit");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.init");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  s32 sName;
  sName = "seitflist";
  Word* pWord = nnpTask->pFindWord(sName);
  ASSERT(pWord);
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(pWord->pDataArea());
  
  
  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.new -- new an seitf cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNew(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorNew");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitfcursor.new");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(Heap::pAllocate(CN_SEITFCURSOR));

  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  Object* pPtrSEitfCursor = pSEitfCursor;
  
  nnpTask->m_pDataStack->vPush(pPtrSEitfCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.next -- advance to next and get seitf                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNext(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorNext");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.next");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vNext();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_pDataStack->vPush(pObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
