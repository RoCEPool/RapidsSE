/*============================================================================*/
/* Steps_RowCursor.cpp - row cursor step group                                */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowCursor(void)
{
  vAddStep("rowCursor::new"_Stz,         &Step::vRowCursorNew);
  
  vAddStep("rowCursor.begin"_Stz,        &Step::vRowCursorBegin);
  vAddStep("rowCursor.delete"_Stz,       &Step::vRowCursorDelete);
  vAddStep("rowCursor.end"_Stz,          &Step::vRowCursorEnd);
  vAddStep("rowCursor.entryBuffer@"_Stz, &Step::vRowCursorEntryBufferFetch);
  vAddStep("rowCursor.entryBuffer!"_Stz, &Step::vRowCursorEntryBufferStore);
  vAddStep("rowCursor.first"_Stz,        &Step::vRowCursorFirst);
  vAddStep("rowCursor.init"_Stz,         &Step::vRowCursorInit);
  vAddStep("rowCursor.next"_Stz,         &Step::vRowCursorNext);
  vAddStep("rowCursor.row"_Stz,          &Step::vRowCursorRow);
  vAddStep("rowCursor.table@"_Stz,       &Step::vRowCursorTableFetch);
  vAddStep("rowCursor.table!"_Stz,       &Step::vRowCursorTableStore);
}

/*----------------------------------------------------------------------------*/
/* rowCursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorBegin(MP_Task mpTask)
{
ENTRY("vRowCursorBegin"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.begin"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  i8 iPartn;
  if ( ! mpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("rowCursor.begin"_Stz);
  
  pRowCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorDelete(MP_Task mpTask)
{
ENTRY("vRowCursorDelete"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.delete"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRowCursor);
  delete pRowCursor;
  
  //HeapMem::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorEnd(MP_Task mpTask)
{
ENTRY("vRowCursorEnd"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.end"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.first -- set to first and get row                                */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorFirst(MP_Task mpTask)
{
ENTRY("vRowCursorFirst"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.first"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vFirst();
  
  i8 bRow = pRowCursor->pRow() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorInit(MP_Task mpTask)
{
ENTRY("vRowCursorInit"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.init"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  new(pRowCursor) RowCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor::new -- new an row cursor                                         */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorNew(MP_Task mpTask)
{
ENTRY("vRowCursorNew"_Stz);
  
  RowCursor* pRowCursor = new RowCursor;
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  mpTask->m_pDataStack->vPushPointer(pRowCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorNext(MP_Task mpTask)
{
ENTRY("vRowCursorNext"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.next"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vNext();
  
  i8 bRow = pRowCursor->pRow() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.row -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorRow(MP_Task mpTask)
{
  ENTRY("vRowCursorRow"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.row"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  mpTask->m_pDataStack->vPushEntry(pRowCursor->pRow());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.entryBuffer@ -- fetch entryBuffer                                */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorEntryBufferFetch(MP_Task mpTask)
{
  ENTRY("vRowCursorEntryBufferFetch"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
  THROW_INVALID_DSTACK("rowCursor.entryBuffer@"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  EntryBuffer* pRowBuffer;
  pRowBuffer = pRowCursor->pRowBuffer();
  
  mpTask->m_pDataStack->vPushPointer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.entryBuffer! -- store entryBuffer                                */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorEntryBufferStore(MP_Task mpTask)
{
  ENTRY("vRowCursorEntryBufferStore"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.entryBuffer!"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowCursor.entryBuffer!"_Stz);
  
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  
  pRowCursor->vRowBuffer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorTableFetch(MP_Task mpTask)
{
ENTRY("vRowCursorTableFetch"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.table@"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable = pRowCursor->pTable();
  
  mpTask->m_pDataStack->vPushPointer(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Step::vRowCursorTableStore(MP_Task mpTask)
{
ENTRY("vRowCursorTableStore"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.table!"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rowCursor.table!"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRowCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
