/*============================================================================*/
/* Steps_RbCursor.cpp - row buffer cursor step group                          */
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

// rbcursor: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RbCursor(void)
{
  vAddPrimitive("rbCursor.begin"_Str,     &vRbCursorBegin,      false);
  vAddPrimitive("rbCursor.delete"_Str,    &vRbCursorDelete,     false);
  vAddPrimitive("rbCursor.end"_Str,       &vRbCursorEnd,        false);
  vAddPrimitive("rbCursor.first"_Str,     &vRbCursorFirst,      false);
  vAddPrimitive("rbCursor.get"_Str,       &vRbCursorGet,        false);
  vAddPrimitive("rbCursor.init"_Str,      &vRbCursorInit,       false);
  vAddPrimitive("rbCursor.new"_Str,       &vRbCursorNew,        false);
  vAddPrimitive("rbCursor.next"_Str,      &vRbCursorNext,       false);
  vAddPrimitive("rbCursor.rowBuffer"_Str, &vRbCursorRowBuffer,  false);
  vAddPrimitive("rbCursor.table@"_Str,    &vRbCursorTableFetch, false);
  vAddPrimitive("rbCursor.table!"_Str,    &vRbCursorTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* rbCursor.begin -- begin using row buffer cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorBegin(NNP_Task nnpTask)
{
ENTRY("vRbCursorBegin"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.begin");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("rbCursor.begin");
  
  pRbCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.delete -- delete a row buffer cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorDelete(NNP_Task nnpTask)
{
ENTRY("vRbCursorDelete"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.delete");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRbCursor);
  delete pRbCursor;
  
  //Heap::vFree(pObject);                              //! THIS SEEMS WRONG -- RAY
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.end -- end using row buffer cursor                                */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorEnd(NNP_Task nnpTask)
{
ENTRY("vRbCursorEnd"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.end");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.first -- set to first                                             */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorFirst(NNP_Task nnpTask)
{
ENTRY("vRbCursorFirst"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.first");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vFirst();
  
  i8 bRowBuffer = pRbCursor->pRowBuffer() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.get -- get current row buffer                                     */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorGet(NNP_Task nnpTask)
{
ENTRY("vRbCursorGet"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.get");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  RowBuffer* pRowBuffer = pRbCursor->pRowBuffer();
  
  nnpTask->m_pDataStack->vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.init -- init a row buffer cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorInit(NNP_Task nnpTask)
{
ENTRY("vRbCursorInit"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.init");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  new(pRbCursor) RbCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.new -- new a row buffer cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorNew(NNP_Task nnpTask)
{
ENTRY("vRbCursorNew"_Str);
  
  RbCursor* pRbCursor = new RbCursor;
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  nnpTask->m_pDataStack->vPush(pRbCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.next -- advance to next                                           */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorNext(NNP_Task nnpTask)
{
ENTRY("vRbCursorNext"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.next");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vNext();
  
  i8 bRowBuffer = pRbCursor->pRowBuffer() ? 1 : 0;
  
  nnpTask->m_pDataStack->vPush(bRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.rowBuffer -- get current rowBuffer                                */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorRowBuffer(NNP_Task nnpTask)
{
  ENTRY("vRbCursorRowBuffer"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
  THROW_INVALID_DSTACK("rbCursor.rowBuffer");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  RowBuffer* pRowBuffer;
  pRowBuffer = pRbCursor->pRowBuffer();
  
  nnpTask->m_pDataStack->vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.table@ -- fetch table                                             */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorTableFetch(NNP_Task nnpTask)
{
ENTRY("vRbCursorTableFetch"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.table@");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pPtrTable = pRbCursor->pTable();
  
  nnpTask->m_pDataStack->vPush(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.table! -- store table                                             */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorTableStore(NNP_Task nnpTask)
{
ENTRY("vRbCursorTableStore"_Str);
  
  Object* pPtrRbCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.table!");
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rbCursor.table!");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRbCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
