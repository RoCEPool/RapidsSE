/*============================================================================*/
/* Steps_RbCursor.cpp - row buffer cursor step group                          */
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
  vAddPrimitive("rbCursor::new"_Str,       &vRbCursorNew,        false);
  vAddPrimitive("rbCursor.next"_Str,      &vRbCursorNext,       false);
  vAddPrimitive("rbCursor.rowBuffer"_Str, &vRbCursorRowBuffer,  false);
  vAddPrimitive("rbCursor.table@"_Str,    &vRbCursorTableFetch, false);
  vAddPrimitive("rbCursor.table!"_Str,    &vRbCursorTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* rbCursor.begin -- begin using row buffer cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorBegin(MP_Task mpTask)
{
ENTRY("vRbCursorBegin"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.begin"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  i8 iPartn;
  if ( ! mpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("rbCursor.begin"_Str);
  
  pRbCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.delete -- delete a row buffer cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorDelete(MP_Task mpTask)
{
ENTRY("vRbCursorDelete"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.delete"_Str);
  
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
void Task::vRbCursorEnd(MP_Task mpTask)
{
ENTRY("vRbCursorEnd"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.end"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.first -- set to first                                             */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorFirst(MP_Task mpTask)
{
ENTRY("vRbCursorFirst"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.first"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vFirst();
  
  i8 bRowBuffer = pRbCursor->pRowBuffer() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(bRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.get -- get current row buffer                                     */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorGet(MP_Task mpTask)
{
ENTRY("vRbCursorGet"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.get"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  EntryBuffer* pRowBuffer = pRbCursor->pRowBuffer();
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.init -- init a row buffer cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorInit(MP_Task mpTask)
{
ENTRY("vRbCursorInit"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.init"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  new(pRbCursor) RbCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor::new -- new a row buffer cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorNew(MP_Task mpTask)
{
ENTRY("vRbCursorNew"_Str);
  
  RbCursor* pRbCursor = new RbCursor;
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  mpTask->m_pDataStack->vPushObject(pRbCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.next -- advance to next                                           */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorNext(MP_Task mpTask)
{
ENTRY("vRbCursorNext"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.next"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  pRbCursor->vNext();
  
  i8 bRowBuffer = pRbCursor->pRowBuffer() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushInt(bRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.rowBuffer -- get current rowBuffer                                */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorRowBuffer(MP_Task mpTask)
{
  ENTRY("vRbCursorRowBuffer"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
  THROW_INVALID_DSTACK("rbCursor.rowBuffer"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  EntryBuffer* pRowBuffer;
  pRowBuffer = pRbCursor->pRowBuffer();
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.table@ -- fetch table                                             */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorTableFetch(MP_Task mpTask)
{
ENTRY("vRbCursorTableFetch"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.table@"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pPtrTable = pRbCursor->pTable();
  
  mpTask->m_pDataStack->vPushObject(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.table! -- store table                                             */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorTableStore(MP_Task mpTask)
{
ENTRY("vRbCursorTableStore"_Str);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.table!"_Str);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rbCursor.table!"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRbCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
