/*============================================================================*/
/* Steps_JobIndexJoin.cpp - jobIndexJoin step group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vJob_IndexJoinLastBuffersStore(MP_Task mpTask)
{
  ENTRY("vJob_IndexJoinLastBuffersStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.lastBuffers!"_Stz);
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  i8 nLastBuffers;
  if ( ! mpTask->m_pDataStack->bPopAsInt(nLastBuffers) )
    THROW_INVALID_DSTACK("Job_IndexJoin.lastBuffers"_Stz);
  
  pJob_IndexJoin->m_nLastBuffers = nLastBuffers;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.leftIndex! -- set the left index                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinLeftIndexStore(MP_Task mpTask)
{
  ENTRY("vJob_IndexJoinLeftIndexStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.leftIndex!"_Stz);
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  Object* pIndexPtr;
  if ( ! mpTask->m_pDataStack->bPop(pIndexPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.leftIndex!"_Stz);
  Index* pIndex = _pIndex(pIndexPtr);
  
  pJob_IndexJoin->m_pLeftIndex = pIndex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.leftTable! -- store left table in the job                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinLeftTableStore(MP_Task mpTask)
{
  ENTRY("vJob_IndexJoinLeftTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.leftTable!"_Stz);
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.leftTable"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_IndexJoin->m_pLeftTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.rightIndex! -- set the right index                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinRightIndexStore(MP_Task mpTask)
{
  ENTRY("vJob_IndexJoinRightIndexStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.rightIndex!"_Stz);
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  Object* pIndexPtr;
  if ( ! mpTask->m_pDataStack->bPop(pIndexPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.rightIndex!"_Stz);
  Index* pIndex = _pIndex(pIndexPtr);
  
  pJob_IndexJoin->m_pRightIndex = pIndex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.rightIndexName@ -- get the right index name                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinRightIndexNameFetch(MP_Task mpTask)
{
  ENTRY("vJob_IndexJoinRightIndexNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.rightIndexName!"_Stz);
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  mpTask->m_pDataStack->vPushS32(pJob_IndexJoin->m_sRightIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.rightTable! -- store right table in the job                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinRightTableStore(MP_Task mpTask)
{
  ENTRY("vJob_IndexJoinRightTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.rightTable!"_Stz);
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.rightTable"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_IndexJoin->m_pRightTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.rightTableName@ -- get the right table name                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinRightTableNameFetch(MP_Task mpTask)
{
  ENTRY("vJob_IndexJoinRightTableNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.rightTableName!"_Stz);
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);
  
  mpTask->m_pDataStack->vPushS32(pJob_IndexJoin->m_sRightTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_IndexJoin.spawnSub -- spawn sub fetch job                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_IndexJoinSpawnSub(MP_Task mpTask)
{
  ENTRY("vJob_IndexJoinSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.spawnSub"_Stz);
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJobPtr);

  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("Job_IndexJoin.spawnSub"_Stz);
  
  Session* pSession = pJob_IndexJoin->m_pSession;

  JobClass* pJobClass_SubIndexJoin = JobClass::pFind(CN_JOB_SUBINDEXJOIN);
  
  Job_SubIndexJoin* pJob_SubIndexJoin =
  static_cast<Job_SubIndexJoin*>(pJobClass_SubIndexJoin->pPreJob(pSession));
  
  pJob_SubIndexJoin->vSeed(SS_SEED_RUN);
  
  pJob_SubIndexJoin->m_iParentId    = pJob_IndexJoin->m_iJobId;
  pJob_SubIndexJoin->m_pParentJob   = pJob_IndexJoin;
  pJob_SubIndexJoin->m_sObjectName  = pJob_IndexJoin->m_sObjectName;
  pJob_SubIndexJoin->m_i_Slot       = iSlot;
  pJob_SubIndexJoin->m_pBufferQueue = pJob_IndexJoin->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubIndexJoin,SS_SEED_RUN));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_SubIndexJoin.indexJoin -- execute the index join                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIndexJoinIndexJoin(MP_Task mpTask)
{
  ENTRY("vJob_SubIndexJoinIndexJoin"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_IndexJoin.spawnSub"_Stz);
  Job_SubIndexJoin* pJob_SubIndexJoin = _pJob_SubIndexJoin(pJobPtr);
  
  // get the slot we are working in
  i8 iSlot = pJob_SubIndexJoin->m_i_Slot;
  
  // get the buffer queue
  Queue* pQueue = pJob_SubIndexJoin->m_pBufferQueue;
  
  // get the Job_IndexJoin
  Job_IndexJoin* pJob_IndexJoin = _pJob_IndexJoin(pJob_SubIndexJoin->m_pParentJob);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a refBuffer for the results
  // pRefBuffer = new RefBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the left table and left index
  Table* pLeftTable = pJob_IndexJoin->m_pLeftTable;
  Index* pLeftIndex = pJob_IndexJoin->m_pLeftIndex;
  
  // get the left index tree
  TablePtn* pLeftPartition = pLeftTable->pPartition(iSlot);
  i8 iLeftIndexNo = pLeftIndex->iIndexNo();
  IndexTree* pLeftIndexTree = pLeftPartition->pIndexTree(iLeftIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the right table and right index
  Table* pRightTable = pJob_IndexJoin->m_pRightTable;
  Index* pRightIndex = pJob_IndexJoin->m_pRightIndex;
  
  // get the right index tree
  TablePtn* pRightPartition = pRightTable->pPartition(iSlot);
  i8 iRightIndexNo = pRightIndex->iIndexNo();
  IndexTree* pRightIndexTree = pRightPartition->pIndexTree(iRightIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first left Buffer
  EntryBuffer* pLeftBuffer = pLeftIndexTree->pRoot();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the right left Buffer
  EntryBuffer* pRightBuffer = pRightIndexTree->pRoot();
  
  // get the lead entry of the left buffer
  Item* aLeftEntry = pLeftBuffer->aLeadEntry();
  
  // get the lead entry of the right buffer
  Item* aRightEntry = pRightBuffer->aLeadEntry();
  
  Formatters* pFormatters = &gpAttachMap->m_oFormatters;
  
  //Column* pLeftColumn0    = pLeftIndex->pIdxColVec()->pGet(0);
  //Column* pRightColumn0   = pRightIndex->pIdxColVec()->pGet(0);
  
  bool bMoveLeft;
  bool bMoveRight;
  pLeftBuffer->vDump();
  pRightBuffer->vDump();
  
  while (aLeftEntry || aRightEntry) {
    
    bMoveLeft = false;
    bMoveRight = false;
    
//    if (aLeftEntry)
//      pLeftBuffer->vShowEntry(aLeftEntry);
//    if (aRightEntry)
//      pRightBuffer->vShowEntry(aRightEntry);
    
    // we have a left entry
    if (aLeftEntry) {
    
      // right entry ?
      if (aRightEntry) {
        
        // compare the keys
        eKC eCompare = KeyData::eCompare(aLeftEntry, aRightEntry);
        
        switch (eCompare) {
          case KC_SAME:
            // leftKey == rightKey
            vTrace("Joining rows %p %p\n"_Stz, aLeftEntry, aRightEntry);
            bMoveLeft = true;
            bMoveRight = true;
            break;
          case KC_BEFORE:
            // leftKey < rightKey
            vTrace("Joining rows %p NULL\n"_Stz, aLeftEntry);
            bMoveLeft = true;
            break;
          case KC_AFTER:
            // leftKey > rightKey
            vTrace("Joining rows NULL %p\n"_Stz, aRightEntry);
            bMoveRight = true;
            break;
          default:
            NEVER_REACH;
        }
      }
      else {
        // no right entry
        vTrace("Joining rows %p NULL\n"_Stz, aLeftEntry);
        bMoveLeft = true;
      }
    }
    else {
      // no left entry
        
      // right entry ?
      if (aRightEntry) {
        vTrace("Joining rows NULL %p\n"_Stz, aRightEntry);
        bMoveRight = true;
      }
      
    }
    
    if (bMoveLeft) {
      // move to the next left
      if (aLeftEntry) {
        aLeftEntry = EntryBuffer::aGetNextEntry(&pLeftBuffer, aLeftEntry);
      }
    }
  
    if (bMoveRight) {
      // move to the next right
      if (aRightEntry) {
        aRightEntry = EntryBuffer::aGetNextEntry(&pRightBuffer, aRightEntry);
      }
    }
  }
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
