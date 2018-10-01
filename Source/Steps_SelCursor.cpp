/*============================================================================*/
/* Steps_SelCursor.cpp - select cursor step group                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// selCursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_SelCursor(void)
{
  vAddStep("selCursor.begin"_Stz,     &Task::vSelCursorBegin,      false);
  vAddStep("selCursor.delete"_Stz,    &Task::vSelCursorDelete,     false);
  vAddStep("selCursor.end"_Stz,       &Task::vSelCursorEnd,        false);
  vAddStep("selCursor.first"_Stz,     &Task::vSelCursorFirst,      false);
  vAddStep("selCursor.get"_Stz,       &Task::vSelCursorGet,        false);
  vAddStep("selCursor.hasRow"_Stz,    &Task::vSelCursorHasRow,     false);
  vAddStep("selCursor.init"_Stz,      &Task::vSelCursorInit,       false);
  vAddStep("selCursor::new"_Stz,      &Task::vSelCursorNew,        false);
  vAddStep("selCursor.next"_Stz,      &Task::vSelCursorNext,       false);
  vAddStep("selCursor.rowBuffer"_Stz, &Task::vSelCursorRowBuffer,  false);
  vAddStep("selCursor.table@"_Stz,    &Task::vSelCursorTableFetch, false);
  vAddStep("selCursor.table!"_Stz,    &Task::vSelCursorTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* selCursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorBegin(MP_Task mpTask)
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
void Task::vSelCursorDelete(MP_Task mpTask)
{
ENTRY("vSelCursorDelete"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.delete"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pSelCursor);
  delete pSelCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorEnd(MP_Task mpTask)
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
void Task::vSelCursorFirst(MP_Task mpTask)
{
ENTRY("vSelCursorFirst"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.first"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vFirst();
  
  i8 bRow = pSelCursor->pRow() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorGet(MP_Task mpTask)
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
void Task::vSelCursorHasRow(MP_Task mpTask)
{
  ENTRY("vSelCursorGet"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.get"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 bRow = pSelCursor->pRow() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorInit(MP_Task mpTask)
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
void Task::vSelCursorNew(MP_Task mpTask)
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
void Task::vSelCursorNext(MP_Task mpTask)
{
ENTRY("vSelCursorNext"_Stz);
  
  Object* pPtrSelCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.next"_Stz);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vNext();
  
  i8 bRow = pSelCursor->pRow() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.rowBuffer -- get current rowBuffer                               */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorRowBuffer(MP_Task mpTask)
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
void Task::vSelCursorTableFetch(MP_Task mpTask)
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
void Task::vSelCursorTableStore(MP_Task mpTask)
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
