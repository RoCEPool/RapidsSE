/*============================================================================*/
/* Steps_TxRef.cpp - tx ref step group                                        */
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
  vAddPrimitive("txRef.init"_Str,      &vTxRefInit,        false);
  vAddPrimitive("txRef.begin"_Str,     &vTxRefBegin,       false);
  vAddPrimitive("txRef.end"_Str,       &vTxRefEnd,         false);
  vAddPrimitive("txRef.commit"_Str,    &vTxRefCommit,      false);
  vAddPrimitive("txRef.rollback"_Str,  &vTxRefRollback,    false);
}

/*----------------------------------------------------------------------------*/
/* txRef.init -- init of txref on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vTxRefInit(NNP_Task nnpTask)
{
ENTRY("vTxRefInit"_Str);
  
  Object* pPtrTxRef;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.init"_Str);
  
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
ENTRY("vTxRefBegin"_Str);
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("tx.begin"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bBegun = pTxRef->bBeginTx();
  
  nnpTask->m_pDataStack->vPush(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.end -- end a tx (via a txRef on stack)                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRefEnd(NNP_Task nnpTask)
{
ENTRY("vTxRefEnd"_Str);
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.end"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bEnded = pTxRef->bEndTx();
  
  nnpTask->m_pDataStack->vPush(bEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.commit -- commit a tx (via a txRef on stack)                         */
/*----------------------------------------------------------------------------*/
void Task::vTxRefCommit(NNP_Task nnpTask)
{
ENTRY("vTxRefCommit"_Str);
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.commit"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bCommitted = pTxRef->bCommitTx();
  
  nnpTask->m_pDataStack->vPush(bCommitted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.rollback -- rollback a tx (via a txRef on stack)                     */
/*----------------------------------------------------------------------------*/
void Task::vTxRefRollback(NNP_Task nnpTask)
{
ENTRY("vTxRefRollback"_Str);
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.rollback"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bRolledBack = pTxRef->bRollbackTx();
  
  nnpTask->m_pDataStack->vPush(bRolledBack);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
