/*============================================================================*/
/* Task_RowCursor.cpp - task row cursor group                                 */
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

// rowcursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RowCursor(void)
{
  vAddPrimitive("rowCursor.begin",  &vRowCursorBegin,  false);
  vAddPrimitive("rowCursor.delete", &vRowCursorDelete, false);
  vAddPrimitive("rowCursor.end",    &vRowCursorEnd,    false);
  vAddPrimitive("rowCursor.first",  &vRowCursorFirst,  false);
  vAddPrimitive("rowCursor.get",    &vRowCursorGet,    false);
  vAddPrimitive("rowCursor.init",   &vRowCursorInit,   false);
  vAddPrimitive("rowCursor.new",    &vRowCursorNew,    false);
  vAddPrimitive("rowCursor.next",   &vRowCursorNext,   false);
  vAddPrimitive("rowCursor.rowBuffer", &vRowCursorRowBuffer,  false);
  vAddPrimitive("rowCursor.table@", &vRowCursorTableFetch, false);
  vAddPrimitive("rowCursor.table!", &vRowCursorTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorBegin(NNP_Task nnpTask)
{
ENTRY("vRowCursorBegin");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.begin");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("rowcursor.begin");
  
  pRowCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorDelete(NNP_Task nnpTask)
{
ENTRY("vRowCursorDelete");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.delete");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRowCursor);
  delete pRowCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorEnd(NNP_Task nnpTask)
{
ENTRY("vRowCursorEnd");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.end");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.first -- set to first and get row                                */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorFirst(NNP_Task nnpTask)
{
ENTRY("vRowCursorFirst");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.first");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vFirst();
  
  i8 bRow = pRowCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorGet(NNP_Task nnpTask)
{
ENTRY("vRowCursorGet");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.get");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  s8K xRow;
  xRow = pRowCursor->pRowAsStr();
  
  nnpTask->m_pDataStack->vPush(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorInit(NNP_Task nnpTask)
{
ENTRY("vRowCursorInit");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.init");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  new(pRowCursor) RowCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.new -- new an row cursor                                         */
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
/* rowcursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNext(NNP_Task nnpTask)
{
ENTRY("vRowCursorNext");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.next");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vNext();
  
  i8 bRow = pRowCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.rowBuffer -- get current rowBuffer                               */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRowBuffer(NNP_Task nnpTask)
{
  ENTRY("vRowCursorRowBuffer");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
  THROW_INVALID_DSTACK("rowcursor.rowBuffer");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  RowBuffer* pRowBuffer;
  pRowBuffer = pRowCursor->pRowBuffer();
  
  nnpTask->m_pDataStack->vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableFetch(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableFetch");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.table@");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable = pRowCursor->pTable();
  
  nnpTask->m_pDataStack->vPush(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableStore(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableStore");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowCursor) )
    THROW_INVALID_DSTACK("rowcursor.table!");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rowcursor.table!");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRowCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
