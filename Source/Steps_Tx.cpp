/*============================================================================*/
/* Steps_Tx.cpp - tx step grou                                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
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
  vAddPrimitive("tx::begin"_Stz,      &vTxBegin,           false);
  vAddPrimitive("tx.commit"_Stz,      &vTxCommit,          false);
  vAddPrimitive("tx.counterDec"_Stz,  &vTxCounterDec,      false);
  vAddPrimitive("tx.counter@"_Stz,    &vTxCounterFetch,    false);
  vAddPrimitive("tx.counter!"_Stz,    &vTxCounterStore,    false);
  vAddPrimitive("tx.distMember@"_Stz, &vTxDistMemberFetch, false);
  vAddPrimitive("tx.distMember!"_Stz, &vTxDistMemberStore, false);
  vAddPrimitive("tx.dump"_Stz,        &vTxDump,            false);
  vAddPrimitive("tx::dumpAll"_Stz,    &vTxDumpAll,         false);
  vAddPrimitive("tx.free"_Stz,        &vTxFree,            false);
  vAddPrimitive("tx.id"_Stz,          &vTxId,              false);
  vAddPrimitive("tx.prepare"_Stz,     &vTxPrepare,         false);
  vAddPrimitive("tx.rollback"_Stz,    &vTxRollback,        false);
  vAddPrimitive("tx::testDist"_Stz,   &vTxTestDist,        false);
}

/*----------------------------------------------------------------------------*/
/* tx::begin -- begin a tx                                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxBegin(MP_Task mpTask)
{
  ENTRY("vTxBegin"_Stz);
  
  Tx* pTx = new Tx;
  
  vTrace("Begun tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
         pTx->iDistTxNode(), pTx->iDistTxNo(),
         pTx->aDecode());
  
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

  vTrace("Committed tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
         pTx->iDistTxNode(), pTx->iDistTxNo(),
         pTx->aDecode());
  
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
    THROW_INVALID_DSTACK("tx.counterDec"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iCounter = pTx->iCounterDec();
  
  vTrace("Dec counter tx: %lld.%lld-%lld.%lld(%s) now %lld\n"_Stz,
         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
         pTx->iDistTxNode(), pTx->iDistTxNo(),
         pTx->aDecode(),
         iCounter);
  
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
    THROW_INVALID_DSTACK("tx.counter!"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iCounter;
  if ( ! mpTask->m_pDataStack->bPop(iCounter) )
    THROW_INVALID_DSTACK("tx.counter!"_Stz);
  
  pTx->vCounterSet(iCounter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.distMember@ -- fetch if given node is a distributed member of tx        */
/*----------------------------------------------------------------------------*/
void Task::vTxDistMemberFetch(MP_Task mpTask)
{
  ENTRY("vTxDistMemberFetch"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.distMember@"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  ASSERT(pTx->bLocalTx());
  
  i8 i_Node;
  if ( ! mpTask->m_pDataStack->bPop(i_Node) )
    THROW_INVALID_DSTACK("tx.distMember@"_Stz);
  
  bool bDistMember = pTx->bDistMember(i_Node);
  
  mpTask->m_pDataStack->vPushInt(bDistMember);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.distMember! -- store if given node is a distributed member of tx        */
/*----------------------------------------------------------------------------*/
void Task::vTxDistMemberStore(MP_Task mpTask)
{
  ENTRY("vTxDistMemberStore"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.distMember!"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  ASSERT(pTx->bLocalTx());
  
  i8 i_Node;
  if ( ! mpTask->m_pDataStack->bPop(i_Node) )
    THROW_INVALID_DSTACK("tx.distMember!"_Stz);

  i8 iDistMember;
  if ( ! mpTask->m_pDataStack->bPop(iDistMember) )
    THROW_INVALID_DSTACK("tx.distMember!"_Stz);
  
  pTx->vDistMember(i_Node,iDistMember);
  
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
  
  gpTxSet->vDumpAll();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.free -- free a tx                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTxFree(MP_Task mpTask)
{
  ENTRY("vTxFree"_Stz);
  
  Object* pPtrTx;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.free"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  vTrace("Freeing tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
         pTx->iDistTxNode(), pTx->iDistTxNo(),
         pTx->aDecode());
  
  delete pTx;
  
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
  
  i8 iTxId = pTx->iLocalTxId();
  
  mpTask->m_pDataStack->vPushInt(iTxId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.prepare -- prepare a tx                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxPrepare(MP_Task mpTask)
{
  ENTRY("vTxPrepare"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.prepare"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vPhase(TP_PREPARED);
  
  vTrace("Prepared tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
         pTx->iDistTxNode(), pTx->iDistTxNo(),
         pTx->aDecode());
  
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
  
  vTrace("Rolledback tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
         pTx->iDistTxNode(), pTx->iDistTxNo(),
         pTx->aDecode());
  
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
    THROW_INVALID_DSTACK("tx.testDist"_Stz);
  
  Tx* pTx = Tx::pTestDistTx(iDistTxId);

  vTrace("Testing dist tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
         pTx->iDistTxNode(), pTx->iDistTxNo(),
         pTx->aDecode());
  
  mpTask->m_pDataStack->vPushObject(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
