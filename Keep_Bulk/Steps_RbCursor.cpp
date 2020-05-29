/*============================================================================*/
/* Steps_RbCursor.cpp - row buffer cursor step group                          */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RbCursor(void)
{
  vAddStep("rbCursor::new"_Stz,      &Step::vRbCursorNew);
  
  vAddStep("rbCursor.begin"_Stz,     &Step::vRbCursorBegin);
  vAddStep("rbCursor.delete"_Stz,    &Step::vRbCursorDelete);
  vAddStep("rbCursor.end"_Stz,       &Step::vRbCursorEnd);
  vAddStep("rbCursor.first"_Stz,     &Step::vRbCursorFirst);
  vAddStep("rbCursor.get"_Stz,       &Step::vRbCursorGet);
  vAddStep("rbCursor.init"_Stz,      &Step::vRbCursorInit);
  vAddStep("rbCursor.next"_Stz,      &Step::vRbCursorNext);
  vAddStep("rbCursor.rowBuffer"_Stz, &Step::vRbCursorRowBuffer);
  vAddStep("rbCursor.table@"_Stz,    &Step::vRbCursorTableFetch);
  vAddStep("rbCursor.table!"_Stz,    &Step::vRbCursorTableStore);
}

/*----------------------------------------------------------------------------*/
/* rbCursor.begin -- begin using row buffer cursor                            */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorBegin(MP_Task mpTask)
{
ENTRY("vRbCursorBegin"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.begin"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  i8 iPartn;
  if ( ! mpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("rbCursor.begin"_Stz);
  
  pRbCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.delete -- delete a row buffer cursor                              */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorDelete(MP_Task mpTask)
{
ENTRY("vRbCursorDelete"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.delete"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRbCursor);
  delete pRbCursor;
  
  //HeapMem::vFree(pObject);                              //! THIS SEEMS WRONG -- RAY
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.end -- end using row buffer cursor                                */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorEnd(MP_Task mpTask)
{
ENTRY("vRbCursorEnd"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.end"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.first -- set to first                                             */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorFirst(MP_Task mpTask)
{
ENTRY("vRbCursorFirst"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.first"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vFirst();
  
  i8 bRowBuffer = pRbCursor->pRowBuffer() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(bRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.get -- get current row buffer                                     */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorGet(MP_Task mpTask)
{
ENTRY("vRbCursorGet"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.get"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  EntryBuffer* pRowBuffer = pRbCursor->pRowBuffer();
  
  mpTask->m_pDataStack->vPushPointer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.init -- init a row buffer cursor                                  */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorInit(MP_Task mpTask)
{
ENTRY("vRbCursorInit"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.init"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  new(pRbCursor) RbCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor::new -- new a row buffer cursor                                    */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorNew(MP_Task mpTask)
{
ENTRY("vRbCursorNew"_Stz);
  
  RbCursor* pRbCursor = new RbCursor;
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  mpTask->m_pDataStack->vPushPointer(pRbCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.next -- advance to next                                           */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorNext(MP_Task mpTask)
{
ENTRY("vRbCursorNext"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.next"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vNext();
  
  i8 bRowBuffer = pRbCursor->pRowBuffer() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(bRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.rowBuffer -- get current rowBuffer                                */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorRowBuffer(MP_Task mpTask)
{
  ENTRY("vRbCursorRowBuffer"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
  THROW_INVALID_DSTACK("rbCursor.rowBuffer"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  EntryBuffer* pRowBuffer;
  pRowBuffer = pRbCursor->pRowBuffer();
  
  mpTask->m_pDataStack->vPushPointer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.table@ -- fetch table                                             */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorTableFetch(MP_Task mpTask)
{
ENTRY("vRbCursorTableFetch"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.table@"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pPtrTable = pRbCursor->pTable();
  
  mpTask->m_pDataStack->vPushPointer(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.table! -- store table                                             */
/*----------------------------------------------------------------------------*/
void Step::vRbCursorTableStore(MP_Task mpTask)
{
ENTRY("vRbCursorTableStore"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.table!"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rbCursor.table!"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRbCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
