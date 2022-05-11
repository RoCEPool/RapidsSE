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
  
  pEngineTask->vState(TS_RETIRED);
  pEngineTask->vDropTaskLock((Stz*)__func__);

  //Api::vReapJobsOnce();
  gpAttachMap->pJobs()->vReapJobsOnce();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bAddAxioms) {
    AM::HANDLERS::pEventHandlers()->vInitialise();
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
