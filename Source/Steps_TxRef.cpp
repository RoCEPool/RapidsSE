/*============================================================================*/
/* Steps_TxRef.cpp - tx ref step group                                        */
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
void Task::vTxRefInit(MP_Task mpTask)
{
ENTRY("vTxRefInit"_Str);
  
  Object* pPtrTxRef;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.init"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  new(pTxRef) TxRef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.begin -- begin a tx (via a txRef on stack)                           */
/*----------------------------------------------------------------------------*/
void Task::vTxRefBegin(MP_Task mpTask)
{
ENTRY("vTxRefBegin"_Str);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("tx.begin"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bBegun = pTxRef->bBeginTx();
  
  mpTask->m_pDataStack->vPushInt(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.end -- end a tx (via a txRef on stack)                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRefEnd(MP_Task mpTask)
{
ENTRY("vTxRefEnd"_Str);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.end"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bEnded = pTxRef->bEndTx();
  
  mpTask->m_pDataStack->vPushInt(bEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.commit -- commit a tx (via a txRef on stack)                         */
/*----------------------------------------------------------------------------*/
void Task::vTxRefCommit(MP_Task mpTask)
{
ENTRY("vTxRefCommit"_Str);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.commit"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bCommitted = pTxRef->bCommitTx();
  
  mpTask->m_pDataStack->vPushInt(bCommitted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.rollback -- rollback a tx (via a txRef on stack)                     */
/*----------------------------------------------------------------------------*/
void Task::vTxRefRollback(MP_Task mpTask)
{
ENTRY("vTxRefRollback"_Str);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.rollback"_Str);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bRolledBack = pTxRef->bRollbackTx();
  
  mpTask->m_pDataStack->vPushInt(bRolledBack);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
