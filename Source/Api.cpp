/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern AttachMap*   gpAttachMap;
extern ActivityMap* gpActivityMap;


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete jobs                                                                */
/*----------------------------------------------------------------------------*/
void Api::vDeleteJobs(Session* pSession)
{
  Stack* pJobStack = pSession->m_pJobStack;
  Object* pJobObject;
  
  while (pJobStack->bPop(pJobObject)) {
    
    Job* aJob = (Job*)pJobObject;
    
    // find the job class on this node
    eCN eClass = aJob->eClass();
    JobClass* pJobClass = JobClass::pFind(eClass);
    
    ApiState::vMarkLocation(pSession,200);
    
    //vTrace("API deleting job %p jobId %lld %s\n"_Stz, aJob, aJob->m_iJobId, aCN[eClass]);
    
    // If we come across a JOB_FETCHROWS then we need to empty its queues
    if (eClass == CN_JOB_FETCHROWS) {
      Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(aJob);
      
      //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pTable->sTable(),
      //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
      
      // if we had an entry buffer delete it
      pJob_FetchRows->vDeleteBuffer();
      
      while (pJob_FetchRows->m_nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers) {
        // pop an entryBuffer off the queue
        ApiState::vMarkLocation(pSession,300);
        
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        //        GenVar xVariable;
        //        while (!pJob_FetchRows->m_pBufferQueue->bPop(&xVariable)) {
        //          Pthread::vSleepMilli(1);
        //          ApiState::vMarkLocation(pSession,400);
        //          continue;
        //        }
        
        //        ApiState::vMarkLocation(pSession,500);
        //
        //        PtrVal* aPtrVal = xVariable.aNestedPtrVal();
        //        pJob_FetchRows->m_pFetchBuffer = (FetchBuffer*)(aPtrVal->pObj());
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        FetchBuffer* pFetchBuffer;
        for (;;) {
          pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
          if (pFetchBuffer)
            break;
          Pthread::vSleepMilli(1,CN_JOB_FETCHROWS);
          ApiState::vMarkLocation(pSession,400);
          continue;
        }
        ApiState::vMarkLocation(pSession,500);
        
        pJob_FetchRows->m_pFetchBuffer = pFetchBuffer;
        // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
        
        
        if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
          pJob_FetchRows->m_nLastBuffersScanned++;
          //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_pTable->sTable(),
          //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
        }
        
        // delete the buffer
        pJob_FetchRows->vDeleteBuffer();
      }
    }
    ApiState::vMarkLocation(pSession,600);
    
    // delete the job
    //vTrace("Deleting job %p %s\n"_Stz, aJob, aCN[aJob->eClass()]);
    pJobClass->vDeleteJob(pSession,aJob);
    
    pSession->m_nJobDelete++;
  }
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
Job* Api::pExecuteCommand(Session* pSession, Stz* sCommand,
                          bool bPostJob, bool bFromPrompt)
{
  CommandProcessor* pCommandProcessor = pSession->m_pCommandProcessor;
  
  ApiState::vMarkLocation(pSession,101);
  
  pCommandProcessor->vTokenise(sCommand);
  
  bool bFound = pCommandProcessor->bFindJobClass();
  if (! bFound) {
    return nullptr;
  }
  
  pCommandProcessor->vPreJob(pSession);
  
  Job* pJob = pCommandProcessor->pJob();
  if (!pJob) {
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn and wait for job
  Api::vSpawnJobAndWait(pSession, pJob);
  
  // run post ?
  if (bPostJob)
    pCommandProcessor->vPostJob(pSession);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Api::vIncStatistics(eAC eApiCall)
{
  if (! gpAttachMap)
    return;
  
  switch (eApiCall) {
    case AC_BeginInsert:         gpAttachMap->m_oStatistics.Api.m_nBeginInsert++;          break;
    case AC_BeginSubInserter:    gpAttachMap->m_oStatistics.Api.m_nBeginSubInserter++;     break;
    case AC_CheckVersion:        gpAttachMap->m_oStatistics.Api.m_nCheckVersion++;         break;
    case AC_CopyBuffer:          gpAttachMap->m_oStatistics.Api.m_nCopyBuffer++;           break;
    case AC_DeleteSession:       gpAttachMap->m_oStatistics.Api.m_nDeleteSession++;        break;
    case AC_EndInsert:           gpAttachMap->m_oStatistics.Api.m_nEndInsert++;            break;
    case AC_EndJobs:             gpAttachMap->m_oStatistics.Api.m_nEndJobs++;              break;
    case AC_EndSubInserter:      gpAttachMap->m_oStatistics.Api.m_nEndSubInserter++;       break;
    case AC_ExecuteCommand:      gpAttachMap->m_oStatistics.Api.m_nExecuteCommand++;       break;
    case AC_FetchNextBuffer:     gpAttachMap->m_oStatistics.Api.m_nFetchNextBuffer++;      break;
    case AC_GetLoadResult:       gpAttachMap->m_oStatistics.Api.m_nGetLoadResult++;        break;
    case AC_GetRecordBuffer:     gpAttachMap->m_oStatistics.Api.m_nGetRecordBuffer++;      break;
    case AC_GetRow:              gpAttachMap->m_oStatistics.Api.m_nGetRow++;               break;
    case AC_GetRowCount:         gpAttachMap->m_oStatistics.Api.m_nGetRowCount++;          break;
    case AC_GetTableColumnRow:   gpAttachMap->m_oStatistics.Api.m_nGetTableColumnRow++;    break;
    case AC_GetTableColumnRowCount: gpAttachMap->m_oStatistics.Api.m_nGetTableColumnRowCount++;  break;
    case AC_InsertRow:           gpAttachMap->m_oStatistics.Api.m_nInsertRow++;            break;
    case AC_IsInvalidSession:    gpAttachMap->m_oStatistics.Api.m_nIsInvalidSession++;     break;
    case AC_NewSession:          gpAttachMap->m_oStatistics.Api.m_nNewSession++;           break;
    case AC_OpenPool:            gpAttachMap->m_oStatistics.Api.m_nPoolOpen++;             break;
    case AC_ScanNextRow:         gpAttachMap->m_oStatistics.Api.m_nScanNextRow++;          break;
    case AC_TranslateError:      gpAttachMap->m_oStatistics.Api.m_nTranslateError++;       break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Api::vMonitorPortrait(s48K& sPortrait)
{
  s48K s;
  i8 nCats = 0;
  i8 nBreak = 80;
  i8 iPrev = 0;
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  
  if (pStats->Api.m_nBeginInsert) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginInsertTable %lld "_Stz,pStats->Api.m_nBeginInsert);
  }
  if (pStats->Api.m_nBeginSubInserter) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginSubInserter %lld "_Stz,pStats->Api.m_nBeginSubInserter);
  }
  if (pStats->Api.m_nCheckVersion) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("checkVersion %lld "_Stz,pStats->Api.m_nCheckVersion);
  }
  if (pStats->Api.m_nCopyBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("copyBuffer %lld "_Stz,pStats->Api.m_nCopyBuffer);
  }
  if (pStats->Api.m_nDeleteSession) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("deleteSession %lld "_Stz,pStats->Api.m_nDeleteSession);
  }
  if (pStats->Api.m_nEndInsert) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("endInsert %lld "_Stz,pStats->Api.m_nEndInsert);
  }
  if (pStats->Api.m_nEndJobs) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("endJobs %lld "_Stz,pStats->Api.m_nEndJobs);
  }
  if (pStats->Api.m_nEndSubInserter) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("endSubInserter %lld "_Stz,pStats->Api.m_nEndSubInserter);
  }
  if (pStats->Api.m_nExecuteCommand) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("executeCommand %lld "_Stz,pStats->Api.m_nExecuteCommand);
  }
  if (pStats->Api.m_nFetchNextBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("fetchNextBuffer %lld "_Stz,pStats->Api.m_nFetchNextBuffer);
  }
  if (pStats->Api.m_nGetLoadResult) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getLoadResult %lld "_Stz,pStats->Api.m_nGetLoadResult);
  }
  if (pStats->Api.m_nGetRecordBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRecordBuffer %lld "_Stz,pStats->Api.m_nGetRecordBuffer);
  }
  if (pStats->Api.m_nGetRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRow %lld "_Stz,pStats->Api.m_nGetRow);
  }
  if (pStats->Api.m_nGetRowCount) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRowCount %lld "_Stz,pStats->Api.m_nGetRowCount);
  }
  if (pStats->Api.m_nGetTableColumnRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getTableColumnRow %lld "_Stz,pStats->Api.m_nGetTableColumnRow);
  }
  if (pStats->Api.m_nGetTableColumnRowCount) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("GetTableColumnRowCount %lld "_Stz,pStats->Api.m_nGetTableColumnRowCount);
  }
  if (pStats->Api.m_nInsertRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("insertRow %lld "_Stz,pStats->Api.m_nInsertRow);
  }
  if (pStats->Api.m_nIsInvalidSession) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("isInvalidSession %lld "_Stz,pStats->Api.m_nIsInvalidSession);
  }
  if (pStats->Api.m_nNewSession) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("newSession %lld "_Stz,pStats->Api.m_nNewSession);
  }
  if (pStats->Api.m_nPoolOpen) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("poolOpen %lld "_Stz,pStats->Api.m_nPoolOpen);
  }
  if (pStats->Api.m_nScanNextRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("scanNextRow %lld "_Stz,pStats->Api.m_nScanNextRow);
  }
  if (pStats->Api.m_nTranslateError) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("translareError %lld "_Stz,pStats->Api.m_nTranslateError);
  }

  if (nCats) {
    sPortrait.vCopy("Api: "_Stz);
    sPortrait.vConcat(s);
    sPortrait.vConcat("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
bool Api::bSpawnJob(Session* pSession, Job* pJob, eSS eSeed)
{
  //----------------------------------------------------------------------------
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSession)
      vWarn("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    vWarn("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    return false;
  }
  
  //----------------------------------------------------------------------------
  
  Task* pNewTask = new Task(pJob);
  
  // form the word to execute for the request
  pJob->vSeed(eSeed);
  
  pNewTask->vSession(pSession);
  
  Handlers::vSpawnedTask(pNewTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* Spawn amd wait for job to complete                                         */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJobAndWait(Session* pSession, Job* pJob)
{
  i8 iJobSeq = pJob->m_iJobSeq;
  
  // lock this job's mutex
  pJob->vMutexLock();
  //  i8 rCode = pthread_mutex_lock(&pJob->m_xThreadMutex);
  //  if (0 != rCode)
  //    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  
  ApiState::vMarkLocation(pSession,106);
  
  // set the job state to be locked
  pJob->m_eState = JS_LOCKED;
  
  // spawn the insertTable job
  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob,SS_SEED_REQ));
  ApiState::vMarkLocation(pSession,107);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for condition & state
  
  // set the job state to be waiting
  pJob->m_eState = JS_WAITING;
  
  // wait till signalled
  do {
    
    RASSERT( _bIsValidJob(pJob->eClass()) );
    RASSERT( iJobSeq == pJob->m_iJobSeq );
    
    //    vTrace("ThreadCond:\n"_Stz);
    //    MemLib::vHexDump(&pJob->m_xThreadCond,  sizeof(pthread_cond_t));
    //
    //    vTrace("ThreadMutex:\n"_Stz);
    //    MemLib::vHexDump(&pJob->m_xThreadMutex, sizeof(pthread_mutex_t));
    
    pJob->vMutexWait();
    //    rCode = pthread_cond_wait(&pJob->m_xThreadCond,
    //                              &pJob->m_xThreadMutex);
    //    if (0 != rCode)
    //      vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
    
    RASSERT( _bIsValidJob(pJob->eClass()) );
    RASSERT( iJobSeq == pJob->m_iJobSeq );
    
    
  } while (pJob->m_eState != JS_SIGNALLED);
  
  RASSERT( _bIsValidJob(pJob->eClass()) );
  RASSERT( iJobSeq == pJob->m_iJobSeq );
  
  ApiState::vMarkLocation(pSession,108);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // set the job state back to running
  pJob->m_eState = JS_RUNNING;
  
  // unlock this job's mutex
  pJob->vMutexUnlock();
  //  rCode = pthread_mutex_unlock(&pJob->m_xThreadMutex);
  //  if (0 != rCode)
  //    vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* validate the SE session                                                    */
/*----------------------------------------------------------------------------*/
eEC Api::eValidateSession(i8 iTrace, Session* pSession, Stz* aApiCall)
{
  
  // pSession cannot be null
  if (pSession == nullptr) {
    TRACE_ERROR("va", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSession should be a pool address
  if (!gpBufferPool->bValidAddress(pSession)) {
    TRACE_ERROR("vb", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSession should be an SESSION class
  if (pSession->eClass() != CN_SESSION) {
    TRACE_ERROR("vc", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
