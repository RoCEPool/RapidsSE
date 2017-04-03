/*============================================================================*/
/* Steps_SelCursor.cpp - select cursor step group                             */
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

// selCursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_SelCursor(void)
{
  vAddPrimitive("selCursor.begin"_Str,     &vSelCursorBegin,      false);
  vAddPrimitive("selCursor.delete"_Str,    &vSelCursorDelete,     false);
  vAddPrimitive("selCursor.end"_Str,       &vSelCursorEnd,        false);
  vAddPrimitive("selCursor.first"_Str,     &vSelCursorFirst,      false);
  vAddPrimitive("selCursor.get"_Str,       &vSelCursorGet,        false);
  vAddPrimitive("selCursor.init"_Str,      &vSelCursorInit,       false);
  vAddPrimitive("selCursor.new"_Str,       &vSelCursorNew,        false);
  vAddPrimitive("selCursor.next"_Str,      &vSelCursorNext,       false);
  vAddPrimitive("selCursor.rowBuffer"_Str, &vSelCursorRowBuffer,  false);
  vAddPrimitive("selCursor.table@"_Str,    &vSelCursorTableFetch, false);
  vAddPrimitive("selCursor.table!"_Str,    &vSelCursorTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* selCursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorBegin(NNP_Task nnpTask)
{
ENTRY("vSelCursorBegin"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.begin"_Str);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("selCursor.begin"_Str);
  
  pSelCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorDelete(NNP_Task nnpTask)
{
ENTRY("vSelCursorDelete"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.delete"_Str);
  
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
ENTRY("vSelCursorEnd"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.end"_Str);
  
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
ENTRY("vSelCursorFirst"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.first"_Str);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vFirst();
  
  i8 bRow = pSelCursor->aRowAsStr() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorGet(NNP_Task nnpTask)
{
ENTRY("vSelCursorGet"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.get"_Str);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  s48K xRow;
  xRow = (Str*)pSelCursor->aRowAsStr();
  
  nnpTask->m_pDataStack->vPush(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorInit(NNP_Task nnpTask)
{
ENTRY("vSelCursorInit"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.init"_Str);
  
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
ENTRY("vSelCursorNew"_Str);
  
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
ENTRY("vSelCursorNext"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.next"_Str);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vNext();
  
  i8 bRow = pSelCursor->aRowAsStr() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* selCursor.rowBuffer -- get current rowBuffer                               */
/*----------------------------------------------------------------------------*/
void Task::vSelCursorRowBuffer(NNP_Task nnpTask)
{
  ENTRY("vSelCursorRowBuffer"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
  THROW_INVALID_DSTACK("selCursor.rowBuffer"_Str);
  
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
ENTRY("vSelCursorTableFetch"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.table@"_Str);
  
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
ENTRY("vSelCursorTableStore"_Str);
  
  Object* pPtrSelCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrSelCursor) )
    THROW_INVALID_DSTACK("selCursor.table!"_Str);
  
  SelCursor* pSelCursor = _pSelCursor(pPtrSelCursor);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("selCursor.table!"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pSelCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
