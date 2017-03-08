/*============================================================================*/
/* Steps_RowCursor.cpp - row cursor step group                                */
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

// rowCursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RowCursor(void)
{
  vAddPrimitive("rowCursor.begin"_Str,      &vRowCursorBegin,           false);
  vAddPrimitive("rowCursor.delete"_Str,     &vRowCursorDelete,          false);
  vAddPrimitive("rowCursor.end"_Str,        &vRowCursorEnd,             false);
  vAddPrimitive("rowCursor.first"_Str,      &vRowCursorFirst,           false);
  vAddPrimitive("rowCursor.init"_Str,       &vRowCursorInit,            false);
  vAddPrimitive("rowCursor.new"_Str,        &vRowCursorNew,             false);
  vAddPrimitive("rowCursor.next"_Str,       &vRowCursorNext,            false);
  vAddPrimitive("rowCursor.row"_Str,        &vRowCursorRow,             false);
  vAddPrimitive("rowCursor.rowBuffer@"_Str, &vRowCursorRowBufferFetch,  false);
  vAddPrimitive("rowCursor.rowBuffer!"_Str, &vRowCursorRowBufferStore,  false);
  vAddPrimitive("rowCursor.table@"_Str,     &vRowCursorTableFetch,      false);
  vAddPrimitive("rowCursor.table!"_Str,     &vRowCursorTableStore,      false);
}

/*----------------------------------------------------------------------------*/
/* rowCursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorBegin(NNP_Task nnpTask)
{
ENTRY("vRowCursorBegin");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.begin");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("rowCursor.begin");
  
  pRowCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorDelete(NNP_Task nnpTask)
{
ENTRY("vRowCursorDelete");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.delete");
  
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
void Task::vRowCursorEnd(NNP_Task nnpTask)
{
ENTRY("vRowCursorEnd");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.end");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.first -- set to first and get row                                */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorFirst(NNP_Task nnpTask)
{
ENTRY("vRowCursorFirst");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.first");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vFirst();
  
  i8 bRow = pRowCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorInit(NNP_Task nnpTask)
{
ENTRY("vRowCursorInit");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.init");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  new(pRowCursor) RowCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.new -- new an row cursor                                         */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNew(NNP_Task nnpTask)
{
ENTRY("vRowCursorNew");
  
  RowCursor* pRowCursor = new RowCursor;
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  nnpTask->m_pDataStack->vPush(pRowCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNext(NNP_Task nnpTask)
{
ENTRY("vRowCursorNext");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.next");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vNext();
  
  i8 bRow = pRowCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRow(NNP_Task nnpTask)
{
  ENTRY("vRowCursorRow");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.row");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  s48K xRow;
  xRow = (Str*)pRowCursor->pRowAsStr();
  
  nnpTask->m_pDataStack->vPush(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.rowBuffer@ -- fetch rowBuffer                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRowBufferFetch(NNP_Task nnpTask)
{
  ENTRY("vRowCursorRowBufferFetch");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
  THROW_INVALID_DSTACK("rowCursor.rowBuffer@");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  RowBuffer* pRowBuffer;
  pRowBuffer = pRowCursor->pRowBuffer();
  
  nnpTask->m_pDataStack->vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.rowBuffer! -- store rowBuffer                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRowBufferStore(NNP_Task nnpTask)
{
  ENTRY("vRowCursorRowBufferStore");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.rowBuffer!");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowCursor.rowBuffer!");
  
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  pRowCursor->vRowBuffer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableFetch(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableFetch");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.table@");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable = pRowCursor->pTable();
  
  nnpTask->m_pDataStack->vPush(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowCursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableStore(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableStore");
  
  Object* pPtrRpwCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRpwCursor) )
    THROW_INVALID_DSTACK("rowCursor.table!");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRpwCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rowCursor.table!");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRowCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
