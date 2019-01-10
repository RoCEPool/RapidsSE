/*============================================================================*/
/* Steps_SelCursor.cpp - select cursor step group                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SelCursor(void)
{
  vAddStep("selCursor::new"_Stz,      &Step::vSelCursorNew);
  
  vAddStep("selCursor.begin"_Stz,     &Step::vSelCursorBegin);
  vAddStep("selCursor.delete"_Stz,    &Step::vSelCursorDelete);
  vAddStep("selCursor.end"_Stz,       &Step::vSelCursorEnd);
  vAddStep("selCursor.first"_Stz,     &Step::vSelCursorFirst);
  vAddStep("selCursor.get"_Stz,       &Step::vSelCursorGet);
  vAddStep("selCursor.hasRow"_Stz,    &Step::vSelCursorHasRow);
  vAddStep("selCursor.init"_Stz,      &Step::vSelCursorInit);
  vAddStep("selCursor.next"_Stz,      &Step::vSelCursorNext);
  vAddStep("selCursor.rowBuffer"_Stz, &Step::vSelCursorRowBuffer);
  vAddStep("selCursor.table@"_Stz,    &Step::vSelCursorTableFetch);
  vAddStep("selCursor.table!"_Stz,    &Step::vSelCursorTableStore);
}

/*----------------------------------------------------------------------------*/
/* selCursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorBegin(MP_Task mpTask)
{
ENTRY("vSelCursorBegin"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.begin"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 iPartn;
  if ( ! mpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("selCursor.begin"_Stz);
  
  pSelCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorDelete(MP_Task mpTask)
{
ENTRY("vSelCursorDelete"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.delete"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pSelCursor);
  delete pSelCursor;
  
  HeapMem::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorEnd(MP_Task mpTask)
{
ENTRY("vSelCursorEnd"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.end"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.first -- set to first and get row                                */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorFirst(MP_Task mpTask)
{
ENTRY("vSelCursorFirst"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.first"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vFirst();
  
  i8 bRow = pSelCursor->pRow() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorGet(MP_Task mpTask)
{
ENTRY("vSelCursorGet"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.get"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  s48K xRow;
  
  xRow = (Stz*)pSelCursor->aRowAsStr();
  
  mpTask->m_pDataStack->vPushS48K(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.hasRow -- is there a current row                                 */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorHasRow(MP_Task mpTask)
{
  ENTRY("vSelCursorGet"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.get"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 bRow = pSelCursor->pRow() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorInit(MP_Task mpTask)
{
ENTRY("vSelCursorInit"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.init"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  new(pSelCursor) SelCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor::new -- new an row cursor                                         */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorNew(MP_Task mpTask)
{
ENTRY("vSelCursorNew"_Stz);
  
  SelCursor* pSelCursor = new SelCursor;
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSelCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorNext(MP_Task mpTask)
{
ENTRY("vSelCursorNext"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.next"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vNext();
  
  i8 bRow = pSelCursor->pRow() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.rowBuffer -- get current rowBuffer                               */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorRowBuffer(MP_Task mpTask)
{
  ENTRY("vSelCursorRowBuffer"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
  THROW_INVALID_DSTACK("selCursor.rowBuffer"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  EntryBuffer* pRowBuffer;
  pRowBuffer = pSelCursor->pRowBuffer();
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorTableFetch(MP_Task mpTask)
{
ENTRY("vSelCursorTableFetch"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.table@"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  Object* pPtrTable = pSelCursor->pTable();
  
  mpTask->m_pDataStack->vPushObject(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Step::vSelCursorTableStore(MP_Task mpTask)
{
ENTRY("vSelCursorTableStore"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.table!"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("selCursor.table!"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pSelCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
