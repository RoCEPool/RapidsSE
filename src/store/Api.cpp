/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "ApiMacros.h"
#include "Api.hpp"
#include "Session.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Op.hpp"
#include "LogicUnit.hpp"
#include "Procedure.hpp"
#include "Sessions.hpp"
#include "Output.hpp"
#include "EventHandlers.hpp"
#include "Grid.hpp"

#include "TagConstants.h"
#include "ePP.h"

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
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
Session* Api::pAllocSession(eSK eSession)
{
  _static_
  
  Sessions* pSessions = AM::pSessions();
  
  Session* pSession = pSessions->pAllocSession(eSession);
    
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
void Api::vFreeSession(Session* pSession)
{
  _static_
  
  Sessions* pSessions = AM::pSessions();

  pSessions->vFreeSession(pSession);
}

/*----------------------------------------------------------------------------*/
/* alloc a new job for session                                                */
/*----------------------------------------------------------------------------*/
Job* Api::pAllocJob(Session* pSession)
{
  _static_

  RASSERT(pSession);
    
  CmdId tCmdId;
  tCmdId.vNextCmdId();
  
  TagSet* pCmdTags = & pSession->m_oCmdTagSet;
  Stz* szJobNm = pCmdTags->szValue(C_TAG_JOBNM);

  Job* pJob = new Job(szJobNm,
                      pSession->m_i_Session,
                      tCmdId
                      );

  return pJob;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute job (spawn job and wait for completion)                            */
/*----------------------------------------------------------------------------*/
void Api::vExecuteJob(Session* pSession, Job* pJob)
{
  _static_
  
  pSession->m_pOutput->vAssertEmptyQueue();

  pSession->m_pOutput->vBeginProcOneCmd();
      

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  JobId tJobId = pJob->tJobId();
  TagSet* pJobTagSet = & pSession->m_oCmdTagSet;
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //RASSERT(! pSession->bHasCascade());

  for (;;) {
  
    i8 iPid = AM::PROCESS::iSePid();
    if (!Process::bTestPid(iPid)) {
      // SE is not running anymore!
      vTrace("Cannot find pid %lld\n"_Stz, iPid);
      return;
    }
    
    vSubmitJob(pSession,pJob);

    if (pJob->bHasTag(C_TAG_NOWAIT)) {
      Api::vMutexUnlock(pSession,pJob);
    }
    else {
      vWaitJob(pSession,pJob);
    }
    
    if (! pSession->bHasCascade()) {
      pSession->vLastJobId(tJobId);
      return;
    }
  
//    if (AM::TRACE::bVerbose(C_VB_API))
//      vTrace("<vb_api>:: cascading: \n"
//             "%s\n"
//             "end <vb_api>\n"_Stz
//             ,pSession->szCascade());
    
    //AM::pHeaps()->vSetTraceFree(true,MY::pTask());
    
    pJob->vCascadeReset();
    
    pJob->vCascadeSet(pSession->szCascade());
    
    pSession->vPopCascade();

    Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
    
    pJob->vEntryPoint(szEntry);

    //AM::pHeaps()->vSetTraceFree(false);
  }
}

/*----------------------------------------------------------------------------*/
/* fork job (spawn job but don't wait for completion)                         */
/*----------------------------------------------------------------------------*/
void Api::vForkJob(Session* pSession, Op* pResponseOp, Job*& rpJob)
{
  _static_
  
  RASSERT(! rpJob->pTask());

  Api::vMutexLock(pSession,rpJob);
  RASSERT(JS_LOCKED == rpJob->eJobState());
  
  rpJob->vTaskCreate();
  
  rpJob->vResponseTimerStart();
  
  Scheduler::vTaskApi_AddPrompt(rpJob->pTask());
  
  Api::vMutexUnlock(pSession,rpJob);
}

/*----------------------------------------------------------------------------*/
/* extract paths                                                              */
/*----------------------------------------------------------------------------*/
void Api::vExtractCmdPaths(TokenVec* pCmdTknVec, TagSet* pCmdTags)
{
  _static_
  
  //TagSet* pJobTagSet = & pSession->m_oJobTagSet;;
  
  i8 i_Paths = pCmdTknVec->iFindToken("paths"_Stz,TN_NAME);

  if (C_UNKNOWN == i_Paths)
    return;
  
  pCmdTknVec->vRemoveToken(i_Paths);

  s32 sName;
  
  i8 nPaths;
  for (nPaths = 0; i_Paths < pCmdTknVec->nTokens(); nPaths++) {
    sName.vSprint("<path%lld>"_Stz,nPaths);
    Token* pTkn = pCmdTknVec->pGet(i_Paths);
    pCmdTags->vAppend(sName,pTkn->szChars());
    pCmdTknVec->vRemoveToken(i_Paths);
  }
  
  pCmdTags->vAppend("<paths>"_Stz,nPaths);

}

/*----------------------------------------------------------------------------*/
/* extract tags                                                               */
/*----------------------------------------------------------------------------*/
bool Api::bExtractCmdTags(TokenVec* pCmdTknVec, TagSet* pCmdTags)
{
  _static_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract leading job control tags (such as /time or /nowait)
  
  Token* pTkn0 = pCmdTknVec->pGet(0,true);
  
  s32 sTag;
  
  while ('/' == * pTkn0->azChars() ) {
    
    Token* pTkn1 = pCmdTknVec->pGet(1,true);
    sTag.vSprint("<%s>"_Stz,pTkn1->szChars());
    
    pCmdTags->vInsert(sTag,1);
    
    pCmdTknVec->vRemoveToken(0);
    pCmdTknVec->vRemoveToken(0);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract trailing job supplemtntal tags ( such as <field_delimiter> = | )

  i8 i_TagBegin = pCmdTknVec->iFirstTag2();
  if (C_UNKNOWN == i_TagBegin)
    return true;

  Token* pTkn_begin = pCmdTknVec->pGet(i_TagBegin);
  UNUSED(pTkn_begin);
  
  i8 nTkns = pCmdTknVec->nTokens();

  i8 i_Scan = i_TagBegin;
  while (i_Scan < nTkns) {
  
    Token* pScan = pCmdTknVec->pGet(i_Scan++);        // <
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsLessThan())
      return false;
    
    Token* pTkn_name = pCmdTknVec->pGet(i_Scan++);    // name
    if (i_Scan >= nTkns)
      return false;
    if (! pTkn_name->bIsName())
      return false;

    pScan = pCmdTknVec->pGet(i_Scan++);               // >
    if (! pScan->bIsGreaterThan())
      return false;
    
    pScan = pCmdTknVec->pGet(i_Scan++);               // =
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsAssign())
      return false;

    Token* pTkn_value = pCmdTknVec->pGet(i_Scan++);   // value
    //if (! pTkn_value->bIsString())
    //  return false;

    s32 sName;
    sName.vSprint("<%s>"_Stz,pTkn_name->szChars());
    pCmdTags->vAppend(sName,pTkn_value->szChars());
  }
    
  pCmdTknVec->vTrimTokens(i_TagBegin);
  
  return true;

}

/*----------------------------------------------------------------------------*/
/* parse command                                                              */
/*----------------------------------------------------------------------------*/
eEC Api::eParseCmd(TokenVec* pCmdTknVec, TagSet* pCmdTags)
{
  _static_

  vExtractCmdPaths(pCmdTknVec,pCmdTags);
  
  bool b = bExtractCmdTags(pCmdTknVec,pCmdTags);
  UNUSED(b);
  
  if (bShortForm(pCmdTknVec)) {
    if (! bExpandToLongForm(pCmdTknVec)) {
      return EC_INVALID_COMMAND;
    }
  }
  else {
    sSTMT sPipelineSmt;
    if (Procedure::bTranslateToPipeline(sPipelineSmt.pString(),
                                          pCmdTknVec)) {
      
      pCmdTknVec->vSoftReset();
      pCmdTknVec->vTokeniseSen(sPipelineSmt);
    }
  }
    
  if (! AM::bParseCmd(pCmdTknVec,pCmdTags))
    return EC_INVALID_COMMAND;


  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* new a and prepare it to execute the command in session                     */
/*----------------------------------------------------------------------------*/
eEC Api::ePrepareJob(Session* pSession, TokenVec* pCmdTknVec, Job*& rpJob)
{
  _static_

  RASSERT(pSession);
  RASSERT(! rpJob);
    
  // get the command proper and the tag set
  TagSet*   pSsnTags   = & pSession->m_oSsnTagSet;
  TagSet*   pCmdTags   = & pSession->m_oCmdTagSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eEC eError = eParseCmd(pCmdTknVec,pCmdTags);
  if (EC_NONE != eError)
    return eError;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  rpJob = pAllocJob(pSession);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TagSet* pPrepareJobTags = rpJob->pJobTags();


  // parse cmd
  sSTMT sStmt;
  pCmdTknVec->vVecToString(sStmt,0,true);
  rpJob->vCmd(sStmt);

  pPrepareJobTags->vInsert(C_TAG_CMD,rpJob->szCmd());
  
  pPrepareJobTags->vCopy(pSsnTags);

  //pCmdTags->vDumpSet(true);
  
  pPrepareJobTags->vCopy(pCmdTags);
  
  if (AM::TRACE::bVerbose(C_VB_API))
    pPrepareJobTags->vDumpSet();

  if (rpJob->bHasTag(C_TAG_LIFE)) {
    SP_TagSet spTagSet;
    vTrace("Job New: %s"_Stz,spTagSet.sz(pPrepareJobTags));
    //pJob->pTagSet()->vPrint();
  }
  
  if ( rpJob->bHasTag(C_SHOW_CMD_TAGS))
    pPrepareJobTags->vDumpSet(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eJK eJobKind = (eJK) pPrepareJobTags->iValue(C_TAG_JOBKIND);
  rpJob->vJobKind(eJobKind);
    
  if (pPrepareJobTags->bHasName(C_SEE_JOB))
    rpJob->vPrintJobContents();
  
  
  //pPrepareJobTags->vDumpSet(true);

  
  //Dic* pDicRoot = AM::DIC::pRoot();
  
  Stz* szEntry = pPrepareJobTags->szValue(C_TAG_ENTRYPOINT);
  
  rpJob->vEntryPoint(szEntry);
  
  
  TagSet* pJobTagSet = & pSession->m_oCmdTagSet;
  
  if (AM::TRACE::bVerbose(C_VB_API)) {
    pCmdTknVec->vDumpVec(false);
    pJobTagSet->vDumpSet(true);
  }
      
  if (AM::bDeveloper() && pCmdTknVec->nTokens() > C_1KB) {
    i8 nTokens = pCmdTknVec->nTokens();
    i8 nUsed   = pCmdTknVec->nUsedByteSpace();
    if (nTokens > C_1KB)
       vTrace("TokenVec usage: nTokens=%lld nUsed=%lld(B)\n"_Stz,nTokens,nUsed);
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* set job as reapable                                                        */
/*----------------------------------------------------------------------------*/
void Api::vReapableJob(Session* pSession, Job*& rpJob)
{
  _static_
    
  if (rpJob && rpJob->bIsPipeline() && ! pSession->m_pOutput->m_bExitMarker)
    pSession->m_pOutput->vProcessBufferQueue(rpJob);

  pSession->m_pOutput->m_pOutputQueue->vReset();
  pSession->m_pOutput->vEndProcOneCmd();
  
  rpJob->vReapable(true);
      
  
  
  if (pSession->m_pOutput->m_pStreamDef_result) {
    StreamDef* pStreamDef = pSession->m_pOutput->m_pStreamDef_result;
    pSession->m_pOutput->m_pStreamDef_result = nullptr;
    delete pStreamDef;
  }
  
  pSession->m_pOutput->m_bExitMarker      = false;
  pSession->m_pOutput->m_bExitEarlyMarker = false;
  
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
/* execute internal command with given tag string                             */
/*----------------------------------------------------------------------------*/
eEC Api::eExecuteCommand(Stz* szCmd, Stz* szWithTagStr, Stz* szTagPath)
{
  _static_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot a session
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  
  eEC reError = pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenise the request
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(szCmd);

  TagSet* pTagSet =  & pSession->m_oCmdTagSet;
  pTagSet->vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);

  Job* pJob = nullptr;
  reError = Api::ePrepareJob(pSession,pCmdTknVec,pJob);

  delete pCmdTknVec;

  if (reError) {
    //m_pDataStack->vPushInt(eError);
    //m_pDataStack->vPushInt(false);
    //EXIT();
    return reError;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // update the job tag set
  
  TagSet* pJobTagSet = pJob->pJobTags();
  
  if (szTagPath) {
    //Stz* szTagPath = m_pJob->pJobTags()->szValue(C_TAG_PATH);
    pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  }
  
  TokenVec* pWithTagStrTknVec = TokenVec::pTokeniseSen(szWithTagStr);
  Api::vPopulateTagSet(pJobTagSet,pWithTagStrTknVec);
  
  
  Now xNow;
  xNow.vSetNow();

  s32 sHostName;
  Hardware::vHostName(sHostName);

  pJobTagSet->vInsert(C_TAG_FREEPCT,gpBufferPool->iFreePercent());
  pJobTagSet->vInsert(C_TAG_GRID,AM::pGrid()->szGridNm());
  pJobTagSet->vInsert(C_TAG_HOST,sHostName);
  pJobTagSet->vInsert(C_TAG_NODE,AM::pGrid()->pThisNode()->szNodeNm());
  pJobTagSet->vInsert(C_TAG_REQUEST,pSession->m_iCmdNo);
  pJobTagSet->vInsert(C_TAG_SHM,gpBufferPool->szShmNm());
  pJobTagSet->vInsert(C_TAG_WHEN,xNow.sz());

  
  
  if (AM::TRACE::bVerbose(C_VB_API))
    pJobTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn job and wait for completion
  
//  bool bSpawned = Api::bSpawnTask_session(pJob);
//  UNUSED(bSpawned);
  pJob->vResponseTimerStart();
  
  Api::vExecuteJob(pSession,pJob);
  
  pJob->vResponseTimerUpdate();

//  vTrace("Job Timer: %lldus\n"_Stz
//         ,pJob->m_oTimer.iMicroseconds());
  
  RASSERT(! pJob->bIsInError() );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete job and free session
  Api::vReapableJob(pSession,pJob);

  Api::vFreeSession(pSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //push result
    
  //m_pDataStack->vPushInt(iJobNo);
  //m_pDataStack->vPushInt(true);
  
  //delete pReqTknVec;
  delete pWithTagStrTknVec;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* fork internal command with given tag string                                */
/*----------------------------------------------------------------------------*/
eEC Api::eForkCommand(Stz* szCmd, Stz* szWithTagStr, Stz* szTagPath,
                      Op* pResponseOp, JobId& rtJobId)
{
  _static_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot a session
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  
  eEC reError = pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenise the request
  
  TokenVec* pCmdTknVec = TokenVec::pTokeniseSen(szCmd);

  TagSet* pTagSet =  & pSession->m_oCmdTagSet;
  pTagSet->vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);

  Job* pJob;
  reError = Api::ePrepareJob(pSession,pCmdTknVec,pJob);

  delete pCmdTknVec;

  if (reError) {
    //m_pDataStack->vPushInt(eError);
    //m_pDataStack->vPushInt(false);
    //EXIT();
    return reError;
  }
    
  rtJobId = pJob->tJobId();
   
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // update the job tag set
  

  TagSet* pJobTagSet = pJob->pJobTags();
  
  if (szTagPath) {
    //Stz* szTagPath = m_pJob->pJobTags()->szValue(C_TAG_PATH);
    pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  }
  
  TokenVec* pWithTagStrTknVec = TokenVec::pTokeniseSen(szWithTagStr);
  Api::vPopulateTagSet(pJobTagSet,pWithTagStrTknVec);
  
  
  Now xNow;
  xNow.vSetNow();

  s32 sHostName;
  Hardware::vHostName(sHostName);

  pJobTagSet->vInsert(C_TAG_FREEPCT,gpBufferPool->iFreePercent());
  pJobTagSet->vInsert(C_TAG_GRID,AM::pGrid()->szGridNm());
  pJobTagSet->vInsert(C_TAG_HOST,sHostName);
  pJobTagSet->vInsert(C_TAG_NODE,AM::pGrid()->pThisNode()->szNodeNm());
  pJobTagSet->vInsert(C_TAG_REQUEST,pSession->m_iCmdNo);
  pJobTagSet->vInsert(C_TAG_SHM,gpBufferPool->szShmNm());
  pJobTagSet->vInsert(C_TAG_WHEN,xNow.sz());


  
  if (AM::TRACE::bVerbose(C_VB_API))
    pJobTagSet->vDumpSet();

#if 1
  pJob->vResponseTimerStart();
  
  Api::vForkJob(pSession,pResponseOp,pJob);

  
#else
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn job and wait for completion
  
//  bool bSpawned = Api::bSpawnTask_session(pJob);
//  UNUSED(bSpawned);
  pJob->vResponseTimerStart();
  
  Api::vExecuteJob(pSession);
  
  pJob->vResponseTimerUpdate();

//  vTrace("Job Timer: %lldus\n"_Stz
//         ,pJob->m_oTimer.iMicroseconds());
  
  RASSERT(! pJob->bIsInError() );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete job and free session
  Api::vReapableJob(pSession);

  Api::vFreeSession(pSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //push result
    
  //m_pDataStack->vPushInt(iJobNo);
  //m_pDataStack->vPushInt(true);
#endif
  
  
  
  //delete pReqTknVec;
  delete pWithTagStrTknVec;

  return EC_NONE;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* submit job (spawn job but don't wait for completion)                       */
/*----------------------------------------------------------------------------*/
void Api::vSubmitJob(Session* pSession, Job* pJob)
{
  _static_
  
  RASSERT(! pJob->pTask());

  Api::vMutexLock(pSession,pJob);
  RASSERT(JS_LOCKED == pJob->eJobState());
  
  pJob->vTaskCreate();
  
  pJob->vResponseTimerStart();
  
  Scheduler::vTaskApi_AddPrompt(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* wait for job completion                                                    */
/*----------------------------------------------------------------------------*/
void Api::vWaitJob(Session* pSession, Job* pJob)
{
  _static_
    
  //pSession->m_oMutex.vObserve(true);
  Api::vMutexWaitCond(pSession,pJob);
  
  pJob->vResponseTimerUpdate();
  
  Api::vMutexUnlock(pSession,pJob);
}

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
//        vTrace("Cant reap retired job %p %lld "
//               "%s %s "
//               "activeOps %lld %s "
//               "\n"_Stz
//
//               ,pRetiredJob,pRetiredJob->tJobId().uJobNo()
//               ,aJK[pRetiredJob->eJobKind()],pRetiredJob->szJobNm()
//               ,nActiveOps,pRetiredJob->m_bReapable ? "reapable" : "not-reapable"
//               );
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
/* is the command a shortform?                                                */
/*----------------------------------------------------------------------------*/
bool Api::bShortForm(TokenVec* pCmdTknVec)
{
  _static_
  
  i8 i_Fan = pCmdTknVec->iFindToken("*|"_Stz);
  if (C_UNKNOWN != i_Fan)
    return true;

  i_Fan = pCmdTknVec->iFindToken("|#"_Stz);
  if (C_UNKNOWN != i_Fan)
    return true;

  return false;
  
}

/*----------------------------------------------------------------------------*/
/* expand shortform into longform                                             */
/*----------------------------------------------------------------------------*/
bool Api::bExpandToLongForm(TokenVec* pCmdTknVec)
{
  _static_
  
  TokenVec* pAllPtnVec;
  TokenVec* pOnOriginVec;
  
  s48K sLongForm;
  
  
  i8 nTkn = pCmdTknVec->nTokens();
  
  i8 i_Fan = pCmdTknVec->iFindToken("*|"_Stz);
  
  if (C_UNKNOWN != i_Fan) {
    pAllPtnVec   = TokenVec::pGetSubVec(pCmdTknVec,0,i_Fan-1);
    pOnOriginVec = TokenVec::pGetSubVec(pCmdTknVec,i_Fan+1,nTkn-1);
    
    sLongForm =
      "pipeline fanin "
      "begin "
      " "
      "  stage stageA on origin : "
      "    start | pulse once | stageB "
      " "
      "  stage stageB on all partitions : "
      "    /ALL_PARTITIONS/ | stageC "
      " "
      "  stage stageC on origin : "
      "    /ON_ORIGIN/ | stageD "
      " "
      "  stage stageD on origin : "
      "    sink | stageA "
      " "
      "end ;"_Stz;
  }
  else {
    i_Fan = pCmdTknVec->iFindToken("|#"_Stz);
    pOnOriginVec = TokenVec::pGetSubVec(pCmdTknVec,0,i_Fan-1);
    pAllPtnVec   = TokenVec::pGetSubVec(pCmdTknVec,i_Fan+1,nTkn-1);
    
    sLongForm =
      "pipeline fanout "
      "begin "
      " "
      "  stage stageA on origin : "
      "    start | pulse once | /ON_ORIGIN/ | stageB hashcast "
      " "
      "  stage stageB on all partitions : "
      "    /ALL_PARTITIONS/ | stageC "
      " "
      "  stage stageC on origin : "
      "    sink | stageA "
      " "
      "end ;"_Stz;
  }
  
  s4K sAllPtn;
  s4K sOnOrigin;

  pAllPtnVec->vToTknString(sAllPtn);
  pOnOriginVec->vToTknString(sOnOrigin);
  
  vTrace("%s\n"_Stz,sAllPtn.azChar());
  vTrace("%s\n"_Stz,sOnOrigin.azChar());

  StrLib::bReplace(sLongForm.azChar(),
                   "/ALL_PARTITIONS/",
                   sAllPtn.azChar());
  
  StrLib::bReplace(sLongForm.azChar(),
                   "/ON_ORIGIN/",
                   sOnOrigin.azChar());
  
  vTrace("%s\n"_Stz,sLongForm.azChar());
  
  pCmdTknVec->vSoftReset();
  
  pCmdTknVec->vTokeniseSen(sLongForm);
  
  return true;
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
  
  CmdId tCmdId;
  tCmdId.vNextCmdId();
  
  Job* pBootJob = new Job("bootSystem"_Stz,
                          AM::SESSION::iInternalSession(),
                          tCmdId
                          );
  
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

  pJobTags->vInsert(C_TAG_PATH,"/"_Stz);
    
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

  CmdId tCmdId;
  tCmdId.vNextCmdId();
  
  Job* pStartJob = new Job("startSystem"_Stz,
                           AM::SESSION::iInternalSession(),
                           tCmdId
                           );
  
  Stz* szJobNm = pStartJob->szJobNm();
  Task* pStartTask = new Task(szJobNm,pStartJob);
  pStartTask->vIsStart(true);

  pStartJob->vTask(pStartTask);
  
  pStartTask->vSetInxPtr("startSystem::startSystem"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  TagSet* pJobTags = pStartJob->pJobTags();
  //pJobTags->vDumpSet();

  // (no cmd to parse)

  pJobTags->vInsert(C_TAG_PATH,"/"_Stz);
  
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
void Api::vSpawnTask_demonJob(Job* pJob)
{
  _static_
  
  RASSERT(! pJob->pTask());

  pJob->vTaskCreate();
  Scheduler::vTaskApi_AddService(pJob->pTask());
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
/* validate the SE session                                                    */
/*----------------------------------------------------------------------------*/
eEC Api::eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall)
{
  _static_
  UNUSED(iTrace);
  UNUSED(eApiCall);

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
  if (pSession->eGetClassNo() != CN_SESSION) {
    TRACE_API_ERROR("eValidateSession(c)", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* spi wait till all jobs completed                                           */
/*----------------------------------------------------------------------------*/
void Api::vSpinTillAllJobsCompleted(void)
{
  _static_
  
  // wait till no local jobs
  AM::pLocalJobs()->vGrabLock();
  while (AM::pLocalJobs()->nJobs())
    ;
  AM::pLocalJobs()->vDropLock();
  
  // wait till no participating jobs
  AM::pParticipatingJobs()->vGrabLock();
  while (AM::pParticipatingJobs()->nJobs())
    ;
  AM::pParticipatingJobs()->vDropLock();
  
  // wait till no originating jobs
  AM::pOriginatingJobs()->vGrabLock();
  while (AM::pOriginatingJobs()->nJobs())
    ;
  AM::pOriginatingJobs()->vDropLock();
  
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

/*----------------------------------------------------------------------------*/
/* lock mutex and mark job as locked                                          */
/*----------------------------------------------------------------------------*/
void Api::vMutexLock(Session* pSession, Job* pJob)
{
  _static_
  
  pSession->m_oMutex.vLock(pSession);
        
  pJob->vJobState(JS_LOCKED);
}

/*----------------------------------------------------------------------------*/
/* mark job as running and unlock session mutex                               */
/*----------------------------------------------------------------------------*/
void Api::vMutexUnlock(Session* pSession, Job* pJob)
{
  _static_
  
  RASSERT(JS_ACTIVE != pJob->eJobState());
    
  pJob->vJobState(JS_ACTIVE);
  
  pSession->m_oMutex.vUnlock(pSession);
}

/*----------------------------------------------------------------------------*/
/* wait for session mutex to be signalled                                     */
/*----------------------------------------------------------------------------*/
void Api::vMutexWaitCond(Session* pSession, Job* pJob)
{
  _static_
  
  do {
    
    //vTrace("WAIT session %p mutex %p\n"_Stz,pSession,&pSession->m_oMutex);

    pSession->m_oMutex.vWait(pSession);
    
  } while (pJob->eJobState() != JS_SIGNALLED);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
