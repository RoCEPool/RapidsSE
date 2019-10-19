/*============================================================================*/
/* Steps_Tx.cpp - tx step grou                                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Tx(void)
{
  vAddStep("tx::begin"_Stz,      &Step::vTxBegin);
  vAddStep("tx.commit"_Stz,      &Step::vTxCommit);
  vAddStep("tx.counterDec"_Stz,  &Step::vTxCounterDec);
  vAddStep("tx.counter@"_Stz,    &Step::vTxCounterFetch);
  vAddStep("tx.counter!"_Stz,    &Step::vTxCounterStore);
  vAddStep("tx.distMember@"_Stz, &Step::vTxDistMemberFetch);
  vAddStep("tx.distMember!"_Stz, &Step::vTxDistMemberStore);
  vAddStep("tx.dump"_Stz,        &Step::vTxDump);
  vAddStep("tx::dumpAll"_Stz,    &Step::vTxDumpAll);
  vAddStep("tx.free"_Stz,        &Step::vTxFree);
  vAddStep("tx.id"_Stz,          &Step::vTxId);
  vAddStep("tx.prepare"_Stz,     &Step::vTxPrepare);
  vAddStep("tx.rollback"_Stz,    &Step::vTxRollback);
  vAddStep("tx::testDist"_Stz,   &Step::vTxTestDist);
}

/*----------------------------------------------------------------------------*/
/* tx::begin -- begin a tx                                                    */
/*----------------------------------------------------------------------------*/
void Step::vTxBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxBegin"_Stz);
  
  Tx* pTx = new Tx;
  
//  vTrace("Begun tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
//         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
//         pTx->iDistTxNode(), pTx->iDistTxNo(),
//         pTx->aDecode());
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.commit -- commit a tx                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTxCommit(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxCommit"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vPhase(XP_COMMITTED);

//  vTrace("Committed tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
//         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
//         pTx->iDistTxNode(), pTx->iDistTxNo(),
//         pTx->aDecode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.counterDec -- decrement tx general counter                              */
/*----------------------------------------------------------------------------*/
void Step::vTxCounterDec(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxCounterDec"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iCounter = pTx->iCounterDec();
  
//  vTrace("Dec counter tx: %lld.%lld-%lld.%lld(%s) now %lld\n"_Stz,
//         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
//         pTx->iDistTxNode(), pTx->iDistTxNo(),
//         pTx->aDecode(),
//         iCounter);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iCounter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.counter@ -- fetch tx general counter                                    */
/*----------------------------------------------------------------------------*/
void Step::vTxCounterFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxCounterFetch"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iCounter = pTx->iCounterGet();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iCounter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.counter! -- store tx general counter                                    */
/*----------------------------------------------------------------------------*/
void Step::vTxCounterStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxCounterStore"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iCounter;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iCounter) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  pTx->vCounterSet(iCounter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.distMember@ -- fetch if given node is a distributed member of tx        */
/*----------------------------------------------------------------------------*/
void Step::vTxDistMemberFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxDistMemberFetch"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  ASSERT(pTx->bLocalTx());
  
  i8 i_Node;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(i_Node) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  bool bDistMember = pTx->bDistMember(i_Node);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(bDistMember);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.distMember! -- store if given node is a distributed member of tx        */
/*----------------------------------------------------------------------------*/
void Step::vTxDistMemberStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxDistMemberStore"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  ASSERT(pTx->bLocalTx());
  
  i8 i_Node;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(i_Node) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);

  i8 iDistMember;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iDistMember) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  pTx->vDistMember(i_Node,iDistMember);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.dump -- dump a tx                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTxDump(MP_Task mpTask, Forth* pForth)
{
ENTRY("vTxDump"_Stz);
  
  Object* pPtrTx;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx::dumpAll -- dump all tx's                                               */
/*----------------------------------------------------------------------------*/
void Step::vTxDumpAll(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxDumpAll"_Stz);
  
  AM::pTxSet()->vDumpAll();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.free -- free a tx                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTxFree(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxFree"_Stz);
  
  Object* pPtrTx;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
//  vTrace("Freeing tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
//         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
//         pTx->iDistTxNode(), pTx->iDistTxNo(),
//         pTx->aDecode());
  
  delete pTx;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.id -- get tx id from tx on data stack                                   */
/*----------------------------------------------------------------------------*/
void Step::vTxId(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxId"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  i8 iTxId = pTx->iLocalTxId();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iTxId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.prepare -- prepare a tx                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTxPrepare(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxPrepare"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vPhase(XP_PREPARED);
  
//  vTrace("Prepared tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
//         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
//         pTx->iDistTxNode(), pTx->iDistTxNo(),
//         pTx->aDecode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.rollback -- rollback a tx                                               */
/*----------------------------------------------------------------------------*/
void Step::vTxRollback(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxRollback"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrTx) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vPhase(XP_ROLLEDBACK);
  
//  vTrace("Rolledback tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
//         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
//         pTx->iDistTxNode(), pTx->iDistTxNo(),
//         pTx->aDecode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx::testDist -- test for a distributed tx and create if it doesnt exist    */
/*----------------------------------------------------------------------------*/
void Step::vTxTestDist(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTxTestDist"_Stz);
  
  i8 iDistTxId;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iDistTxId) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Tx* pTx = Tx::pTestDistTx(iDistTxId);

//  vTrace("Testing dist tx: %lld.%lld-%lld.%lld(%s)\n"_Stz,
//         pTx->iLocalTxNode(), pTx->iLocalTxNo(),
//         pTx->iDistTxNode(), pTx->iDistTxNo(),
//         pTx->aDecode());
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
