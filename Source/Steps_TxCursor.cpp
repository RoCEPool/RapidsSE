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
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// txCursor: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxCursor(void)
{
  vAddPrimitive("txCursor.begin"_Str,  &vTxCursorBegin,  false);
  vAddPrimitive("txCursor.delete"_Str, &vTxCursorDelete, false);
  vAddPrimitive("txCursor.end"_Str,    &vTxCursorEnd,    false);
  vAddPrimitive("txCursor.first"_Str,  &vTxCursorFirst,  false);
  vAddPrimitive("txCursor.get"_Str,    &vTxCursorGet,    false);
  vAddPrimitive("txCursor.init"_Str,   &vTxCursorInit,   false);
  vAddPrimitive("txCursor::new"_Str,    &vTxCursorNew,    false);
  vAddPrimitive("txCursor.next"_Str,   &vTxCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* txCursor.begin -- begin using tx cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorBegin(NNP_Task nnpTask)
{
ENTRY("vTxCursorBegin"_Str);
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.begin"_Str);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.delete -- delete a tx cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTxCursorDelete"_Str);
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.delete"_Str);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  delete pTxCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.end -- end using tx cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorEnd(NNP_Task nnpTask)
{
ENTRY("vTxCursorEnd"_Str);
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txcursor.end"_Str);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.first -- set to first and push tx                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorFirst(NNP_Task nnpTask)
{
ENTRY("vTxCursorFirst"_Str);
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.first"_Str);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vFirst();
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.get -- push current tx                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorGet(NNP_Task nnpTask)
{
ENTRY("vTxCursorGet"_Str);
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.get"_Str);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.init -- init a tx cursor                                          */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorInit(NNP_Task nnpTask)
{
ENTRY("vTxCursorInit"_Str);
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.init"_Str);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  new(pTxCursor) TxCursor(gpTxCollection);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor::new -- new a tx cursor                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNew(NNP_Task nnpTask)
{
ENTRY("vTxCursorNew"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.next -- advance to next and push tx                               */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNext(NNP_Task nnpTask)
{
ENTRY("vTxCursorNext"_Str);
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.next"_Str);
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_THREADCURSOR == pTxCursor->eClass());
  
  pTxCursor->vNext();
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
