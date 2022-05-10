/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Api.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Op.hpp"
#include "LogicUnit.hpp"
#include "Procedure.hpp"
#include "Output.hpp"
#include "EventHandlers.hpp"
#include "Grid.hpp"

#include "TagConstants.h"

#include "SP_TagSet.hpp"

extern AttachMap*   gpAttachMap;
extern BufferPool*  gpBufferPool;

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* alloc a new job                                                            */
/*----------------------------------------------------------------------------*/
Job* Api::pAllocJob(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId)
{
  _static_
  
  Job* pJob = new Job(szJobNm,tParentId,tOriginId,tDistId);

  return pJob;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fork a child job for given command                                         */
/*----------------------------------------------------------------------------*/
eEC Api::eForkJob(JobId& rtChildJobId, Job* pParentJob,
                  TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                  Job*& rpChildJob_tbr)
{
  _static_
  
  rtChildJobId = C_UNKNOWN;
  
  //Job* pChildJob = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prepare job

  CmdHelper::vExtractCmdPaths(pCmdTknVec,pCmdTagSet);
  
  bool b = CmdHelper::bExtractCmdTags(pCmdTknVec,pCmdTagSet);
  UNUSED(b);
  
  sSTMT sMaybePipelineSmt;
  if (Procedure::bTranslateToPipeline(sMaybePipelineSmt.pString(),pCmdTknVec)) {
    pCmdTknVec->vSoftReset();
    pCmdTknVec->vTokeniseSen(sMaybePipelineSmt);
  }
        
  if (! CmdHelper::bParseCmd(pCmdTknVec,pCmdTagSet)) {
    rtChildJobId = C_UNKNOWN;
    return EC_INVALID_COMMAND;
  }

  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify minimum needed tags
  
  if (! pCmdTagSet->bHasName(C_TAG_ENTRYPOINT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_ENTRYPOINT"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_LASTJOBID)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_LASTJOBID"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_JOBKIND)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_JOBKIND"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_PATH"_Stz);
  }
  
  if ((! pCmdTagSet->bHasName(C_TAG_NOWAIT)) &&
      (! pCmdTagSet->bHasName(C_TAG_WAIT))) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: neither C_TAG_NOWAIT or C_TAG_WAIT"_Stz);
  }
  if (pCmdTagSet->bHasName(C_TAG_NOWAIT) &&
      pCmdTagSet->bHasName(C_TAG_WAIT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: both C_TAG_NOWAIT and C_TAG_WAIT"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // alloc job
  
  JobId tParentId;
  if (pParentJob)
    tParentId = pParentJob->tJobId();
  
  JobId tOriginId;
  if (pParentJob)
    tOriginId = pParentJob->tOriginId();
  
  JobId tDistId;
  if (pParentJob)
    tDistId = pParentJob->tDistId();

  rpChildJob_tbr = Api::pAllocJob(szJobNm,tParentId,tOriginId,tDistId);
  rtChildJobId = rpChildJob_tbr->tJobId();

  TagSet* pJobTagSet = rpChildJob_tbr->pJobTags();
  pJobTagSet->vCopy(pCmdTagSet);
  
  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  rpChildJob_tbr->vCmd(sCmd);

  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  //vTrace("szEntry: %s\n"_Stz,szEntry);
  rpChildJob_tbr->vEntryPoint(szEntry);

  eJK eJobKind = (eJK) pJobTagSet->iValue(C_TAG_JOBKIND);
  rpChildJob_tbr->vJobKind(eJobKind);

  
  Stz* szFolderPath = pJobTagSet->szValue(C_TAG_PATH);
  rpChildJob_tbr->vFolderPath(szFolderPath);

  JobId tLastJobId;
  tLastJobId = pJobTagSet->iValue(C_TAG_LASTJOBID);
  rpChildJob_tbr->vLastJobId(tLastJobId);

  bool m_bSystemCloser = pJobTagSet->iValue(C_TAG_SYSTEMCLOSER);
  rpChildJob_tbr->vSystemCloser(m_bSystemCloser);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fork
  
  rpChildJob_tbr->vMutexCreate();
  rpChildJob_tbr->vOutputCreate();
  rpChildJob_tbr->vOutputBeginProc();

  rpChildJob_tbr->vOutputEnabled(true);

  //JobId tJobId = pJob->tJobId();
 
  rpChildJob_tbr->vStmtListCreate();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 iPid = AM::PROCESS::iSePid();
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    vTrace("Cannot find pid %lld\n"_Stz, iPid);
    rtChildJobId = C_UNKNOWN;
    return EC_SE_NOT_RUNNING;
  }

  RASSERT(! rpChildJob_tbr->pTask());

  rpChildJob_tbr->vMutexLock();
  RASSERT(MS_LOCKED == rpChildJob_tbr->eMutexSide());
  
  rpChildJob_tbr->vTaskCreate();

  rpChildJob_tbr->vMutexUnlock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (;;) {
  
    i8 iPid = AM::PROCESS::iSePid();
    if (!Process::bTestPid(iPid)) {
      // SE is not running anymore!
      vTrace("Cannot find pid %lld\n"_Stz, iPid);
      rtChildJobId = C_UNKNOWN;
      return EC_SE_NOT_RUNNING;
    }
    
    
    rpChildJob_tbr->vMutexLock();
    RASSERT(MS_LOCKED == rpChildJob_tbr->eMutexSide());
    
    RASSERT(rpChildJob_tbr->pTask());

    //rpChildJob_tbr->vTaskCreate();
    
    rpChildJob_tbr->vResponseTimerStart();
    
    Scheduler::vTaskApi_AddPrompt(rpChildJob_tbr->pTask());

    if (rpChildJob_tbr->bHasTag(C_TAG_NOWAIT)) {
      RASSERT(! rpChildJob_tbr->bHasTag(C_TAG_WAIT));
      rpChildJob_tbr->vMutexUnlock();
      return EC_NONE;
    }
    
    RASSERT(rpChildJob_tbr->bHasTag(C_TAG_WAIT));
    RASSERT(! rpChildJob_tbr->bHasTag(C_TAG_NOWAIT));


    rpChildJob_tbr->vMutexWait();

    rpChildJob_tbr->vResponseTimerUpdate();
    
    rpChildJob_tbr->vMutexUnlock();

    rpChildJob_tbr->pTask()->vCycleTaskLock((Stz*)__func__);

    if (! rpChildJob_tbr->bHasStmtList()) {
      return EC_NONE;
    }
          
    rpChildJob_tbr->vCascadeReset();
    
    sSTMT sStmt;
    rpChildJob_tbr->vStmtListPop(sStmt);
    rpChildJob_tbr->vCmd(sStmt);
    
    TagSet* pJobTagSet = rpChildJob_tbr->pJobTags();
    //Stz* szEntry = pCmdTagSet->szValue(C_TAG_ENTRYPOINT);
    Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
    //vTrace("szEntry: %s\n"_Stz,szEntry);

    rpChildJob_tbr->vEntryPoint(szEntry);

    //AM::pHeaps()->vSetTraceFree(false);
  }
}

