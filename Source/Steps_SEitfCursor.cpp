/*============================================================================*/
/* Steps_SEitfCursor.cpp - SEitf cursor step group                            */
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

// seitfcursor: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitfCursor(void)
{
  vAddPrimitive("seitfCursor.begin"_Str,  &vSEitfCursorBegin,  false);
  vAddPrimitive("seitfCursor.delete"_Str, &vSEitfCursorDelete, false);
  vAddPrimitive("seitfCursor.end"_Str,    &vSEitfCursorEnd,    false);
  vAddPrimitive("seitfCursor.first"_Str,  &vSEitfCursorFirst,  false);
  vAddPrimitive("seitfCursor.get"_Str,    &vSEitfCursorGet,    false);
  vAddPrimitive("seitfCursor.init"_Str,   &vSEitfCursorInit,   false);
  vAddPrimitive("seitfCursor::new"_Str,    &vSEitfCursorNew,    false);
  vAddPrimitive("seitfCursor.next"_Str,   &vSEitfCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.begin -- begin using seitf currsor                             */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorBegin(MP_Task mpTask)
{
ENTRY("vSEitfCursorBegin"_Str);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.begin"_Str);
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());

  pSEitfCursor->vBegin();
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.delete -- delete an seitf cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorDelete(MP_Task mpTask)
{
ENTRY("vSEitfCursorDelete"_Str);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.delete"_Str);
  
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
void Task::vSEitfCursorEnd(MP_Task mpTask)
{
ENTRY("vSEitfCursorEnd"_Str);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.end"_Str);
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.first -- set to first and get seitf                            */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorFirst(MP_Task mpTask)
{
ENTRY("vSEitfCursorFirst"_Str);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.first"_Str);
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.get -- get current seitf                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorGet(MP_Task mpTask)
{
ENTRY("vSEitfCursorGet"_Str);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.get"_Str);
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = pSEitfCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.init -- init an seitf cursor                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorInit(MP_Task mpTask)
{
ENTRY("vSEitfCursorInit"_Str);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.init"_Str);
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  s32 sName;
  sName = "seitflist"_Str;
  Word* pWord = mpTask->m_xPathStack.pFindWord(sName);
  ASSERT(pWord);
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(pWord->aBodyArea());
  
  
  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor::new -- new an seitf cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNew(MP_Task mpTask)
{
ENTRY("vSEitfCursorNew"_Str);
  
  Object* pPtrSEitfList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitfcursor::new"_Str);
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(Heap::pAllocate(CN_SEITFCURSOR));

  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  Object* pPtrSEitfCursor = pSEitfCursor;
  
  mpTask->m_pDataStack->vPushObject(pPtrSEitfCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.next -- advance to next and get seitf                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNext(MP_Task mpTask)
{
ENTRY("vSEitfCursorNext"_Str);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.next"_Str);
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vNext();
  
  Object* pObject = pSEitfCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
