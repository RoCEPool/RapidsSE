/*============================================================================*/
/* Steps_TxCursor.cpp - tx cursor step group                                  */
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

// txCursor: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxCursor(void)
{
  vAddPrimitive("txCursor.begin"_Stz,  &vTxCursorBegin,  false);
  vAddPrimitive("txCursor.delete"_Stz, &vTxCursorDelete, false);
  vAddPrimitive("txCursor.end"_Stz,    &vTxCursorEnd,    false);
  vAddPrimitive("txCursor.first"_Stz,  &vTxCursorFirst,  false);
  vAddPrimitive("txCursor.get"_Stz,    &vTxCursorGet,    false);
  vAddPrimitive("txCursor.init"_Stz,   &vTxCursorInit,   false);
  vAddPrimitive("txCursor::new"_Stz,    &vTxCursorNew,    false);
  vAddPrimitive("txCursor.next"_Stz,   &vTxCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* txCursor.begin -- begin using tx cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorBegin(MP_Task mpTask)
{
ENTRY("vTxCursorBegin"_Stz);
  
  Object* pPtrTxCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.begin"_Stz);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.delete -- delete a tx cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorDelete(MP_Task mpTask)
{
ENTRY("vTxCursorDelete"_Stz);
  
  Object* pPtrTxCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.delete"_Stz);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  delete pTxCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.end -- end using tx cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorEnd(MP_Task mpTask)
{
ENTRY("vTxCursorEnd"_Stz);
  
  Object* pPtrTxCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txcursor.end"_Stz);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.first -- set to first and push tx                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorFirst(MP_Task mpTask)
{
ENTRY("vTxCursorFirst"_Stz);
  
  Object* pPtrTxCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.first"_Stz);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vFirst();
  
  Object* pObject = pTxCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.get -- push current tx                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorGet(MP_Task mpTask)
{
ENTRY("vTxCursorGet"_Stz);
  
  Object* pPtrTxCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.get"_Stz);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  Object* pObject = pTxCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.init -- init a tx cursor                                          */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorInit(MP_Task mpTask)
{
ENTRY("vTxCursorInit"_Stz);
  
  Object* pPtrTxCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.init"_Stz);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  new(pTxCursor) TxCursor(gpTxCollection);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor::new -- new a tx cursor                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNew(MP_Task mpTask)
{
ENTRY("vTxCursorNew"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.next -- advance to next and push tx                               */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNext(MP_Task mpTask)
{
ENTRY("vTxCursorNext"_Stz);
  
  Object* pPtrTxCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.next"_Stz);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_THREADCURSOR == pTxCursor->eClass());
  
  pTxCursor->vNext();
  
  Object* pObject = pTxCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