/*----------------------------------------------------------------------------*/
/* new a generic job        THIS IS TEMP REFACTOR                             */
/*----------------------------------------------------------------------------*/
Job* Api::pNewGenericJob(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId)
{
  _static_
  
  Job* pJobGeneric = new Job(szJobNm,tParentId,tOriginId,tDistId);
  
  pJobGeneric->vParticipantNodeId(AM::pGrid()->iThisNodeId());
  pJobGeneric->vJobKind(JK_GENERIC);

  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(szJobNm);
  TagSet*   pCmdTagSet = new TagSet;

  if (! CmdHelper::bParseCmd(pCmdTknVec,pCmdTagSet)) {
    delete pCmdTknVec;
    delete pCmdTagSet;
    return nullptr;
  }

  Stz* szJobNm_tag = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm_tag && szJobNm_tag->bSame(szJobNm));

  TagSet* pJobTagSet = pJobGeneric->pJobTags();
  pJobTagSet->vCopy(pCmdTagSet);
  
  
  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  pJobGeneric->vEntryPoint(szEntry);


  delete pCmdTknVec;
  delete pCmdTagSet;
  return pJobGeneric;
}

/*----------------------------------------------------------------------------*/
/* set job as reapable                                                        */
/*----------------------------------------------------------------------------*/
void Api::vReapableJob(Job*& rpJob)
{
  _static_
      
  if (rpJob && rpJob->bIsPipeline() && ! rpJob->pOutput()->m_bExitMarker)
    rpJob->pOutput()->vProcessBufferQueue(rpJob);

  rpJob->pOutput()->m_pOutputQueue->vReset();
  rpJob->pOutput()->vEndProcOneCmd();
  
  if (rpJob->pOutput()->m_pStreamDef_result) {
    StreamDef* pStreamDef = rpJob->pOutput()->m_pStreamDef_result;
    rpJob->pOutput()->m_pStreamDef_result = nullptr;
    delete pStreamDef;
  }
  
  rpJob->pOutput()->m_bExitMarker      = false;
  rpJob->pOutput()->m_bExitEarlyMarker = false;
  
  rpJob->vReapable(true);

  rpJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
void Api::vReapAllJobs(bool bForce)
{
  _static_
  
  if (bForce || AM::SYSTEM::bReapEnabled())
    while (AM::pRetiredJobs()->nJobs())
      while (Api::bReapJob())
        {;}
}

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
void Api::vReapJobsOnce(bool bForce)
{
  _static_
  
  if (bForce || AM::SYSTEM::bReapEnabled())
    while (Api::bReapJob())
      {;}
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
bool Api::bReapJob(void)
{
  _static_
   
  JobList* pRetiredJobs = AM::pRetiredJobs();

  if (! pRetiredJobs->nJobs()) {
    //vTrace("Empty retired list\n"_Stz);
    return FALSE(empty_list);
  }
  
  AM::MASTERLOCK::vGrabReap(nullptr);
  
  pRetiredJobs->vGrabLock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pReapableJob = nullptr;
  
  for (SkipNode* pCursor = pRetiredJobs->pFirst();
       pCursor;
       pCursor = pRetiredJobs->pNext(pCursor)) {
    
    Job* pRetiredJob = _pJob(pCursor->m_pObject);
    
    if (pRetiredJob) {
      i8 nActiveOps = pRetiredJob->nActiveOps();
//      vTrace("REAP CANDIDATE Job %p Reapable %lld Active Ops %lld\n"_Stz
//             ,pRetiredJob,(i8)pRetiredJob->m_bReapable,nActiveOps);
      if (pRetiredJob->bReapable() && ! nActiveOps) {
        pReapableJob = pRetiredJob;
        break;
      }
      else {
        vTrace("Cant reap retired job %p %lld "
               "%s %s "
               "activeOps %lld %s "
               "\n"_Stz

               ,pRetiredJob,pRetiredJob->tJobId().uJobNo()
               ,aJK[pRetiredJob->eJobKind()],pRetiredJob->szJobNm()
               ,nActiveOps,pRetiredJob->bReapable() ? "reapable" : "not-reapable"
               );
      }
    }
  }

  pRetiredJobs->vDropLock();
  
  if (! pReapableJob) {
    AM::MASTERLOCK::vDropReap(nullptr);
    return FALSE(nothing_to_reap);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pRetiredJobs->vRemoveJob(pReapableJob);

  AM::MASTERLOCK::vDropReap(nullptr);

  // delete the reapavle job
  
  RASSERT(pReapableJob->pTask());
  pReapableJob->pTask()->vGrabTaskLock((Stz*)__func__);
  pReapableJob->pTask()->vDropTaskLock((Stz*)__func__);


  delete pReapableJob;

  return TRUE(reaped_a_job);
}

/*----------------------------------------------------------------------------*/
/* populate tagset from given token vector                                    */
/*----------------------------------------------------------------------------*/
void Api::vPopulateTagSet(TagSet* pTagSet, TokenVec* pTknVec)
{
  _static_
  
  i8 nTkns = pTknVec->nTokens();

  i8 i_Scan = 0;
  while (i_Scan < nTkns) {
    
    Token* pTkn_name = pTknVec->pGet(i_Scan++);    // <tagNm>
    if (i_Scan >= nTkns)
      break;
    if (TN_TAG != pTkn_name->eToken())
      break;

    Token* pTkn_equals = pTknVec->pGet(i_Scan++);  // =
    UNUSED(pTkn_equals);
    
    if (i_Scan >= nTkns)
      break;
    //if (! pTkn_equals->bIsAssign())
    //  break;

    Token* pTkn_value = pTknVec->pGet(i_Scan++);   // value
    
    pTagSet->vReplace(pTkn_name->szChars(),pTkn_value->szChars());
  }

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inc statistics                                                             */
/*----------------------------------------------------------------------------*/
void Api::vIncStatistics(eAC eApiCall)
{
  _static_
  
  
  if (! AM::bAttached())
    return;
  
  Statistics* pStats = AM::pStatistics();

  switch (eApiCall) {
    case AC_BeginFetch:          pStats->Api.m_nBeginFetch++;          break;
    case AC_BeginInsert:         pStats->Api.m_nBeginInsert++;          break;
    case AC_BeginSubInserter:    pStats->Api.m_nBeginSubInserter++;     break;
    case AC_CheckVersion:        pStats->Api.m_nCheckVersion++;         break;
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
/* call the system boot job to compile the main dictionary                    */
/*----------------------------------------------------------------------------*/
void Api::vCallBootSystemJob(void)
{
  _static_
  
  JobId tParentId;
  RASSERT(tParentId.bIsUnknown());
  
  JobId tOriginId;
  RASSERT(tOriginId.bIsUnknown());

  JobId tDistId;
  RASSERT(tDistId.bIsUnknown());

  Job* pBootJob = new Job("bootSystem"_Stz,tParentId,tOriginId,tDistId);
  
  Stz* szJobNm = pBootJob->szJobNm();
  Task* pBootTask = new Task(szJobNm,pBootJob);
  pBootJob->vTask(pBootTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     
  pBootTask->vAddAxioms();
  pBootTask->vAddMunged();
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("[[mainThread]] Compiling dictionaries...\n"_Stz);
  
  pBootTask->vSetInxPtr("bootSystem::bootSystem"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TagSet* pJobTags = pBootJob->pJobTags();
  //pJobTags->vDumpSet();

  // (no cmd to parse)

  pJobTags->vReplace(C_TAG_PATH,"/"_Stz);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pBootJob->vResponseTimerStart();
  vCallTaskImmediate(pBootTask);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pBootTask->vState(TS_RETIRED);
  pBootTask->vDropTaskLock((Stz*)__func__);

  Api::vReapJobsOnce();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::HANDLERS::pEventHandlers()->vInitialise();
}

/*----------------------------------------------------------------------------*/
/* call system start job to define procedures & internal tables, spawn demons */
/*----------------------------------------------------------------------------*/
void Api::vCallStartSystemJob(void)
{
  _static_

  // this is the main thread
  PsxThd::vMarkNonSeThread();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  JobId tParentId;
  RASSERT(tParentId.bIsUnknown());
  
  JobId tOriginId;
  RASSERT(tOriginId.bIsUnknown());

  JobId tDistId;
  RASSERT(tDistId.bIsUnknown());

  Job* pStartJob = new Job("startSystem"_Stz,tParentId,tOriginId,tDistId);
  
  Stz* szJobNm = pStartJob->szJobNm();
  Task* pStartTask = new Task(szJobNm,pStartJob);
  pStartTask->vIsStart(true);

  pStartJob->vTask(pStartTask);
  
  pStartTask->vSetInxPtr("startSystem::startSystem"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  TagSet* pJobTags = pStartJob->pJobTags();
  //pJobTags->vDumpSet();

  // (no cmd to parse)

  pJobTags->vReplace(C_TAG_PATH,"/"_Stz);
  pJobTags->vReplace(C_TAG_WAIT,1);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pStartJob->vResponseTimerStart();
  vCallTaskImmediate(pStartTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pStartTask->vState(TS_RETIRED);
  pStartTask->vDropTaskLock((Stz*)__func__);
  
  Api::vReapJobsOnce();
}

/*----------------------------------------------------------------------------*/
/* run task immediately                                                       */
/*----------------------------------------------------------------------------*/
void Api::vCallTaskImmediate(Task* pImmediateTask)
{
  _static_
  
  Task* pSaveTask = _pTask(PsxThd::pTlsGet(giTLS_TaskKey));

  pImmediateTask->vIsImmediate(true);
  
  pImmediateTask->vState(TS_EXECUTING);
  
  pImmediateTask->vTaskLoop();
  
  PsxThd::vTlsSet(giTLS_TaskKey,pSaveTask);
}

/*----------------------------------------------------------------------------*/
/* spawn task for given op                                                    */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTask_op(Op* pOp)
{
  _static_
  
  vCreateTaskForOp(pOp);
  
  Scheduler::vTaskApi_AddOp(pOp->pTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTask_dedicatedOp(Op* pOp)
{
  _static_
  
  //RASSERT(! pJob->pTask());
  
  vCreateTaskForOp(pOp);
  Scheduler::vTaskApi_AddDedicated(pOp->pTask());
}


/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTask_receivedJob(Job* pJob)
{
  _static_
  
  pJob->vTaskCreate();
  Scheduler::vTaskApi_AddReceived(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* schedule next phase of participant                                         */
/*----------------------------------------------------------------------------*/
void Api::vScheduleNextPhase(Job* pParticipantJob, Task* pActiviating2PcTask)
{
  _static_
  
  vResetTaskForJob(pParticipantJob);
  
  Task* pTaskParticipant = pParticipantJob->pTask();
  Scheduler::vTaskCross_ScheduleNextPhase(pTaskParticipant, pActiviating2PcTask);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* reset task for given job                                                   */
/*----------------------------------------------------------------------------*/
void Api::vResetTaskForJob(Job* pJob)
{
  _static_
    
  Stz* szEntry = "job::entryPoint"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Task* pTask = pJob->pTask();
    
  pTask->vSetInxPtr(szEntry);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pJob->vResponseTimerStart();
}

/*----------------------------------------------------------------------------*/
/* create task for given op                                                   */
/*----------------------------------------------------------------------------*/
void Api::vCreateTaskForOp(Op* pOp)
{
  _static_
    
  Task* pTask = pOp->pTask();
  RASSERT(! pTask);

  Word* pEntryWord = pOp->pEntryWord();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pTask = new Task(pOp->szOpNm(),pOp);

  pOp->vTask(pTask);

  pTask->vSetInxPtr(pEntryWord);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
