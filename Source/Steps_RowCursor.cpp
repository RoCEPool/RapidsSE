/*============================================================================*/
/* Steps_RowCursor.cpp - row cursor step group                                */
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
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// rowCursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RowCursor(void)
{
  vAddPrimitive("rowCursor.begin"_Stz,      &vRowCursorBegin,           false);
  vAddPrimitive("rowCursor.delete"_Stz,     &vRowCursorDelete,          false);
  vAddPrimitive("rowCursor.end"_Stz,        &vRowCursorEnd,             false);
  vAddPrimitive("rowCursor.first"_Stz,      &vRowCursorFirst,           false);
  vAddPrimitive("rowCursor.init"_Stz,       &vRowCursorInit,            false);
  vAddPrimitive("rowCursor::new"_Stz,        &vRowCursorNew,             false);
  vAddPrimitive("rowCursor.next"_Stz,       &vRowCursorNext,            false);
  vAddPrimitive("rowCursor.row"_Stz,        &vRowCursorRow,             false);
  vAddPrimitive("rowCursor.rowBuffer@"_Stz, &vRowCursorRowBufferFetch,  false);
  vAddPrimitive("rowCursor.rowBuffer!"_Stz, &vRowCursorRowBufferStore,  false);
  vAddPrimitive("rowCursor.table@"_Stz,     &vRowCursorTableFetch,      false);
  vAddPrimitive("rowCursor.table!"_Stz,     &vRowCursorTableStore,      false);
}

/*----------------------------------------------------------------------------*/
/* rowCursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorBegin(MP_Task mpTask)
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
void Task::vRowCursorDelete(MP_Task mpTask)
{
ENTRY("vRowCursorDelete"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.delete"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRowCursor);
  delete pRowCursor;
  
  //Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorEnd(MP_Task mpTask)
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
void Task::vRowCursorFirst(MP_Task mpTask)
{
ENTRY("vRowCursorFirst"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.first"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vFirst();
  
  i8 bRow = pRowCursor->aRowAsStr() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorInit(MP_Task mpTask)
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
void Task::vRowCursorNew(MP_Task mpTask)
{
ENTRY("vRowCursorNew"_Stz);
  
  RowCursor* pRowCursor = new RowCursor;
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  mpTask->m_pDataStack->vPushObject(pRowCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNext(MP_Task mpTask)
{
ENTRY("vRowCursorNext"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.next"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vNext();
  
  i8 bRow = pRowCursor->aRowAsStr() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRow(MP_Task mpTask)
{
  ENTRY("vRowCursorRow"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.row"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  s48K xRow;
  xRow = (Stz*)pRowCursor->aRowAsStr();
  
  mpTask->m_pDataStack->vPushS48K(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.rowBuffer@ -- fetch rowBuffer                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRowBufferFetch(MP_Task mpTask)
{
  ENTRY("vRowCursorRowBufferFetch"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
  THROW_INVALID_DSTACK("rowCursor.rowBuffer@"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  EntryBuffer* pRowBuffer;
  pRowBuffer = pRowCursor->pRowBuffer();
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.rowBuffer! -- store rowBuffer                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRowBufferStore(MP_Task mpTask)
{
  ENTRY("vRowCursorRowBufferStore"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.rowBuffer!"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowCursor.rowBuffer!"_Stz);
  
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  //ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  pRowCursor->vRowBuffer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableFetch(MP_Task mpTask)
{
ENTRY("vRowCursorTableFetch"_Stz);
  
  Object* pPtrRpwCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.table@"_Stz);
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable = pRowCursor->pTable();
  
  mpTask->m_pDataStack->vPushObject(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableStore(MP_Task mpTask)
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
