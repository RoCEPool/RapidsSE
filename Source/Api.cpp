/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "Interface.h"
#include "AM.hpp"

extern BufferPool*  gpBufferPool;
extern Log* gpLog;


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

  SessionState* pState = pSession->m_pSessionState;

  while (pJobStack->bPop(pJobObject)) {
    
    Job* aJob = (Job*)pJobObject;
    
    // find the job class on this node
    eCN eClass = aJob->eClass();
    JobClass* pJobClass = JobClass::pFind(eClass);
    
    ApiState::vMarkApiLoc(pSession,200);
    
    //vTrace("API deleting job %p jobId %lld %s\n"_Stz, aJob, aJob->m_iUniqueId, aCN[eClass]);
    
    // If we come across a JOB_FETCHROWS then we need to empty its queues
    if (eClass == CN_JOB_FETCHROWS) {
      Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(aJob);
      
      //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pTable->sTable(),
      //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
      
      // if we had an entry buffer delete it
      pJob_FetchRows->vDeleteBuffer();
      
      while (pJob_FetchRows->m_nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers) {
        // pop an entryBuffer off the queue
        ApiState::vMarkApiLoc(pSession,300);
        
        FetchBuffer* pFetchBuffer;
        for (;;) {
          pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
          if (pFetchBuffer)
            break;
          AM::vNap(1,CN_JOB_FETCHROWS);
          ApiState::vMarkApiLoc(pSession,400);
          continue;
        }
        ApiState::vMarkApiLoc(pSession,500);
        
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
    ApiState::vMarkApiLoc(pSession,600);
    
    // delete the job
    //vTrace("Deleting job %p %s\n"_Stz, aJob, aCN[aJob->eClass()]);
    pJobClass->vDeleteJob(pSession,aJob);
    
    pState->m_nJobDelete++;
  }
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
Job* Api::pExecuteCommand(Session* pSession, Stz* sCommand,
                          bool bPostJob, bool bFromPrompt)
{
  Input* pInput = pSession->m_pInput;
  
  ApiState::vMarkApiLoc(pSession,101);
  
//  bool bFound = pInput->bFindJobClass();
//  if (! bFound) {
//    return nullptr;
//  }
//  JobClass* pJobClass = pInput->pJobClass();
  
  pInput->vTokenise(sCommand);
  Stz* aCommand1 = pInput->pTkn(0)->szText();
  Stz* aCommand2 = pInput->pTkn(1)->szText();
  
  JobClass* pJobClass = JobClass::pFind(aCommand1, aCommand2);
  if (! pJobClass)
    return nullptr;
  
  ReqTuple xReqTuple("!!EXECUTE_COMMAND"_Stz,false);
  
  Job* pJob = pJobClass->pPreJob(&xReqTuple,pSession);
  if (! pJob) {
    return nullptr;
  }
  
  if (pJob->m_bHasError) {
    //vTrace("job has error (maybe syntax error)\n"_Stz);
    //pJobClass->vPostJob();
    return pJob;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn and wait for job
  
  Api::vSpawnJobAndWait(pSession, pJob);
  
  // run post ?
  if (bPostJob)
    pJobClass->vPostJob(pSession,pJob);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Api::vIncStatistics(eAC eApiCall)
{
  if (! AM::bAttached())
    return;
  
  Statistics* pStats = AM::pStatistics();

  switch (eApiCall) {
    case AC_BeginFetch:          pStats->Api.m_nBeginFetch++;          break;
    case AC_BeginInsert:         pStats->Api.m_nBeginInsert++;          break;
    case AC_BeginSubInserter:    pStats->Api.m_nBeginSubInserter++;     break;
    case AC_CheckVersion:        pStats->Api.m_nCheckVersion++;         break;
    case AC_CopyBuffer:          pStats->Api.m_nCopyBuffer++;           break;
    case AC_DeleteSession:       pStats->Api.m_nDeleteSession++;        break;
    case AC_EndInsert:           pStats->Api.m_nEndInsert++;            break;
    case AC_EndJobs:             pStats->Api.m_nEndJobs++;              break;
    case AC_EndSubInserter:      pStats->Api.m_nEndSubInserter++;       break;
    case AC_ExecuteCommand:      pStats->Api.m_nExecuteCommand++;       break;
    case AC_FetchNextBuffer:     pStats->Api.m_nFetchNextBuffer++;      break;
    case AC_FetchTable:          pStats->Api.m_nFetchTable++;           break;
    case AC_GetFirstBuffer:      pStats->Api.m_nGetFirstBuffer++;       break;
    case AC_GetLoadResult:       pStats->Api.m_nGetLoadResult++;        break;
    case AC_GetNextBuffer:       pStats->Api.m_nGetNextBuffer++;        break;
    case AC_GetRecordBuffer:     pStats->Api.m_nGetRecordBuffer++;      break;
    case AC_GetRow:              pStats->Api.m_nGetRow++;               break;
    case AC_GetRowCount:         pStats->Api.m_nGetRowCount++;          break;
    case AC_GetTableColumnRow:   pStats->Api.m_nGetTableColumnRow++;    break;
    case AC_GetTableColumnRowCount: pStats->Api.m_nGetTableColumnRowCount++;  break;
    case AC_InsertRow:           pStats->Api.m_nInsertRow++;            break;
    case AC_IsInvalidSession:    pStats->Api.m_nIsInvalidSession++;     break;
    case AC_NewSession:          pStats->Api.m_nNewSession++;           break;
    case AC_NextRow:             pStats->Api.m_nNextRow++;              break;
    case AC_OpenPool:            pStats->Api.m_nPoolOpen++;             break;
    case AC_TranslateError:      pStats->Api.m_nTranslateError++;       break;
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
  
  Statistics* pStats = AM::pStatistics();

  if (pStats->Api.m_nBeginFetch) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginFetch %lld "_Stz,pStats->Api.m_nBeginFetch);
  }
  if (pStats->Api.m_nBeginInsert) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginInsert %lld "_Stz,pStats->Api.m_nBeginInsert);
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
  if (pStats->Api.m_nFetchTable) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("fetchTable %lld "_Stz,pStats->Api.m_nFetchTable);
  }
  if (pStats->Api.m_nGetFirstBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getFirstBuffer %lld "_Stz,pStats->Api.m_nGetFirstBuffer);
  }
  if (pStats->Api.m_nGetNextBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getNextBuffer %lld "_Stz,pStats->Api.m_nGetNextBuffer);
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
  if (pStats->Api.m_nNextRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("nextRow %lld "_Stz,pStats->Api.m_nNextRow);
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
  ASSERT(pJob->m_pJobClass);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = AM::iPid();
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSession)
      vWarn("Cannot find pid %lld\n"_Stz, iPid);
    vWarn("Cannot find pid %lld\n"_Stz, iPid);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // form the word to execute for the request
  pJob->vSeed(eSeed);
  
  //if (pJob->m_xReqTuple.m_bMarkTrace)
  //  pJob->m_pJobClass->vShowJob(pJob);
  
  Task* pNewTask = new Task(pJob,pSession);
  pNewTask->vState(TS_ABLE);
  //pNewTask->vSession(pSession);
  
  AM::vLogSsn(SL_INF,pSession,AA_API_SPAWN_JOB,
              "pJob=%p"_Stz, //!format-LogPrm
              pJob,
              C_ENDARGS);


  Scheduler::vAddTaskGeneral(pNewTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* Spawn amd wait for job to complete                                         */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJobAndWait(Session* pSession, Job* pJob)
{
  // lock this job's mutex
  pJob->vLockJobMutex();
  //  i8 rCode = pthread_mutex_lock(&pJob->m_xThreadMutex);
  //  if (0 != rCode)
  //    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  
  ApiState::vMarkApiLoc(pSession,106);
  
  // set the job state to be locked
  pJob->m_eJobState = JS_LOCKED;
  
  // spawn the insertTable job
  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob,SS_SEED_REQ));
  ApiState::vMarkApiLoc(pSession,107);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for condition & state
  
  // set the job state to be waiting
  pJob->m_eJobState = JS_WAITING;
  
  // wait till signalled
  do {
    
    RASSERT( _bIsValidJob(pJob->eClass()) );
    
    //    vTrace("ThreadCond:\n"_Stz);
    //    MemLib::vHexDump(&pJob->m_xThreadCond,  sizeof(pthread_cond_t));
    //
    //    vTrace("ThreadMutex:\n"_Stz);
    //    MemLib::vHexDump(&pJob->m_xThreadMutex, sizeof(pthread_mutex_t));
    
    pJob->vWaitJobMutex();
    //    rCode = pthread_cond_wait(&pJob->m_xThreadCond,
    //                              &pJob->m_xThreadMutex);
    //    if (0 != rCode)
    //      vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
    
    RASSERT( _bIsValidJob(pJob->eClass()) );
    
    
  } while (pJob->m_eJobState != JS_SIGNALLED);
  
  RASSERT( _bIsValidJob(pJob->eClass()) );
  
  ApiState::vMarkApiLoc(pSession,108);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // set the job state back to running
  pJob->m_eJobState = JS_RUNNING;
  
  // unlock this job's mutex
  pJob->vUnlockJobMutex();
  //  rCode = pthread_mutex_unlock(&pJob->m_xThreadMutex);
  //  if (0 != rCode)
  //    vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* spawn dedicated job                                                        */
/*----------------------------------------------------------------------------*/
bool Api::bSpawnJobDedicated(Session* pSession, Job* pJob, eSS eSeed)
{
  ASSERT(pJob->m_pJobClass);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = AM::iPid();
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSession)
      vWarn("Cannot find pid %lld\n"_Stz, iPid);
    vWarn("Cannot find pid %lld\n"_Stz, iPid);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  // form the word to execute for the request
  pJob->vSeed(eSeed);
  
  
  //if (pJob->m_xReqTuple.m_bMarkTrace)
  //  pJob->m_pJobClass->vShowJob(pJob);
  
  Task* pNewTask = new Task(pJob,pSession);
  pNewTask->vState(TS_ABLE);
  //pNewTask->vSession(pSession);
  
  Scheduler::vAddTaskDedicated(pNewTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* spawn operator                                                              */
/*----------------------------------------------------------------------------*/
bool Api::bSpawnOp(Session* pSession, Op* pOp)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = AM::iPid();
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSession)
      vWarn("Cannot find pid %lld\n"_Stz, iPid);
    vWarn("Cannot find pid %lld\n"_Stz, iPid);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pNewTask = new Task(pOp,pSession);
  pNewTask->vState(TS_ABLE);
  pOp->vTask(pNewTask);
  Scheduler::vAddTaskGeneral(pNewTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* validate the SE session                                                    */
/*----------------------------------------------------------------------------*/
eEC Api::eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall)
{
  
  // pSession cannot be null
  if (pSession == nullptr) {
    TRACE_API_ERROR("eValidateSession(a)", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSession should be a pool address
  if (!gpBufferPool->bValidAddress(pSession)) {
    TRACE_API_ERROR("eValidateSession(b)", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSession should be an SESSION class
  if (pSession->eClass() != CN_SESSION) {
    TRACE_API_ERROR("eValidateSession(c)", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
