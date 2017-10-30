/*============================================================================*/
/* Steps_Tx.cpp - tx step grou                                                */
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
extern TxSet*          gpTxSet;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// tx: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Tx(void)
{
  vAddPrimitive("tx::begin"_Stz,        &vTxBegin,        false);
  vAddPrimitive("tx.commit"_Stz,        &vTxCommit,       false);
  vAddPrimitive("tx.counterDec"_Stz,    &vTxCounterDec,   false);
  vAddPrimitive("tx.counter@"_Stz,      &vTxCounterFetch, false);
  vAddPrimitive("tx.counter!"_Stz,      &vTxCounterStore, false);
  vAddPrimitive("tx.dump"_Stz,          &vTxDump,         false);
  vAddPrimitive("tx::dumpAll"_Stz,      &vTxDumpAll,      false);
  vAddPrimitive("tx.id"_Stz,            &vTxId,           false);
  vAddPrimitive("tx.rollback"_Stz,      &vTxRollback,     false);
  vAddPrimitive("tx::testDist"_Stz,     &vTxTestDist,     false);
}

/*----------------------------------------------------------------------------*/
/* tx::begin -- begin a tx                                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxBegin(MP_Task mpTask)
{
  ENTRY("vTxBegin"_Stz);
  
  Tx* pTx = gpTxSet->pAllocLocalTx();
  
  pTx->vPhase(TP_EXECUTING);
  
  mpTask->m_pDataStack->vPushObject(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.commit -- commit a tx                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTxCommit(MP_Task mpTask)
{
  ENTRY("vTxCommit"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.commit"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vPhase(TP_COMMITTED);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.counterDec -- decrement tx general counter                              */
/*----------------------------------------------------------------------------*/
void Task::vTxCounterDec(MP_Task mpTask)
{
  ENTRY("vTxCounterDec"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.counter@"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iCounter = pTx->iCounterDec();
  
  mpTask->m_pDataStack->vPushInt(iCounter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.counter@ -- fetch tx general counter                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCounterFetch(MP_Task mpTask)
{
  ENTRY("vTxCounterFetch"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.counter@"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iCounter = pTx->iCounterGet();
  
  mpTask->m_pDataStack->vPushInt(iCounter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.counter! -- store tx general counter                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCounterStore(MP_Task mpTask)
{
  ENTRY("vTxCounterStore"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.counter@"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iCounter;
  if ( ! mpTask->m_pDataStack->bPop(iCounter) )
    THROW_INVALID_DSTACK("thread.actorModel!"_Stz);
  
  pTx->vCounterSet(iCounter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.dump -- dump a tx                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTxDump(MP_Task mpTask)
{
ENTRY("vTxDump"_Stz);
  
  Object* pPtrTx;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.dump"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx::dumpAll -- dump all tx's                                               */
/*----------------------------------------------------------------------------*/
void Task::vTxDumpAll(MP_Task mpTask)
{
  ENTRY("vTxDumpAll"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.id -- get tx id from tx on data stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTxId(MP_Task mpTask)
{
  ENTRY("vTxId"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.id"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iTxId = pTx->iTxId();
  
  mpTask->m_pDataStack->vPushInt(iTxId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.rollback -- rollback a tx                                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRollback(MP_Task mpTask)
{
  ENTRY("vTxRollback"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.rollback"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vPhase(TP_ROLLEDBACK);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx::testDist -- test for a distributed tx and create if it doesnt exist    */
/*----------------------------------------------------------------------------*/
void Task::vTxTestDist(MP_Task mpTask)
{
  ENTRY("vTxTestDist"_Stz);
  
  i8 iDistTxId;
  if ( ! mpTask->m_pDataStack->bPop(iDistTxId) )
    THROW_INVALID_DSTACK("tx.rollback"_Stz);
  
  Tx* pTx = gpTxSet->pTestDistTx(iDistTxId);
  
  mpTask->m_pDataStack->vPushObject(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
