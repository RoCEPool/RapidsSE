/*============================================================================*/
/* Steps_RbCursor.cpp - row buffer cursor step group                          */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern StepArray*      gpStepArray;
extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// rbcursor: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_RbCursor(void)
{
  vAddPrimitive("rbCursor.begin"_Stz,     &vRbCursorBegin,      false);
  vAddPrimitive("rbCursor.delete"_Stz,    &vRbCursorDelete,     false);
  vAddPrimitive("rbCursor.end"_Stz,       &vRbCursorEnd,        false);
  vAddPrimitive("rbCursor.first"_Stz,     &vRbCursorFirst,      false);
  vAddPrimitive("rbCursor.get"_Stz,       &vRbCursorGet,        false);
  vAddPrimitive("rbCursor.init"_Stz,      &vRbCursorInit,       false);
  vAddPrimitive("rbCursor::new"_Stz,       &vRbCursorNew,        false);
  vAddPrimitive("rbCursor.next"_Stz,      &vRbCursorNext,       false);
  vAddPrimitive("rbCursor.rowBuffer"_Stz, &vRbCursorRowBuffer,  false);
  vAddPrimitive("rbCursor.table@"_Stz,    &vRbCursorTableFetch, false);
  vAddPrimitive("rbCursor.table!"_Stz,    &vRbCursorTableStore, false);
}

/*----------------------------------------------------------------------------*/
/* rbCursor.begin -- begin using row buffer cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorBegin(MP_Task mpTask)
{
ENTRY("vRbCursorBegin"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.begin"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  i8 iPartn;
  if ( ! mpTask->m_pDataStack->bPop(iPartn) )
    THROW_INVALID_DSTACK("rbCursor.begin"_Stz);
  
  pRbCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rbCursor.delete -- delete a row buffer cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vRbCursorDelete(MP_Task mpTask)
{
ENTRY("vRbCursorDelete"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.delete"_Stz);
  
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
ENTRY("vRbCursorEnd"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.end"_Stz);
  
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
ENTRY("vRbCursorFirst"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.first"_Stz);
  
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
ENTRY("vRbCursorGet"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.get"_Stz);
  
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
ENTRY("vRbCursorInit"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.init"_Stz);
  
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
ENTRY("vRbCursorNew"_Stz);
  
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
ENTRY("vRbCursorNext"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.next"_Stz);
  
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
  ENTRY("vRbCursorRowBuffer"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
  THROW_INVALID_DSTACK("rbCursor.rowBuffer"_Stz);
  
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
ENTRY("vRbCursorTableFetch"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.table@"_Stz);
  
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
ENTRY("vRbCursorTableStore"_Stz);
  
  Object* pPtrRbCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRbCursor) )
    THROW_INVALID_DSTACK("rbCursor.table!"_Stz);
  
  RbCursor* pRbCursor = _pRbCursor(pPtrRbCursor);
  ASSERT(CN_RBCURSOR == pRbCursor->eClass());
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("rbCursor.table!"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRbCursor->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
