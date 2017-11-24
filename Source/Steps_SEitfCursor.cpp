/*============================================================================*/
/* Steps_SEitfCursor.cpp - SEitf cursor step group                            */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern StepArray*      gpStepArray;
extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// seitfcursor: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SEitfCursor(void)
{
  vAddPrimitive("seitfCursor.begin"_Stz,  &vSEitfCursorBegin,  false);
  vAddPrimitive("seitfCursor.delete"_Stz, &vSEitfCursorDelete, false);
  vAddPrimitive("seitfCursor.end"_Stz,    &vSEitfCursorEnd,    false);
  vAddPrimitive("seitfCursor.first"_Stz,  &vSEitfCursorFirst,  false);
  vAddPrimitive("seitfCursor.get"_Stz,    &vSEitfCursorGet,    false);
  vAddPrimitive("seitfCursor.init"_Stz,   &vSEitfCursorInit,   false);
  vAddPrimitive("seitfCursor::new"_Stz,    &vSEitfCursorNew,    false);
  vAddPrimitive("seitfCursor.next"_Stz,   &vSEitfCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.begin -- begin using seitf currsor                             */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorBegin(MP_Task mpTask)
{
ENTRY("vSEitfCursorBegin"_Stz);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.begin"_Stz);
  
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
ENTRY("vSEitfCursorDelete"_Stz);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.delete"_Stz);
  
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
ENTRY("vSEitfCursorEnd"_Stz);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.end"_Stz);
  
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
ENTRY("vSEitfCursorFirst"_Stz);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.first"_Stz);
  
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
ENTRY("vSEitfCursorGet"_Stz);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.get"_Stz);
  
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
ENTRY("vSEitfCursorInit"_Stz);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.init"_Stz);
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  s32 sName;
  sName = "seitflist"_Stz;
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
ENTRY("vSEitfCursorNew"_Stz);
  
  Object* pPtrSEitfList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfList) )
    THROW_INVALID_DSTACK("seitfcursor::new"_Stz);
  
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
ENTRY("vSEitfCursorNext"_Stz);
  
  Object* pPtrSEitfCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitfCursor) )
    THROW_INVALID_DSTACK("seitfcursor.next"_Stz);
  
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
