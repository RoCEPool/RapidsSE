/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Api.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Jobs.hpp"
#include "Op.hpp"
#include "LogicUnit.hpp"
#include "Procedure.hpp"
#include "Output.hpp"
#include "EventHandlers.hpp"
#include "Grid.hpp"
#include "AttachMap.hpp"

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
/* delete root job                                                           */
/*----------------------------------------------------------------------------*/
void Api::vDeleteRootJob(Job*& rpRootJob)
{
  _static_
        
  rpRootJob->vReapable(true);
  delete rpRootJob;

  rpRootJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* fork a child job for given command                                         */
/*----------------------------------------------------------------------------*/
eEC Api::eForkJobChild(JobId& rtChildJobId,
                        TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                        JobId tParentId, JobId tOriginId, JobId tDistId,
                        Job*& rpParentJob_tbr,
                        bool& bIsPipeline,
                        bool& bIsBackground)
{
  _static_

  Job* pChildJob;
  rtChildJobId = C_UNKNOWN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // should have a parent (deputy root job)

  RASSERT(! tParentId.bIsUnknown());
  RASSERT(rpParentJob_tbr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prepare the new job

  CmdHelper::vExtractCmdPaths(pCmdTagSet,pCmdTknVec);

  bool b = CmdHelper::bExtractCmdTags(pCmdTagSet,pCmdTknVec);
  UNUSED(b);

  sSTMT sMaybePipelineSmt;
  if (Procedure::bTranslateToPipeline(sMaybePipelineSmt.pString(),pCmdTknVec)) {
    pCmdTknVec->vSoftReset();
    pCmdTknVec->vTokeniseSen(sMaybePipelineSmt);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // parse the command

  if (! CmdHelper::bParseCmd(pCmdTknVec,pCmdTagSet)) {
    rtChildJobId = C_UNKNOWN;
    return EC_INVALID_COMMAND;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify tags

  // C_TAG_JOBNM
  if (! pCmdTagSet->bHasName(C_TAG_JOBNM)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_JOBNM"_Stz);
  }

  // C_TAG_ENTRYPOINT
  if (! pCmdTagSet->bHasName(C_TAG_ENTRYPOINT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_ENTRYPOINT"_Stz);
  }

  // C_TAG_PATH
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob: no C_TAG_PATH"_Stz);
  }

  // C_TAG_NOWAIT / C_TAG_WAIT
  if ((! pCmdTagSet->bHasName(C_TAG_NOWAIT)) &&
      (! pCmdTagSet->bHasName(C_TAG_WAIT))) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobOld: neither C_TAG_NOWAIT or C_TAG_WAIT"_Stz);
  }
  if (pCmdTagSet->bHasName(C_TAG_NOWAIT) &&
      pCmdTagSet->bHasName(C_TAG_WAIT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobOld: both C_TAG_NOWAIT and C_TAG_WAIT"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the child job

  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);


  pChildJob = new Job(szJobNm,tParentId,tOriginId,tDistId);
  bIsPipeline = pChildJob->bIsPipeline();

  //vTrace("FORKING CHILD: %p\n"_Stz, rpChildJob_tbr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy CmdTagSet to the child job tags

  TagSet* pJobTagSet = pChildJob->pJobTags();
  pJobTagSet->vCopy(pCmdTagSet);

  bIsBackground = pChildJob->bHasTag(C_TAG_BG);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // setup the child job

  // sCmd
  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  pChildJob->vCmd(sCmd);

  // szEntry
  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  pChildJob->vEntryPoint(szEntry);

  // szFolderPath
  Stz* szFolderPath = pJobTagSet->szValue(C_TAG_PATH);
  pChildJob->vFolderPath(szFolderPath);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the mutex in the child job
  //rpChildJob_tbr->vMutexCreate();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the task for the child job
  pChildJob->vTaskCreate();
  RASSERT(pChildJob->pTask());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock the mutex
  rpParentJob_tbr->vMutexLock();
  RASSERT(MS_LOCKED == rpParentJob_tbr->eMutexSide());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // start the timer of the child job
  pChildJob->vResponseTimerStart();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add task to scheduler
  Scheduler::vTaskApi_Add(pChildJob->pTask());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for signal from child job
  // if C_TAG_WAIT parent gets signalled when child job completes
  // if C_TAG_NOWAIT parent gets signalled when pretests/ compilation are done

  rpParentJob_tbr->vMutexWait();
  rpParentJob_tbr->vMutexUnlock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // update the timer of the child job
  pChildJob->vResponseTimerUpdate();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cycle the task lock ?????
  // NOT SURE????

  pChildJob->pTask()->vCycleTaskLock((Stz*)__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for error from child job
  eEC eChildError = rpParentJob_tbr->eChildError();

  if (eChildError != EC_NONE) {
    //rpChildJob_tbr->vJobError(eChildError);
    Api::vReapableJob(pChildJob);
    return eChildError;
  }

  // set some tags in the parent for future child jobs
  rpParentJob_tbr->vFolderPath(pChildJob->szFolderPath());
  rpParentJob_tbr->vLastJobId(pChildJob->tLastJobId());
  rpParentJob_tbr->vSystemCloser(pChildJob->bSystemCloser());

  // return the jobId
  rtChildJobId = pChildJob->tJobId();

  // mark new child job as reapable
  //Api::vReapableJob(rpChildJob_tbr);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* fork a child job for given command (to go)                                 */
/*----------------------------------------------------------------------------*/
eEC Api::eForkJobOld(JobId& rtChildJobId,
                  TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                  JobId tParentId, JobId tOriginId, JobId tDistId,
                  Job*& rpParentJob_tbr,
                  Job*& rpChildJob_tbr)
{
  _static_
/*
  rtChildJobId = C_UNKNOWN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assertions

  RASSERT(! tParentId.bIsUnknown());
  RASSERT(rpParentJob_tbr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prepare job

  CmdHelper::vExtractCmdPaths(pCmdTagSet,pCmdTknVec);

  bool b = CmdHelper::bExtractCmdTags(pCmdTagSet,pCmdTknVec);
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify minimum needed tags

  if (! pCmdTagSet->bHasName(C_TAG_JOBNM)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobOld: no C_TAG_JOBNM"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_ENTRYPOINT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobOld: no C_TAG_ENTRYPOINT"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobOld: no C_TAG_PATH"_Stz);
  }

  if ((! pCmdTagSet->bHasName(C_TAG_NOWAIT)) &&
      (! pCmdTagSet->bHasName(C_TAG_WAIT))) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobOld: neither C_TAG_NOWAIT or C_TAG_WAIT"_Stz);
  }
  if (pCmdTagSet->bHasName(C_TAG_NOWAIT) &&
      pCmdTagSet->bHasName(C_TAG_WAIT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJobOld: both C_TAG_NOWAIT and C_TAG_WAIT"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);

  rpChildJob_tbr = new Job(szJobNm,tParentId,tOriginId,tDistId);


  TagSet* pJobTagSet = rpChildJob_tbr->pJobTags();
  pJobTagSet->vCopy(pCmdTagSet);

  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  rpChildJob_tbr->vCmd(sCmd);

  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  rpChildJob_tbr->vEntryPoint(szEntry);

  Stz* szFolderPath = pJobTagSet->szValue(C_TAG_PATH);
  rpChildJob_tbr->vFolderPath(szFolderPath);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  rtChildJobId = rpChildJob_tbr->tJobId();

  rpChildJob_tbr->vMutexCreate();
  //rpChildJob_tbr->vOutputCreate();
  //rpChildJob_tbr->vOutputBeginProc();
  //rpChildJob_tbr->vOutputEnabled(true);
  rpChildJob_tbr->vStmtListCreate();
  rpChildJob_tbr->vTaskCreate();

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

    rpChildJob_tbr->vResponseTimerStart();

    Scheduler::vTaskApi_Add(rpChildJob_tbr->pTask());

    if (rpChildJob_tbr->bHasTag(C_TAG_NOWAIT)) {
      RASSERT(! rpChildJob_tbr->bHasTag(C_TAG_WAIT));
      rpChildJob_tbr->vMutexUnlock();
      return EC_NONE;
    }

    RASSERT(rpChildJob_tbr->bHasTag(C_TAG_WAIT));
    RASSERT(! rpChildJob_tbr->bHasTag(C_TAG_NOWAIT));


    rpChildJob_tbr->vMutexWait();
    rpChildJob_tbr->vMutexUnlock();

    rpChildJob_tbr->vResponseTimerUpdate();


    rpChildJob_tbr->pTask()->vCycleTaskLock((Stz*)__func__);

    if (! rpChildJob_tbr->bHasStmtList()) {
      return EC_NONE;
    }

    rpChildJob_tbr->vCascadeReset();

    sSTMT sStmt;
    rpChildJob_tbr->vStmtListPop(sStmt);
    rpChildJob_tbr->vCmd(sStmt);

    TagSet* pJobTagSet = rpChildJob_tbr->pJobTags();
    Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
    //vTrace("szEntry: %s\n"_Stz,szEntry);

    rpChildJob_tbr->vEntryPoint(szEntry);

    //AM::pHeaps()->vSetTraceFree(false);
  }
  */
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* fork a child job for given command (2PC)                                   */
/*----------------------------------------------------------------------------*/
eEC Api::eForkJob2PC(JobId& rtChildJobId,
                     TokenVec* pCmdTknVec, TagSet* pCmdTagSet,
                     JobId tParentId, JobId tOriginId, JobId tDistId,
                     Job*& rpParentJob_tbr,
                     Job*& rpChildJob_tbr)
{
  _static_

  rtChildJobId = C_UNKNOWN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assertions

  RASSERT(! tParentId.bIsUnknown());
  RASSERT(rpParentJob_tbr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prepare job

  CmdHelper::vExtractCmdPaths(pCmdTagSet,pCmdTknVec);

  bool b = CmdHelper::bExtractCmdTags(pCmdTagSet,pCmdTknVec);
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify minimum needed tags

  if (! pCmdTagSet->bHasName(C_TAG_JOBNM)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_JOBNM"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_ENTRYPOINT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_ENTRYPOINT"_Stz);
  }
  if (! pCmdTagSet->bHasName(C_TAG_PATH)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: no C_TAG_PATH"_Stz);
  }

  if ((! pCmdTagSet->bHasName(C_TAG_NOWAIT)) &&
      (! pCmdTagSet->bHasName(C_TAG_WAIT))) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: neither C_TAG_NOWAIT or C_TAG_WAIT"_Stz);
  }
  if (pCmdTagSet->bHasName(C_TAG_NOWAIT) &&
      pCmdTagSet->bHasName(C_TAG_WAIT)) {
    pCmdTagSet->vDumpSet();
    THROW("Api::eForkJob2PC: both C_TAG_NOWAIT and C_TAG_WAIT"_Stz);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz* szJobNm = pCmdTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);

  rpChildJob_tbr = new Job(szJobNm,tParentId,tOriginId,tDistId);


  TagSet* pJobTagSet = rpChildJob_tbr->pJobTags();
  pJobTagSet->vCopy(pCmdTagSet);

  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  rpChildJob_tbr->vCmd(sCmd);

  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  rpChildJob_tbr->vEntryPoint(szEntry);

  Stz* szFolderPath = pJobTagSet->szValue(C_TAG_PATH);
  rpChildJob_tbr->vFolderPath(szFolderPath);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  rtChildJobId = rpChildJob_tbr->tJobId();

  //rpChildJob_tbr->vMutexCreate();
  //rpChildJob_tbr->vStmtListCreate();
  rpChildJob_tbr->vTaskCreate();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  rpParentJob_tbr->vMutexLock();
  RASSERT(MS_LOCKED == rpParentJob_tbr->eMutexSide());

  RASSERT(rpChildJob_tbr->pTask());

  //rpChildJob_tbr->vResponseTimerStart();

  RASSERT(rpChildJob_tbr->bHasTag(C_TAG_NOWAIT));
  RASSERT(! rpChildJob_tbr->bHasTag(C_TAG_WAIT));

  Scheduler::vTaskApi_Add(rpChildJob_tbr->pTask());

  rpParentJob_tbr->vMutexUnlock();
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* set job as reapable                                                        */
/*----------------------------------------------------------------------------*/
void Api::vReapableJob(Job*& rpJob)
{
  _static_
        
  rpJob->vReapableJob();

  rpJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* new a dummy/root job                                                       */
/*----------------------------------------------------------------------------*/
Job* Api::pNewRootJob(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId)
{
  _static_
  
  Job* pRootJob = new Job(szJobNm,tParentId,tOriginId,tDistId);

  return pRootJob;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* call the system boot job to compile the main dictionary                    */
/*----------------------------------------------------------------------------*/
void Api::vCallEngineJob(Stz* szJobNm, Stz* szEntryPt, bool bAddAxioms)
{
  _static_
  
  // this is the main thread
  PsxThd::vMarkNonSeThread();

  JobId tParentId;
  RASSERT(tParentId.bIsUnknown());

  JobId tOriginId;
  RASSERT(tOriginId.bIsUnknown());

  JobId tDistId;
  RASSERT(tDistId.bIsUnknown());

  Job* pRootJob = Api::pNewRootJob("startSystemRoot"_Stz,tParentId,tOriginId,tDistId);
  pRootJob->vOutputCreate();
  pRootJob->vOutputBeginProc();
  pRootJob->vOutputEnabled(true);

  tParentId = pRootJob->tJobId();
  tOriginId = pRootJob->tOriginId();
  tDistId = pRootJob->tDistId();

  Job* pEngineJob   = new Job(szJobNm,tParentId,tOriginId,tDistId);
  Task* pEngineTask = new Task(szJobNm,pEngineJob);
  pEngineJob->vTask(pEngineTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     
  if (bAddAxioms) {
    pEngineTask->vAddAxioms();
    pEngineTask->vAddMunged();
  }
  
  pEngineTask->vSetInxPtr(szEntryPt);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vCallJobImmediate(pEngineJob);
  
  TagSet* pJobTags = pEngineJob->pJobTags();
  //pJobTags->vDumpSet();

  pJobTags->vReplace(C_TAG_PATH,"/"_Stz);
  pJobTags->vReplace(C_TAG_WAIT,1);
  
  pEngineJob->vResponseTimerStart();
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vCallTaskImmediate(pImmediateTask);
  Task* pSaveTask = _pTask(PsxThd::pTlsGet(giTLS_TaskKey));
  
  pEngineTask->vState(TS_EXECUTING);
  
  pEngineTask->vTaskLoop();
  
  PsxThd::vTlsSet(giTLS_TaskKey,pSaveTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Api::vDeleteRootJob(pRootJob);

  pEngineTask->vState(TS_RETIRED);
  pEngineTask->vDropTaskLock((Stz*)__func__);

  //Api::vReapJobsOnce();
  //gpAttachMap->pJobs()->vReapJobsOnce();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bAddAxioms) {
    AM::HANDLERS::pEventHandlers()->vInitialise();
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
