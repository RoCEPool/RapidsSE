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
 /*
  JobClass* pJobClass_SubFetch = JobClass::pFind(CN_JOB_SUBINDEXJOIN);
  
  Job_SubFetch* pJob_SubFetch =
  static_cast<Job_SubFetch*>(pJobClass_SubFetch->pPreJob(pSession));
  
  pJob_SubFetch->vSeed(SS_SEED_RUN);
  
  pJob_SubFetch->m_iParentId    = pJob_IndexJoin->m_iJobId;
  pJob_SubFetch->m_pParentJob   = pJob_IndexJoin;
  pJob_SubFetch->m_sObjectName  = pJob_IndexJoin->m_sObjectName;
  pJob_SubFetch->m_pPlanner     = pPlanner;
  pJob_SubFetch->m_iPhrase      = iPhrase;
  pJob_SubFetch->m_i_Slot       = iSlot;
  pJob_SubFetch->m_pBufferQueue = pJob_IndexJoin->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubFetch,SS_SEED_RUN));
*/
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
