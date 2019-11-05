/*============================================================================*/
/* Steps_JobIndexJoin.cpp - jobIndexJoin step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobIndexJoin(void)
{
  vAddStep("Job_IndexJoin.lastBuffers!"_Stz,    &Step::vJob_IndexJoinLastBuffersStore);
  vAddStep("Job_IndexJoin.leftIndex!"_Stz,      &Step::vJob_IndexJoinLeftIndexStore);
  vAddStep("Job_IndexJoin.leftTable!"_Stz,      &Step::vJob_IndexJoinLeftTableStore);
  vAddStep("Job_IndexJoin.rightIndex!"_Stz,     &Step::vJob_IndexJoinRightIndexStore);
  vAddStep("Job_IndexJoin.rightIndexName@"_Stz, &Step::vJob_IndexJoinRightIndexNameFetch);
  vAddStep("Job_IndexJoin.rightTable!"_Stz,     &Step::vJob_IndexJoinRightTableStore);
  vAddStep("Job_IndexJoin.rightTableName@"_Stz, &Step::vJob_IndexJoinRightTableNameFetch);
  
  vAddStep("Job_IndexJoin.spawnSub"_Stz,        &Step::vJob_IndexJoinSpawnSub);
  
  vAddStep("Job_SubIndexJoin.indexJoin"_Stz,    &Step::vJob_SubIndexJoinIndexJoin);
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.lastBuffers! -- set no of expected last buffers              */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinLastBuffersStore(void)
{
  ENTRY("vJob_IndexJoinLastBuffersStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  i8 nLastBuffers;
  if ( ! m_pDataStack->bPopAsInt(nLastBuffers) )
    THROW_DSTACK();
  
  pJob_IndexJoin->m_nLastBuffers = nLastBuffers;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.leftIndex! -- set the left index                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinLeftIndexStore(void)
{
  ENTRY("vJob_IndexJoinLeftIndexStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  Object* pIndexPtr;
  if ( ! m_pDataStack->bPopObject(pIndexPtr) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pIndexPtr);
  
  pJob_IndexJoin->m_pLeftIndex = pIndex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.leftTable! -- store left table in the job                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinLeftTableStore(void)
{
  ENTRY("vJob_IndexJoinLeftTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  Object* pTablePtr;
  if ( ! m_pDataStack->bPopObject(pTablePtr) )
    THROW_DSTACK();
  Table* pTable = _pTable(pTablePtr);
  
  pJob_IndexJoin->m_pLeftTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.rightIndex! -- set the right index                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinRightIndexStore(void)
{
  ENTRY("vJob_IndexJoinRightIndexStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  Object* pIndexPtr;
  if ( ! m_pDataStack->bPopObject(pIndexPtr) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pIndexPtr);
  
  pJob_IndexJoin->m_pRightIndex = pIndex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.rightIndexName@ -- get the right index name                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinRightIndexNameFetch(void)
{
  ENTRY("vJob_IndexJoinRightIndexNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  m_pDataStack->vPushS32(pJob_IndexJoin->m_sRightIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.rightTable! -- store right table in the job                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinRightTableStore(void)
{
  ENTRY("vJob_IndexJoinRightTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  Object* pTablePtr;
  if ( ! m_pDataStack->bPopObject(pTablePtr) )
    THROW_DSTACK();
  Table* pTable = _pTable(pTablePtr);
  
  pJob_IndexJoin->m_pRightTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.rightTableName@ -- get the right table name                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinRightTableNameFetch(void)
{
  ENTRY("vJob_IndexJoinRightTableNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  m_pDataStack->vPushS32(pJob_IndexJoin->m_sRightTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.spawnSub -- spawn sub fetch job                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinSpawnSub(void)
{
  ENTRY("vJob_IndexJoinSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);

  i8 iSlot;
  if ( ! m_pDataStack->bPopAsInt(iSlot) )
    THROW_DSTACK();
  
  Session* pSession = pJob_IndexJoin->pSession();
  ReqTuple* aReqTuple = &pJob_IndexJoin->m_xReqTuple;

  JobClass* pJobClass_SubIndexJoin = JobClass::pFind(CN_JOB_SUBINDEXJOIN);
  
  Job_SubIndexJoin* pJob_SubIndexJoin =
    static_cast<Job_SubIndexJoin*>
      (pJobClass_SubIndexJoin->pPreJob(aReqTuple,pSession));
  
  pJob_SubIndexJoin->vSeed(SS_SEED_RUN);
  
  pJob_SubIndexJoin->m_iParentId    = pJob_IndexJoin->m_iFocalId;
  pJob_SubIndexJoin->m_pParentJob   = pJob_IndexJoin;
  pJob_SubIndexJoin->m_oEnameSet.vAdd("<tableNm>"_Stz,
                                    pJob_IndexJoin->m_oEnameSet.szValue("<tableNm>"_Stz));
  pJob_SubIndexJoin->vSlot(iSlot);
  pJob_SubIndexJoin->m_pBufferQueue = pJob_IndexJoin->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pJob_SubIndexJoin));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_SubIndexJoin.indexJoin -- execute the index join                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIndexJoinIndexJoin(void)
{
  ENTRY("vJob_SubIndexJoinIndexJoin"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubIndexJoin* pJob_SubIndexJoin = _pJob_SubIndexJoin(pJobPtr);
  
  // get the slot we are working in
  i8 iSlot = pJob_SubIndexJoin->i_Slot();
  
  // get the buffer queue
  Queue* pQueue = pJob_SubIndexJoin->m_pBufferQueue;
  
  // get the Job_IndexJoin
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJob_SubIndexJoin->m_pParentJob);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the left table and left index
  Table* pLeftTable = pJob_IndexJoin->m_pLeftTable;
  Index* pLeftIndex = pJob_IndexJoin->m_pLeftIndex;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the right table and right index
  Table* pRightTable = pJob_IndexJoin->m_pRightTable;
  Index* pRightIndex = pJob_IndexJoin->m_pRightIndex;
  
  // get the left index tree
  TablePtn* pLeftPartition = pLeftTable->pPartition(iSlot);
  i8 iLeftIndexNo = pLeftIndex->iIndexNo();
  IndexTree* pLeftIndexTree = pLeftPartition->pIndexTree(iLeftIndexNo);
  
  // get the right index tree
  TablePtn* pRightPartition = pRightTable->pPartition(iSlot);
  i8 iRightIndexNo = pRightIndex->iIndexNo();
  IndexTree* pRightIndexTree = pRightPartition->pIndexTree(iRightIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // execute the index join on the two tables
  //JoinOp::vIndexJoin(pQueue, pLeftIndexTree, pRightIndexTree);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
