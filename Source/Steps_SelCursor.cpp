/*============================================================================*/
/* Steps_SelCursor.cpp - select cursor step group                             */
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

// selCursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SelCursor(void)
{
  vAddPrimitive("selCursor.begin",     &vSelCursorBegin,      false);
  vAddPrimitive("selCursor.delete",    &vSelCursorDelete,     false);
  vAddPrimitive("selCursor.end",       &vSelCursorEnd,        false);
  vAddPrimitive("selCursor.first",     &vSelCursorFirst,      false);
  vAddPrimitive("selCursor.get",       &vSelCursorGet,        false);
  vAddPrimitive("selCursor.init",      &vSelCursorInit,       false);
  vAddPrimitive("selCursor.new",       &vSelCursorNew,        false);
  vAddPrimitive("selCursor.next",      &vSelCursorNext,       false);
  vAddPrimitive("selCursor.rowBuffer", &vSelCursorRowBuffer,  false);
  vAddPrimitive("selCursor.table@",    &vSelCursorTableFetch, false);
  vAddPrimitive("selCursor.table!",    &vSelCursorTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* selCursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorBegin(NNP_Task nnpTask)
{
ENTRY("vSelCursorBegin");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.begin");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("selCursor.begin");
  
  pSelCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorDelete(NNP_Task nnpTask)
{
ENTRY("vSelCursorDelete");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.delete");
  
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
void Task::vSelCursorEnd(NNP_Task nnpTask)
{
ENTRY("vSelCursorEnd");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.end");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.first -- set to first and get row                                */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorFirst(NNP_Task nnpTask)
{
ENTRY("vSelCursorFirst");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.first");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vFirst();
  
  i8 bRow = pSelCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorGet(NNP_Task nnpTask)
{
ENTRY("vSelCursorGet");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.get");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  s48K xRow;
  xRow = pSelCursor->pRowAsStr();
  
  nnpTask->m_pDataStack->vPush(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorInit(NNP_Task nnpTask)
{
ENTRY("vSelCursorInit");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.init");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  new(pSelCursor) SelCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.new -- new an row cursor                                         */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorNew(NNP_Task nnpTask)
{
ENTRY("vSelCursorNew");
  
  SelCursor* pSelCursor = new SelCursor;
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  nnpTask->m_pDataStack->vPush(pSelCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorNext(NNP_Task nnpTask)
{
ENTRY("vSelCursorNext");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.next");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vNext();
  
  i8 bRow = pSelCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.rowBuffer -- get current rowBuffer                               */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorRowBuffer(NNP_Task nnpTask)
{
  ENTRY("vSelCursorRowBuffer");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
  THROW_INVALID_DSTACK("selCursor.rowBuffer");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  RowBuffer* pRowBuffer;
  pRowBuffer = pSelCursor->pRowBuffer();
  
  nnpTask->m_pDataStack->vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorTableFetch(NNP_Task nnpTask)
{
ENTRY("vSelCursorTableFetch");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.table@");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  Object* pPtrTable = pSelCursor->pTable();
  
  nnpTask->m_pDataStack->vPush(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorTableStore(NNP_Task nnpTask)
{
ENTRY("vSelCursorTableStore");
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.table!");
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("selCursor.table!");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pSelCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
