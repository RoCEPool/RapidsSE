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

// txRef: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxRef(void)
{
  vAddPrimitive("txRef.init",      &vTxRefInit,        false);
  vAddPrimitive("txRef.begin",     &vTxRefBegin,       false);
  vAddPrimitive("txRef.end",       &vTxRefEnd,         false);
  vAddPrimitive("txRef.commit",    &vTxRefCommit,      false);
  vAddPrimitive("txRef.rollback",  &vTxRefRollback,    false);
}

/*----------------------------------------------------------------------------*/
/* txRef.init -- init of txref on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxRefInit(NNP_Task nnpTask)
{
ENTRY("vTxRefInit");
  
  Object* pPtrTxRef;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.init");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  new(pTxRef) TxRef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.begin -- begin a tx (via a txRef on stack)                           */
/*----------------------------------------------------------------------------*/
void Task::vTxRefBegin(NNP_Task nnpTask)
{
ENTRY("vTxRefBegin");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("tx.begin");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bBegun = pTxRef->bBeginTx();
  
  nnpTask->m_xDataStack.vPush(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.end -- end a tx (via a txRef on stack)                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRefEnd(NNP_Task nnpTask)
{
ENTRY("vTxRefEnd");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.end");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bEnded = pTxRef->bEndTx();
  
  nnpTask->m_xDataStack.vPush(bEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.commit -- commit a tx (via a txRef on stack)                         */
/*----------------------------------------------------------------------------*/
void Task::vTxRefCommit(NNP_Task nnpTask)
{
ENTRY("vTxRefCommit");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.commit");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bCommitted = pTxRef->bCommitTx();
  
  nnpTask->m_xDataStack.vPush(bCommitted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.rollback -- rollback a tx (via a txRef on stack)                     */
/*----------------------------------------------------------------------------*/
void Task::vTxRefRollback(NNP_Task nnpTask)
{
ENTRY("vTxRefRollback");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.rollback");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bRolledBack = pTxRef->bRollbackTx();
  
  nnpTask->m_xDataStack.vPush(bRolledBack);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
