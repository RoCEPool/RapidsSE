/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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

// txCursor: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxCursor(void)
{
  vAddPrimitive("txCursor.begin",  &vTxCursorBegin,  false);
  vAddPrimitive("txCursor.delete", &vTxCursorDelete, false);
  vAddPrimitive("txCursor.end",    &vTxCursorEnd,    false);
  vAddPrimitive("txCursor.first",  &vTxCursorFirst,  false);
  vAddPrimitive("txCursor.get",    &vTxCursorGet,    false);
  vAddPrimitive("txCursor.init",   &vTxCursorInit,   false);
  vAddPrimitive("txCursor.new",    &vTxCursorNew,    false);
  vAddPrimitive("txCursor.next",   &vTxCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* txCursor.begin -- begin using tx cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorBegin(NNP_Task nnpTask)
{
ENTRY("vTxCursorBegin");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.begin");
  
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
ENTRY("vTxCursorDelete");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.delete");
  
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
ENTRY("vTxCursorEnd");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txcursor.end");
  
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
ENTRY("vTxCursorFirst");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.first");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vFirst();
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.get -- push current tx                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorGet(NNP_Task nnpTask)
{
ENTRY("vTxCursorGet");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.get");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.init -- init a tx cursor                                          */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorInit(NNP_Task nnpTask)
{
ENTRY("vTxCursorInit");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.init");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  new(pTxCursor) TxCursor(gpTxCollection);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.new -- new a tx cursor                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNew(NNP_Task nnpTask)
{
ENTRY("vTxCursorNew");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.next -- advance to next and push tx                               */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNext(NNP_Task nnpTask)
{
ENTRY("vTxCursorNext");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxCursor) )
    THROW_INVALID_DSTACK("txCursor.next");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_THREADCURSOR == pTxCursor->eClass());
  
  pTxCursor->vNext();
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
