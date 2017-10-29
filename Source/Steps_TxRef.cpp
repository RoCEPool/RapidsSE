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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// txRef: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TxRef(void)
{
  vAddPrimitive("txRef.begin"_Stz,     &vTxRefBegin,       false);
  vAddPrimitive("txRef::checkDistId"_Stz, &vTxRefCheckDistId, false);
  vAddPrimitive("txRef.commit"_Stz,    &vTxRefCommit,      false);
  vAddPrimitive("txRef.dump"_Stz,      &vTxRefDump,        false);
  vAddPrimitive("txRef.end"_Stz,       &vTxRefEnd,         false);
  vAddPrimitive("txRef.id"_Stz,        &vTxRefId,          false);
  vAddPrimitive("txRef.init"_Stz,      &vTxRefInit,        false);
  vAddPrimitive("txRef::new"_Stz,      &vTxRefNew,         false);
  vAddPrimitive("txRef.rollback"_Stz,  &vTxRefRollback,    false);
}

/*----------------------------------------------------------------------------*/
/* txRef.begin -- begin a tx (via a txRef on stack)                           */
/*----------------------------------------------------------------------------*/
void Task::vTxRefBegin(MP_Task mpTask)
{
ENTRY("vTxRefBegin"_Stz);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("tx.begin"_Stz);
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  
  bool bBegun = pTxRef->bBeginTx();
  
  mpTask->m_pDataStack->vPushInt(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.checkDistId -- check existance of distributed tx id                  */
/*----------------------------------------------------------------------------*/
void Task::vTxRefCheckDistId(MP_Task mpTask)
{
  ENTRY("vTxRefCheckDistId"_Stz);
  
  i8 iDistTxId;
  
  if ( ! mpTask->m_pDataStack->bPop(iDistTxId) )
    THROW_INVALID_DSTACK("txRef.checkDistId"_Stz);
  
  vTrace("Checking distributed id %lld\n"_Stz,iDistTxId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.commit -- commit a tx (via a txRef on stack)                         */
/*----------------------------------------------------------------------------*/
void Task::vTxRefCommit(MP_Task mpTask)
{
ENTRY("vTxRefCommit"_Stz);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.commit"_Stz);
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  
  bool bCommitted = pTxRef->bCommitTx();
  
  mpTask->m_pDataStack->vPushInt(bCommitted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.dump -- dump a tx (via a txRef on stack)                             */
/*----------------------------------------------------------------------------*/
void Task::vTxRefDump(MP_Task mpTask)
{
  ENTRY("vTxRefDump"_Stz);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.dump"_Stz);
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  
  pTxRef->vDump();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.end -- end a tx (via a txRef on stack)                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRefEnd(MP_Task mpTask)
{
  ENTRY("vTxRefEnd"_Stz);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.end"_Stz);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bEnded = pTxRef->bEndTx();
  
  mpTask->m_pDataStack->vPushInt(bEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.id -- get tx id from txref on data stack                             */
/*----------------------------------------------------------------------------*/
void Task::vTxRefId(MP_Task mpTask)
{
  ENTRY("vTxRefId"_Stz);
  
  Object* pPtrTxRef;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.id"_Stz);
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  
  i8 iTxId = pTxRef->iTxId();
  
  mpTask->m_pDataStack->vPushInt(iTxId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.init -- init of txref on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vTxRefInit(MP_Task mpTask)
{
  ENTRY("vTxRefInit"_Stz);
  
  Object* pPtrTxRef;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.init"_Stz);
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  new(pTxRef) TxRef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef::new -- allocate new txref                                           */
/*----------------------------------------------------------------------------*/
void Task::vTxRefNew(MP_Task mpTask)
{
  ENTRY("vTxRefNew"_Stz);
  
  TxRef* pTxRef = new TxRef;
  
  mpTask->m_pDataStack->vPushObject(pTxRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.rollback -- rollback a tx (via a txRef on stack)                     */
/*----------------------------------------------------------------------------*/
void Task::vTxRefRollback(MP_Task mpTask)
{
ENTRY("vTxRefRollback"_Stz);
  
  Object* pPtrTxRef;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("txRef.rollback"_Stz);  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  
  bool bRolledBack = pTxRef->bRollbackTx();
  
  mpTask->m_pDataStack->vPushInt(bRolledBack);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
